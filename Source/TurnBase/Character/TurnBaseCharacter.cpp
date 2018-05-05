// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TurnBaseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Controller/TurnBasePlayerController.h"
#include "Character/BaseEnemy/BaseEnemy.h"
#include "Actor/BaseBattleLevelActor.h"
#include "Runtime/Engine/Public/EngineUtils.h"
#include "Globe/CustomMSG.h"
#include "Character/BaseBattlePawn/BasePlayerPawn.h"
#include "Controller/BaseBattlePawnController.h"

ATurnBaseCharacter::ATurnBaseCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(CapsuleSize.X, CapsuleSize.Y);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATurnBaseCharacter::OnMeetEnemy);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeScale3D(MeshScale);
	GetMesh()->SetRelativeLocation(FVector(0.f,0.f,-CapsuleSize.Y));

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PlayerPawns.Add(0, nullptr);
	PlayerPawns.Add(1, nullptr);
	PlayerPawns.Add(2, nullptr);
	PlayerPawns.Add(3, nullptr);
	PlayerPawns.Add(4, nullptr);
	PlayerPawns.Add(5, nullptr);

	DefaultPawn.Add(0);
	DefaultPawn.Add(1);
	DefaultPawn.Add(2);

}

void ATurnBaseCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		ATurnBasePlayerController* PC = Cast<ATurnBasePlayerController>(GetController());
		if (PC && !PC->bIsInBattleLevel)
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}


void ATurnBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ABaseBattleLevelActor> It(GetWorld()); It; ++It)
	{
		if (*It)
		{
			ABaseBattleLevelActor* levelactor = *It;
			BattleLevels.Add((int32)(levelactor->LevelType), levelactor);
		}
	}

	if (DefaultPawn.Num()>0)
	{
		for (int32 i = 0; i < FMath::Min<int32>(DefaultPawn.Num(), 6); i++)
		{
			PlayerPawns[i] = GetWorld()->SpawnActor<ABasePlayerPawn>(ABasePlayerPawn::StaticClass(), PawnsDefaultLoc + FVector(50.f*i, 0.f, 0.f), FRotator(ForceInit));
			if (PlayerPawns[i])
			{
				PlayerPawns[i]->InitPawnFromData(DefaultPawn[i]);

				ABaseBattlePawnController* control = GetWorld()->SpawnActor<ABaseBattlePawnController>(ABaseBattlePawnController::StaticClass());
				if (control)
				{
					control->Possess(PlayerPawns[i]);
				}
			}		
		}
	}	
}

ABasePlayerPawn* ATurnBaseCharacter::GetPlayerPawn(int32 Index)
{
	if (PlayerPawns.Contains(Index))
	{
		return PlayerPawns[Index];
	}
	return nullptr;
}

void ATurnBaseCharacter::ResetPawnsLoc()
{
	for (int32 i = 0; i< 6; i++)
	{
		if (PlayerPawns[i])
		{
			PlayerPawns[i]->ResetForBattleEnd(PawnsDefaultLoc);
		}
	}
}

void ATurnBaseCharacter::OnMeetEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ABaseEnemy* enemy = Cast<ABaseEnemy>(OtherActor);
		if (enemy)
		{
			if (BattleLevels.Num() > 0)
			{
				BattleLevels[0]->BattleStart(this, enemy);
			}
		}
	}
}
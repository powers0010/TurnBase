// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Globe/CustomMSG.h"
#include "TurnBaseCharacter.generated.h"

class ABasePlayerPawn;

UCLASS(Blueprintable)
class ATurnBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATurnBaseCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	virtual void BeginPlay() override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

protected:
	UFUNCTION()
		void OnMeetEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
protected:
	UPROPERTY(EditDefaultsOnly)
		TArray<int32> DefaultPawn;

	UPROPERTY()
		TMap<int32 , ABasePlayerPawn*> PlayerPawns;

	TMap<int32, class ABaseBattleLevelActor*>  BattleLevels;

public:
	ABasePlayerPawn* GetPlayerPawn(int32 Index);

	void ResetPawnsLoc();
};


// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAnimInstance.h"
#include "Character/TurnBaseCharacter.h"


void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* owner = Cast<ACharacter>(TryGetPawnOwner());
	if (owner)
	{
		Speed = owner->GetVelocity().Size();
	}
}


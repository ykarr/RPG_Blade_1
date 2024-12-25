// YunMinSeong. All right reserved. 


#include "AnimInstances/BladeCharacterAnimInstance.h"
#include "Characters/BladeCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UBladeCharacterAnimInstance::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ABladeCharacterBase>(TryGetPawnOwner());
	if (OwningCharacter) {
		OwningCharacterMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UBladeCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (!CharacterValidCheck())return;
	PlayerVelocity = OwningCharacter->GetVelocity();
	GroundSpeed = PlayerVelocity.Size2D();
	bHasAcceleration = OwningCharacterMovementComponent->GetCurrentAcceleration().SizeSquared() > 0;
	IsFalling = OwningCharacterMovementComponent->IsFalling();
	//LocomotionDirection
	LocomotionDirection=UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(),OwningCharacter->GetActorRotation());
}



bool UBladeCharacterAnimInstance::CharacterValidCheck()
{//둘다 성공적으로 얻었다면 True 아닌경우 False
	return OwningCharacterMovementComponent || OwningCharacter;
}

// YunMinSeong. All right reserved. 


#include "AnimInstances/Player/BladePlayerAnimInstance.h"
#include "Characters/BladePlayerCharacter.h"
void UBladePlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (OwningCharacter) {
		OwningPlayerCharacter = CastChecked<ABladePlayerCharacter>(OwningCharacter);
	}
}

void UBladePlayerAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (bHasAcceleration) {
		IdleElapsedTime = 0.f;
		bShouldEnterRelaxState = false;
	}
	else {
		IdleElapsedTime += DeltaSeconds;
		bShouldEnterRelaxState = (IdleElapsedTime>=EnterRelaxStateThreshold);
	}

}

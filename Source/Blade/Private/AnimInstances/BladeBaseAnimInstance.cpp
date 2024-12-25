// YunMinSeong. All right reserved. 


#include "AnimInstances/BladeBaseAnimInstance.h"
#include "BladeFunctionLibrary.h"
bool UBladeBaseAnimInstance::DoesOwnerHavTag(FGameplayTag TagToCheck) const
{
	if (APawn* OwningPawn = TryGetPawnOwner()) {
		return UBladeFunctionLibrary::NativeDoesActorHaveTag(OwningPawn,TagToCheck);
	}
	return false;
}

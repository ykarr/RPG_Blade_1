// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayTagContainer.h"
#include "BladeBaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UBladeBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintPure,meta=(BlueprintThreadSafe))
	bool DoesOwnerHavTag(FGameplayTag TagToCheck) const;
};

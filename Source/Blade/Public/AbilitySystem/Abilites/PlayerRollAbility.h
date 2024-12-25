// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "PlayerRollAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UPlayerRollAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void ComputeRollDirectionAndDistance();
	UPROPERTY(EditAnywhere);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	UPROPERTY(EditAnywhere);
	FScalableFloat RollingDistanceScalableFloat;
};

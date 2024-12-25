// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladeEnemyGameplayAbility.h"
#include "EnemyMeleeAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UEnemyMeleeAttackAbility : public UBladeEnemyGameplayAbility
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
	void DealingDamage(FGameplayEventData Payload);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	FScalableFloat InDamageScalableFloat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> GEDeal_DamageToken;
};

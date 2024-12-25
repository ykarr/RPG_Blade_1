// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladeGameplayAbility.h"
#include "BladeEnemyGameplayAbility.generated.h"

class ABladeEnemyCharacterBase;
class UEnemyCombatComponent;
/**
 * 
 */
UCLASS()
class BLADE_API UBladeEnemyGameplayAbility : public UBladeGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(Blueprintpure, Category = "Blade|Ability")
	ABladeEnemyCharacterBase* GetEnemyCharacterFromActorInfo();
	//UFUNCTION(Blueprintpure, Category = "Blade|Ability")

	UFUNCTION(Blueprintpure, Category="Blade|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();
	
	UFUNCTION(BlueprintCallable)
	void OnEnemyDied();

	UFUNCTION(Blueprintpure, Category="Blade|Ability")
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass,const FScalableFloat& InDamageScalableFloat);
private:
	TWeakObjectPtr<ABladeEnemyCharacterBase> CachedEnemyCharacter;
};

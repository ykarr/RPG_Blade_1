// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladeGameplayAbility.h"
#include "BladePlayerGameplayAbility.generated.h"

class ABladePlayerCharacter;
class ABladeController;
class UPlayerCombatComponent;
/**
 * 
 */
UCLASS()
class BLADE_API UBladePlayerGameplayAbility : public UBladeGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, category = "Blade|Ability")
	ABladePlayerCharacter* GetPlayerCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, category = "Blade|Ability")
	ABladeController* GetPlayerControllerFromActorInfo();

	UFUNCTION(BlueprintPure, category = "Blade|Ability")
	UPlayerCombatComponent* GetPlayerCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Blade|Ability")
	FGameplayEffectSpecHandle MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect>EffectClass, float InWeaponDamage, FGameplayTag InCurrentAttackTypeTag,int32 InCurrentComboCount);

	UFUNCTION(BlueprintCallable, Category="Blade|Ability")
	bool GetAbilityRemainingCooldownByTag(FGameplayTag InCoolDownTag, float& TotalCooldownTime, float& RemainingCooldownTime);

	UFUNCTION(BlueprintCallable, Category="Blade|Ability")
	UPlayerUIComponent* GetPlayerUIComponentFromActorInfo();
private:
	TWeakObjectPtr<ABladePlayerCharacter> CachedBladePlayerCharacter;
	TWeakObjectPtr<ABladeController> CachedBladePlayerController;
};

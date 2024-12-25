// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BladeTypes/BladeStructTypes.h"
#include "BladeAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UBladeAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void OnAbilityInputPressed(const FGameplayTag& InputTag);
	void OnAbilityInputReleased(const FGameplayTag& InputTag);

	UFUNCTION(BlueprintCallable,Category="Blade|Ability",meta=(ApplyLevel="1"))
	void GrantPlayerWeaponAbilities(const TArray<FBladePlayerAbilitySet>& InDefaultWeaponAbilities, const TArray<FBladePlayerSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
	
	UFUNCTION(BlueprintCallable,Category="Blade|Ability") //UPARAM�� Ȥ�� ���� ������ C++���� ����� ����. ���߿� �����ϰų� ����..
	void RemoveGrantPlayerWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandleToRemove);

	UFUNCTION(BlueprintCallable,Category="Blade|Ability")
	bool TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate);

	void NativeAbilityActivate(const FGameplayTag& InputTag);
	void NativeAbilityDeActivate(const FGameplayTag& InputTag);
};

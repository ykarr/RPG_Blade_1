// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BladeTypes/BladeEnumTypes.h"
#include "BladeGameplayAbility.generated.h"

class UBaseCombatComponent;
class UBladeAbilitySystemComponent;
UENUM(blueprinttype)
enum class EBladeAbilityActivationPolicy: uint8
{
	OnTriggered,
	OnGiven
};
/**
 * 
 */
UCLASS()
class BLADE_API UBladeGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
protected:
	/** Called when the ability is given to an AbilitySystemComponent */
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)override;
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	UPROPERTY(EditDefaultsOnly, Category = "Blade|Ability")
	EBladeAbilityActivationPolicy AbilityActivationPolicy = EBladeAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure, category = "Blade|Ability")
	UBaseCombatComponent* GetPawnCombatComponentFromActoInfo() const;

	UFUNCTION(BlueprintPure, category = "Blade|Ability")
	UBladeAbilitySystemComponent* GetBladeAbilitySystemComponentFromActoInfo()const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle);
	UFUNCTION(BlueprintCallable, category = "Blade|Ability",meta=(DisplayName="ApplyGameplayEffectSpecHandleToTargetActor",ExpandEnumAsExecs="OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EBladeSuccessType& OutSuccessType);

	UFUNCTION(BlueprintCallable, Category = "Blade|Ability")
	void NativeApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,const TArray<FHitResult>& InHitResults);
};

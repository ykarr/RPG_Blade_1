// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "PlayerRageAbility.generated.h"

class UAnimMontage;
/**
 * 
 */
UCLASS()
class BLADE_API UPlayerRageAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	UPROPERTY(EditDefaultsOnly,Category="Anim")
	UAnimMontage* PlayMontage;
	//UPROPERTY(EditDefaultsOnly,Category="Cost")
	//UGameplayEffect* RageCost;
	UPROPERTY(EditDefaultsOnly,Category="Cost")
	TSubclassOf<UGameplayEffect> RageCost;



	UFUNCTION()
	void OnMontageEnded();
	
	UFUNCTION()
	void OnRecivedBeginEvent(FGameplayEventData Payload);
	UFUNCTION()
	void OnRecivedEndEvent();

	FActiveGameplayEffectHandle ActiveGameplayEffectHandle;
};

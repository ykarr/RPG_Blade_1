// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "BladeTypes/BladeStructTypes.h"
#include "PlayerEquipAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UPlayerEquipAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
protected:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToPlayer();

	UFUNCTION(BlueprintCallable)
	void LinkWeaponData();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	FGameplayTag InWeaponTag;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	FName SocketName;
};

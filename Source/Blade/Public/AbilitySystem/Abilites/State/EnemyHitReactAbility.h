// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladeEnemyGameplayAbility.h"
#include "EnemyHitReactAbility.generated.h"

class UGameplayEffect;
/**
 * 
 */
UCLASS()
class BLADE_API UEnemyHitReactAbility : public UBladeEnemyGameplayAbility
{
	GENERATED_BODY()
protected:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	void SetMaterials();
	void ResetMaterials();

	void ActivateEvent(const FGameplayEventData* TriggerEventData);
	UPROPERTY(EditDefaultsOnly,Category = "Data")
	bool bFaceAttacker = true;
	//UPROPERTY(EditDefaultsOnly,Category = "Data")
	//bool bApplyStance = true;
	UPROPERTY(EditDefaultsOnly,Category = "Data")
	bool bHasHitReactMontagesToPlay = true;
	UPROPERTY(EditDefaultsOnly,Category = "Data")
	bool bHasChildMesh=false;
	UPROPERTY(EditDefaultsOnly,Category = "Data")
	TSubclassOf<UGameplayEffect> EnemyUnderAttack;

	void SetPlayMontage();

	UPROPERTY(EditDefaultsOnly,Category = "Data")
	TArray<UAnimMontage*> MontageToPlay;
private:
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageBlendOut();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCancelled();
};

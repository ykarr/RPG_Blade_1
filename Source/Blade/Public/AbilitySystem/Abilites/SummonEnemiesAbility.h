// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladeEnemyGameplayAbility.h"
#include "SummonEnemiesAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API USummonEnemiesAbility : public UBladeEnemyGameplayAbility
{
	GENERATED_BODY()
protected:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Data")
	int32 NumToSpawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Data")
	float RandomSpawnRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Data")
	TSoftClassPtr<ABladeEnemyCharacterBase> SoftEnemyClassToSpawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Data")
	UAnimMontage* PlayToMontage;
private:
	//UAbilityTask_PlayMontageAndWait
	UFUNCTION()
	void OnMontageCompleted();
	UFUNCTION()
	void OnMontageBlendOut();
	UFUNCTION()
	void OnMontageInterrupted();
	UFUNCTION()
	void OnMontageCancelled();
	//UAbilityTask_WaitSpawnEnemies
	UFUNCTION()
	void OnSpawnFinished(const TArray<ABladeEnemyCharacterBase*>& SpawnedEnemy);
};

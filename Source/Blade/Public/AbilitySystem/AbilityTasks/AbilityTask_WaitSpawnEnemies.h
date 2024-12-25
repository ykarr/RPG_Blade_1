// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitSpawnEnemies.generated.h"


class ABladeEnemyCharacterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitSpawnEnemiesDelegate, const TArray<ABladeEnemyCharacterBase*>&, SpawnedEnemies);
/**
 * 
 */
UCLASS()
class BLADE_API UAbilityTask_WaitSpawnEnemies : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,Category="Blade|AbilityTasks",meta=(DisplayName="Wait GameplayEvent And Spawn Enemies"))
	static UAbilityTask_WaitSpawnEnemies* WaitSpawnEnemies(
		UGameplayAbility* OwningAbility, 
		FGameplayTag EventTag, 
		TSoftClassPtr<ABladeEnemyCharacterBase> SoftEnemyClassToSpawn, 
		const FVector& SpawnOrigin,
		int32 NumToSpawn =1,
		float RandomSpawnRadius=200.f
	);

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate OnSpawnFinished;

	UPROPERTY(BlueprintAssignable)
	FWaitSpawnEnemiesDelegate DidNotSpawn;
	//UGameplayTag Interface
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;
private:
	FGameplayTag CachedEventTag;
	TSoftClassPtr<ABladeEnemyCharacterBase> CachedSoftEnemyClassToSpawn;
	FVector CachedSpawnOrigin;
	int32 CachedNumToSpawn;
	float CachedRandomSpawnRadius;
	FDelegateHandle DelegateHandle;

	void OnGameplayEventRecived(const FGameplayEventData* InPayload);
	
	void OnEnemyClassLoaded();
};



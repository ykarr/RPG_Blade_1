// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladeEnemyGameplayAbility.h"
#include "EnemyProjectileAttackAbility.generated.h"

struct FScalableFloat;
/**
 * 
 */
UCLASS()
class BLADE_API UEnemyProjectileAttackAbility : public UBladeEnemyGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void StartProjectileSpawn(FGameplayEventData Data);
	//void StartProjectileSpawnInSocket(FName& SocketName);
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	UPROPERTY(EditDefaultsOnly,Category="Projectile Data")
	TSubclassOf<AActor> ProjectileObjectClass;

	UPROPERTY(EditDefaultsOnly,Category="Projectile Data")
	bool bSpawnInSocket = false;
	UPROPERTY(EditDefaultsOnly,Category="Projectile Data")
	FName SpawnInSocket;
	
	UPROPERTY(EditDefaultsOnly,Category="Projectile Data")
	FScalableFloat DamageScalableFloat;
	UPROPERTY(EditDefaultsOnly,Category="Projectile Data")
	TSubclassOf<UGameplayEffect> DealingEffectClass;
};

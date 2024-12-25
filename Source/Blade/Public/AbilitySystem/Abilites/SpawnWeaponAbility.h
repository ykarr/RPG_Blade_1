// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladeGameplayAbility.h"
#include "SpawnWeaponAbility.generated.h"

class AWeaponBase;
/**
 * 
 */
UCLASS()
class BLADE_API USpawnWeaponAbility : public UBladeGameplayAbility
{
	GENERATED_BODY()
public:
	USpawnWeaponAbility();
protected:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WeaponSet")
	TSubclassOf<AWeaponBase> WeaponClassToSpawn;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponSet")
	FName SocketName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponSet")
	FGameplayTag WeaponTagToRegister;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponSet")
	bool RegisterAsEquippedWeapon = false;

	UPROPERTY(BlueprintReadonly)
	AWeaponBase* SpawnWeapon;
};

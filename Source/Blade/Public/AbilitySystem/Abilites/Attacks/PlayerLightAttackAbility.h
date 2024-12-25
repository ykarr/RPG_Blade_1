// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "PlayerLightAttackAbility.generated.h"

class UAnimMontage;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class BLADE_API UPlayerLightAttackAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Attack")
	TMap<int32, UAnimMontage*> AttackMontageMap;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Attack")
	int32 CurrentLightAttackComboCount=1;

	UFUNCTION(BlueprintPure)
	UAnimMontage* FindCurrentAttackMontage();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Attack")
	FTimerHandle ComboCountResetTimerHandle;

	UFUNCTION(BlueprintCallable)
	void CalcComboCount();

	// 기본 구현이 제공되고, 블루프린트에서 재정의할 수 있습니다.
	UFUNCTION(BlueprintCallable)
	void ResetAttackComboCount();
	
	//Dealing Damage
	UFUNCTION(BlueprintCallable)
	void DamageDealing(FGameplayEventData Payload);
	UFUNCTION(BlueprintCallable, Category = "SpecialAttack")
	void HandleApplyDamage(FGameplayEventData Payload);
	UFUNCTION(BlueprintCallable)
	void CoolDownStart();
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Damage")
	TSubclassOf<UGameplayEffect> GEDeal_DamageToken;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Damage")
	FGameplayTag InCurrentAttackTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	int32 CachedAttackComboCount = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rage")
	TSubclassOf<UGameplayEffect> GainRageClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpecialAttack")
	FGameplayTag GameplayCueTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpecialAttack",meta=(Categories="InputTag"))
	FGameplayTag AbilityCoolDownTag;
};

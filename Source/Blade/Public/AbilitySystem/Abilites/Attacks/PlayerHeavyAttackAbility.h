// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "PlayerHeavyAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UPlayerHeavyAttackAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	TMap<int32, UAnimMontage*> AttackMontageMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	int32 CurrentHeavyAttackComboCount = 1;

	UFUNCTION(BlueprintPure)
	UAnimMontage* FindCurrentAttackMontage();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	FTimerHandle ComboCountResetTimerHandle;

	UFUNCTION(BlueprintCallable)
	void CalcComboCount();

	UFUNCTION(BlueprintCallable)
	void ResetAttackComboCount();
	
	UFUNCTION(BlueprintCallable)
	void PlayerCanFinishAttack();

	//Dealing Damage
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Damage")
	int32 CachedAttackComboCount = 1;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	FGameplayTag InCurrentAttackTypeTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Damage")
	TSubclassOf<UGameplayEffect> GEDeal_DamageToken;

	UFUNCTION(BlueprintCallable)
	void DealingDamage(FGameplayEventData Payload);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Rage")
	TSubclassOf<UGameplayEffect> GainRageClass;
	//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	//TSubclassOf<UGameplayEffect> GEDeal_DamageToken;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	//FGameplayTag InCurrentAttackTypeTag;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	//int32 CachedAttackComboCount = 1;
};

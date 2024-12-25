// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "PlayerBlockAbility.generated.h"

class UBladeAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class BLADE_API UPlayerBlockAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()

public:

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;
	UFUNCTION(BlueprintCallable, Category="Block")
	void SetBlockActiveTIme();
	UFUNCTION(BlueprintCallable, Category="Block")
	void SetGameplayMagicshieldParms();
	UFUNCTION(BlueprintCallable, Category = "Block")
	void ReceiveBlock(FGameplayEventData Payload);
	void StartResetFinishAttackTimer();
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FName FXMagicShieldTag="GameplayCue.FX.MagicShield";
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FName ExecutSuccessfulBlockTag="GameplayCue.FX.MagicShield.SuccessfulBlock";
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	FName ExecutePerfectBlockTag="GameplayCue.FX.MagicShield.PerfectBlock";

	UPROPERTY(EditDefaultsOnly, Category = "Block")
	bool bIsRemoveTagToEnd=true;
	UPROPERTY(EditDefaultsOnly,BlueprintReadonly, Category = "Block")
	bool bIsPerfectBlock;
	UPROPERTY(EditDefaultsOnly, Category = "Block")
	float Strength=80.f;

	// TSharedPtr로 선언한 Root Motion Constant Force
	//struct FRootMotionSourceHandle* RootMotionHandle;
	
private:
	//UPROPERTY()
	//FTimerHandle DelayTimerHandle;
	float BlockActiveTime;
	// 종료 시 제거할 GameplayCue들을 추적하는 배열
	TArray<FGameplayTag> TrackedGameplayCues;
	UBladeAbilitySystemComponent* BladeAbilitySystemComp;
	
	
	uint16 RootMotionSourceHandle;

	
};

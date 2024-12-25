// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "SpecialAttackAbility.generated.h"

class UNiagaraSystem;
class UCapsuleComponent;
class UCameraComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class BLADE_API USpecialAttackAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
protected:

	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;
	void CheckDuplicate(TArray<FHitResult>& InHits);
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag SpecialAttackCoolDownTag;
	UPROPERTY(EditDefaultsOnly)
	bool bEnableDebug=false;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;
	UPROPERTY(EditDefaultsOnly)
	TArray<TEnumAsByte<	EObjectTypeQuery> > ObjectTypes;
	UFUNCTION(BlueprintCallable)
	void SetSpecialAttackCoolTime();
	UFUNCTION(BlueprintCallable)
	void BlinkDash(float DashDistance, UNiagaraSystem* InNiagaraSystem,UAnimMontage* InAnimMontage,bool SpawnStartEmitter=false, bool SpawnEndEmitter = false);
	UFUNCTION()
	void OnBlinkDashTick(float DeltaTime);
	UFUNCTION()
	void OnBlinkFinish();
	UFUNCTION()
	void OnAnimEnded();
	void ApplyAreaOfEffect(const TArray<FHitResult>& InHitResults);
	void TryTraceFunc( FRotator InOrient,float InDistance);
	void AttackAnimSet(UAnimMontage* InAnimMontage);
private:
	FVector TargetPos;
	UCapsuleComponent* PlayerCapsuleComponent;
	FRotator TargetRelativeRot;
	bool IsMoving=false;
	UCameraComponent* PlayerCameraComponent;

};

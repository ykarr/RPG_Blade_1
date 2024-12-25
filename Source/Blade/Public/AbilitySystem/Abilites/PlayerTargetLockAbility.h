// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "PlayerTargetLockAbility.generated.h"


class UTargetLockWidget;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class BLADE_API UPlayerTargetLockAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;
private:
	void TryLockOnTarget();
	//Target Lock이 가능한 Target수집.
	void GetAvailableActorsToTarget();
	void CancelTargetLockAbility();
	void CleanUp();
	void DrawTargetLockWidget();
	void SetTargetLockWidgetLoc();
	void InitTargetLockMovement();
	void ResetTargetLockMovement();
	void InitTargetLockInputMappingContext();
	void ResetTargetLockInputMappingContext();
	bool ThisActorHasTag(FGameplayTag InTag);
	void GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight);
	UFUNCTION()
	void SwitchTarget( FGameplayEventData Data);
	/*Tick을 사용하기 위해 바인딩해야하는 함수. UFUNCTION 필수*/
	UFUNCTION()
	void OnTargetLockTick(float DeltaTime);
	/*여기까지*/
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors);

	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	float TraceDistance = 5000.f;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	float TargetLockRotationInterpSpeed = 5.f;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	TArray<TEnumAsByte<	EObjectTypeQuery> > BoxTraceChannel;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	bool bIsDebugging;
	UPROPERTY()
	TArray<AActor* > AvailableActorsToLock;
	UPROPERTY()
	AActor* CurrentLockActor;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	TSubclassOf<UTargetLockWidget> TargetLockWidgetClass;

	UPROPERTY()
	FVector2D TargetLockBoxSIze = FVector2D::ZeroVector;
	UPROPERTY()
	UTargetLockWidget* TargetLockWidget;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	UInputMappingContext* TargetLockMappingContext;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	FGameplayTag SwichTargetTag;
	UPROPERTY(EditDefaultsOnly,Category="Target Lock")
	float TargetLockCameraOffsetDistance = 20.f;
};

// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "PlayerPickupPotionAbility.generated.h"

class APickupPotionBase;
/**
 * 
 */
UCLASS()
class BLADE_API UPlayerPickupPotionAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
protected:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;
	UFUNCTION(BlueprintCallable)
	void CollectStones();
	UFUNCTION(BlueprintCallable)
	void ConsumePotions();
private:
	UPROPERTY(EditDefaultsOnly )
	float BoxTraceDistance=50.f;
	
	UPROPERTY(EditDefaultsOnly )
	FVector TraceBoxSize = FVector(100.f);

	UPROPERTY(EditDefaultsOnly )
	TArray<TEnumAsByte<	EObjectTypeQuery> > PotionTraceChannel;

	UPROPERTY(EditDefaultsOnly )
	bool TraceDebug;

	TArray<APickupPotionBase*> CollectedPotions;
};

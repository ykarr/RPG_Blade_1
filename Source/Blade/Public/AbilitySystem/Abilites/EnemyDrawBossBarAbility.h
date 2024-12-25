// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladeEnemyGameplayAbility.h"
#include "EnemyDrawBossBarAbility.generated.h"

class UBossHealthBarWidget;
/**
 * 
 */
UCLASS()
class BLADE_API UEnemyDrawBossBarAbility : public UBladeEnemyGameplayAbility
{
	GENERATED_BODY()
public:
protected:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)override;
	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;
	
	UPROPERTY(EditDefaultsOnly,Category="BossData")
	FText BossName;
	UPROPERTY(EditDefaultsOnly,Category="BossData")
	TSubclassOf< UBossHealthBarWidget> BossHealthBarWidgetClass;
	UPROPERTY()
	UBossHealthBarWidget* BossHealthBarWidget;
};
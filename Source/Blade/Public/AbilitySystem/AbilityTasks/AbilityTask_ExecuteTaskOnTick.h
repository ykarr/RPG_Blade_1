// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExecuteTaskOnTick.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTickDelegate, float, DeltaTime);
/**
 * 
 */
UCLASS()
class BLADE_API UAbilityTask_ExecuteTaskOnTick : public UAbilityTask
{
	GENERATED_BODY()
public:
	UAbilityTask_ExecuteTaskOnTick();
	//UFUNCTION(BlueprintCallable,Category="Blade|AbilityTasks",meta=(Hidepin="OwningAbility",DefaultToSelf="OwningAbility",blueprintInternalUseOnly="true"))
	//static UAbilityTask_ExecuteTaskOnTick* ExecuteTaskOnTick(UGameplayAbility* OwningAbility);

	//UFUNCTION(BlueprintCallable,Category="Blade|AbilityTasks",meta=(Hidepin="OwningAbility",DefaultToSelf="OwningAbility",blueprintInternalUseOnly="true"))
	UFUNCTION(BlueprintCallable)
	static UAbilityTask_ExecuteTaskOnTick* ExecuteTaskTick(UGameplayAbility* OwningAbility);
	//Gameplaytask Interface
	virtual void TickTask(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTickDelegate OnAbilityTaskTick;
};

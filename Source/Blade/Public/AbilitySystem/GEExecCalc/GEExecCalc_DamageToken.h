// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GEExecCalc_DamageToken.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UGEExecCalc_DamageToken : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGEExecCalc_DamageToken();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};

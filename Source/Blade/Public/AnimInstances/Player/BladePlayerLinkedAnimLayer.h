// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/BladeBaseAnimInstance.h"
#include "BladePlayerLinkedAnimLayer.generated.h"

class UBladePlayerAnimInstance;
/**
 * 
 */
UCLASS()
class BLADE_API UBladePlayerLinkedAnimLayer : public UBladeBaseAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure,meta=(BlueprintThreadSafe))
	UBladePlayerAnimInstance* GetPlayerAnimInstance() const;
};

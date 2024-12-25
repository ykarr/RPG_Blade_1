// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnExtentionComponent.h"
#include "PawnUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);
/**
 * 
 */
UCLASS()
class BLADE_API UPawnUIComponent : public UPawnExtentionComponent
{
	GENERATED_BODY()
public:
	//BlueprintAssignable 프로퍼티를 블루프린트에서 할당할 수 있도록 노출시킵니다.
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHealthChanged;
};

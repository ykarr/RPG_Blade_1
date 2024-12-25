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
	//BlueprintAssignable ������Ƽ�� �������Ʈ���� �Ҵ��� �� �ֵ��� �����ŵ�ϴ�.
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentHealthChanged;
};

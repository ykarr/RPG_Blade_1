// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "TargetLockWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class BLADE_API UTargetLockWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	USizeBox* TargetLockBox ;
};

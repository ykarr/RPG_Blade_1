// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"

class UBladeWidgetBase;
/**
 * 
 */
UCLASS()
class BLADE_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
public:
	void RegisterEnemyWidget(UBladeWidgetBase* BladeWidgetBase);
	void RemoveEnemyWidgetIfAny();

	TArray<UBladeWidgetBase*> BladeWidgets;
};

// YunMinSeong. All right reserved. 


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/BladeWidgetBase.h"
#include "CustomDebugHelper.h"
void UEnemyUIComponent::RegisterEnemyWidget(UBladeWidgetBase* BladeWidgetBase)
{
	BladeWidgets.Add(BladeWidgetBase);
}

void UEnemyUIComponent::RemoveEnemyWidgetIfAny()
{
	if (BladeWidgets.IsEmpty()) {
		return;
	}
	for (UBladeWidgetBase* Widget : BladeWidgets) {
		if (Widget) {
			//Debug::Print("RemoveFromParent");
			Widget->RemoveFromParent();
		}
	}

	BladeWidgets.Empty();
}

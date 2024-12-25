// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "EnemyHealthBarBase.generated.h"

class UBladeProgressBarWidget;
/**
 * 
 */
UCLASS()
class BLADE_API UEnemyHealthBarBase : public UBladeWidgetBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UBladeProgressBarWidget* EnemyHealthBar;

	virtual void OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningUIComponent)override;

	UFUNCTION()
	void CurrentHealthChanged(float value);
	bool FirstHide = true;
	FTimerHandle InvisibleTimerHandle;
};

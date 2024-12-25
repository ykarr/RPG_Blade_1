// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "BossHealthBarWidget.generated.h"

class UBladeProgressBarWidget;
class UTextBlock;
/**
 * 
 */
UCLASS()
class BLADE_API UBossHealthBarWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
public:
	void SetBossName(const FText& InBossName);
protected:
	virtual void OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningUIComponent)override;
	UFUNCTION()
	void CurrentHealthChanged(float value);
	virtual void NativeDestruct()override;
	UPROPERTY(meta = (BindWidget))
	UBladeProgressBarWidget* EnemyHealthBar;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BossName;
private:
};

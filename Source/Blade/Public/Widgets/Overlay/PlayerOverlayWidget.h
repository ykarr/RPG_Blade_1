// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "PlayerOverlayWidget.generated.h"

class UBladeProgressBarWidget;
class UIconSlotWidget;
class UUserGuideWidget;
/**
 * 
 */
UCLASS()
class BLADE_API UPlayerOverlayWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningUIComponent) override;
	UFUNCTION()
	void CurrentHealthChanged(float value);
	UFUNCTION()
	void CurrentRageChanged(float value);
	UFUNCTION()
	void OnWeaponChanged(TSoftObjectPtr<UTexture2D> SoftWeaponIcon);
	UFUNCTION()
	void OnKeyGuideInteract(bool bShouldDisplayKeyGuide);


	UPROPERTY(meta=(BindWidget))
	UBladeProgressBarWidget* HealthBar;
	UPROPERTY(meta=(BindWidget))
	UBladeProgressBarWidget* RageBar;
	UPROPERTY(meta=(BindWidget))
	UIconSlotWidget* CurrentWeaponIconSlot;
	UPROPERTY(meta=(BindWidget))
	UUserGuideWidget* KeyGuideWidget;
};

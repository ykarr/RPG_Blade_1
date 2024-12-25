// YunMinSeong. All right reserved. 


#include "Widgets/Overlay/PlayerOverlayWidget.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Widgets/ProgressBar/BladeProgressBarWidget.h"
#include "Widgets/Slot/IconSlotWidget.h"
#include "Widgets/Guide/UserGuideWidget.h"

void UPlayerOverlayWidget::OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningUIComponent)
{
	OwningUIComponent->OnCurrentHealthChanged.AddDynamic(this, &ThisClass::CurrentHealthChanged);
	OwningUIComponent->OnCurrentRageChanged.AddDynamic(this, &ThisClass::CurrentRageChanged);
	OwningUIComponent->OnEquippedWeaponChanged.AddDynamic(this, &ThisClass::OnWeaponChanged);
	OwningUIComponent->OnPotionInteractedDelegate.AddDynamic(this, &ThisClass::OnKeyGuideInteract);
	KeyGuideWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerOverlayWidget::CurrentHealthChanged(float value)
{
	HealthBar->SetBladeProgressBarPercent(value);
}

void UPlayerOverlayWidget::CurrentRageChanged(float value)
{
	RageBar->SetBladeProgressBarPercent(value);
}

void UPlayerOverlayWidget::OnWeaponChanged(TSoftObjectPtr<UTexture2D> SoftWeaponIcon)
{
	CurrentWeaponIconSlot->SetSlotImageToSoftTextureIcon(SoftWeaponIcon);
}

void UPlayerOverlayWidget::OnKeyGuideInteract(bool bShouldDisplayKeyGuide)
{
	if (bShouldDisplayKeyGuide) {
		KeyGuideWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		FText KeyText = FText::FromString(TEXT("T"));
		//FText KeyText;
		KeyGuideWidget->SetKeyGuideText(KeyText);
	}
	else {
		KeyGuideWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

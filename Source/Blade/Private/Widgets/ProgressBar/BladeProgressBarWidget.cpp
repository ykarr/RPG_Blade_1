// YunMinSeong. All right reserved. 


#include "Widgets/ProgressBar/BladeProgressBarWidget.h"
#include "Components/SizeBox.h"

#include "Components/ProgressBar.h"

void UBladeProgressBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UBladeProgressBarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetOverrideSize(BladeSizeBoxWidth, BladeSizeBoxHeight);
	SetBladeProgressBarPercent(BladeProgressBar->Percent);
}
void UBladeProgressBarWidget::SetOverrideSize(float Width, float Height)
{
	BladeSizeBox->SetHeightOverride(Height);
	BladeSizeBox->SetWidthOverride(Width);
}

void UBladeProgressBarWidget::SetBladeProgressBarPercent(float value)
{
	if (ToggleProgressBarColor) {
		FLinearColor LocalColor;
		//if (value >= 1.f) {
		//	LocalColor = ProgressFillColor;k
		//}
		if (value >= 0.55f) {
			LocalColor = ProgressBaseColor;
		}
		else if (value >= 0.25) {
			LocalColor = ProgressWarningColor;

		}
		else {
			LocalColor = ProgressCriticalColor;
		}
		BladeProgressBar->SetFillColorAndOpacity(LocalColor);
	}
	BladeProgressBar->SetPercent(value);
}

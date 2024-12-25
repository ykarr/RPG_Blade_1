// YunMinSeong. All right reserved. 


#include "Widgets/Slot/IconSlotWidget.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"

void UIconSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetSlotImageToSoftTextureIcon(nullptr);
}

void UIconSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetOverrideSize(BladeSizeBoxWidth, BladeSizeBoxHeight);
	
}

void UIconSlotWidget::SetOverrideSize(float Width, float Height)
{
	BladeSizeBox->SetHeightOverride(Height);
	BladeSizeBox->SetWidthOverride(Width);
	SetSlotImageToSoftTextureIcon(ItemIcon);
	
}

void UIconSlotWidget::SetSlotImageToSoftTextureIcon(TSoftObjectPtr<UTexture2D> InTexture)
{
	
	if (InTexture.ToSoftObjectPath().IsValid()) {
		BladeImage->SetBrushFromSoftTexture(InTexture, true);
		BladeImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	//if (InTexture.IsValid()) {
	//	BladeImage->SetBrushFromSoftTexture(InTexture, true);
	//	BladeImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	//}
	//else if (InTexture.ToSoftObjectPath().IsValid()) {

	//}
	else {
		BladeImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

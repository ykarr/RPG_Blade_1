// YunMinSeong. All right reserved. 


#include "Widgets/Guide/UserGuideWidget.h"
#include "Components\SizeBox.h"
#include "Components\TextBlock.h"
#include "EnhancedInputSubsystems.h"
void UUserGuideWidget::SetKeyGuideText(FText& GuideKey)
{
	if (Key_GuideAction) {
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetOwningLocalPlayer());
		if (SubSystem) {
			TArray<FKey> GuideKeyArray= SubSystem->QueryKeysMappedToAction(Key_GuideAction);
			if (!GuideKeyArray.IsEmpty()) {
				GuideKeyText->SetText(GuideKeyArray[0].GetDisplayName());
			}
		}
		
	}
	else {
		GuideKeyText->SetText(GuideKey);
	}
}
void UUserGuideWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetOverrideSize(BladeSizeBoxWidth, BladeSizeBoxHeight);
}

void UUserGuideWidget::SetOverrideSize(float Width, float Height)
{
	BladeSizeBox->SetHeightOverride(Height);
	BladeSizeBox->SetWidthOverride(Width);
	
}

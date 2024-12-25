// YunMinSeong. All right reserved. 


#include "Widgets/GameModeWidget/WaveTextNoCountWidget.h"
#include "Animation/WidgetAnimation.h"
//#include "BladeFunctionLibrary.h"
#include "Components/TextBlock.h"
void UWaveTextNoCountWidget::SetWaveMainText(FText* InMainText)
{
	if (InMainText) {
		WaveMainText->SetText(*InMainText);
	}
}
void UWaveTextNoCountWidget::NativeConstruct()
{
	Super::NativeConstruct();
	WaveEntryAnimationEvent.Clear();
	WaveEntryAnimationEvent.BindUFunction(this,"OnWaveEntryAnimEnded");
	BindToAnimationFinished(WaveEntryAnimation, WaveEntryAnimationEvent);
	PlayAnimation(WaveEntryAnimation);
}

void UWaveTextNoCountWidget::OnWaveEntryAnimEnded()
{
	WaveFadeOutAnimationEvent.Clear();
	WaveFadeOutAnimationEvent.BindUFunction(this, "OnWaveFadeOutAnimEnded");
	BindToAnimationFinished(WaveFadeOutAnimation, WaveFadeOutAnimationEvent);
	PlayAnimation(WaveFadeOutAnimation);
}

void UWaveTextNoCountWidget::OnWaveFadeOutAnimEnded()
{
	RemoveFromParent();
}

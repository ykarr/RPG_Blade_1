// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "WaveTextNoCountWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class BLADE_API UWaveTextNoCountWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
public:
	void SetWaveMainText(FText* InMainText);
protected:
	//WidgetAnimation�� ����ϱ� ���ؼ��� Transient,meta=(BindWidgetAnim) �ʿ�. 
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* WaveEntryAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* WaveFadeOutAnimation;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WaveMainText;
	virtual void NativeConstruct() override;
private:
	FWidgetAnimationDynamicEvent WaveEntryAnimationEvent;
	FWidgetAnimationDynamicEvent WaveFadeOutAnimationEvent;
	UFUNCTION()
	void OnWaveEntryAnimEnded();
	UFUNCTION()
	void OnWaveFadeOutAnimEnded();
};

// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "BladeTypes\BladeEnumTypes.h"
#include "WaveTextWidget.generated.h"

class UWidgetAnimation;
class UTextBlock;
/**
 * 
 */
UCLASS()
class BLADE_API UWaveTextWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
public:
	//WidgetAnimation�� ����ϱ� ���ؼ��� Transient,meta=(BindWidgetAnim) �ʿ�. 
	UPROPERTY(Transient,meta=(BindWidgetAnim))
	UWidgetAnimation* WaveEntryAnimation;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* CountDownText;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* WaveMainText;
	void StartCountDown(float TotalTime);
	void SetWaveMainText(FText* InTextDisplay);
private:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void UpdateCount();
	EBladeCountDownActionOutput CountDownActionOutput;
	float RemaingTime;
};

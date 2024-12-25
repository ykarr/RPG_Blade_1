// YunMinSeong. All right reserved. 


#include "Widgets/GameModeWidget/WaveTextWidget.h"
#include "Animation/WidgetAnimation.h"
#include "BladeFunctionLibrary.h"
#include "Components/TextBlock.h"

#include "CustomDebugHelper.h"
void UWaveTextWidget::StartCountDown(float TotalTime)
{
	RemaingTime = TotalTime;
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;  // 콜백이 실행될 타겟 (보통 자신)
	LatentInfo.ExecutionFunction = FName("UpdateCount");  // 콜백 함수 이름
	LatentInfo.Linkage = 0;  // 연결 지점(중복되지 않도록 설정)
	LatentInfo.UUID = GetUniqueID(); // 고유 ID로 사용될 값
	UBladeFunctionLibrary::CoolDown(this, TotalTime,0.f, RemaingTime,EBladeCountDownActionInput::Start, CountDownActionOutput, LatentInfo);
}
void UWaveTextWidget::SetWaveMainText(FText* InTextDisplay)
{
	if (InTextDisplay) {
		WaveMainText->SetText(*InTextDisplay);
	}
}
void UWaveTextWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(WaveEntryAnimation);
}

void UWaveTextWidget::UpdateCount()
{

	FText InCountDownText;
	switch (CountDownActionOutput)
	{
	case EBladeCountDownActionOutput::Updated:
	
		InCountDownText = FText::FromString(FString::FromInt(FMath::FloorToInt(FMath::RoundHalfToEven(RemaingTime))));
		CountDownText->SetText(InCountDownText);
		break;
	case EBladeCountDownActionOutput::Completed:
		RemoveFromParent();
		break;
	case EBladeCountDownActionOutput::Cancelled:
		break;
	default:
		break;
	}
}

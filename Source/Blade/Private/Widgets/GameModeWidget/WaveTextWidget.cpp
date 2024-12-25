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
	LatentInfo.CallbackTarget = this;  // �ݹ��� ����� Ÿ�� (���� �ڽ�)
	LatentInfo.ExecutionFunction = FName("UpdateCount");  // �ݹ� �Լ� �̸�
	LatentInfo.Linkage = 0;  // ���� ����(�ߺ����� �ʵ��� ����)
	LatentInfo.UUID = GetUniqueID(); // ���� ID�� ���� ��
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

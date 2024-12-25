// YunMinSeong. All right reserved. 


#include "Widgets/Slot/AbilityIconSlotWidget.h"
#include "Components/TextBlock.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Engine\AssetManager.h"
#include "Components/Image.h"
#include "Kismet\KismetMathLibrary.h"
#include "BladeFunctionLibrary.h"
#include "Components/ProgressBar.h"
//
#include "CustomDebugHelper.h"
void UAbilityIconSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitSlot();
}

void UAbilityIconSlotWidget::OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
	OwningUIComponent->OnAbilityIconSlotUpdate.AddUniqueDynamic(this, &ThisClass::ReciveAbilityIconSlotUpdate);
	OwningUIComponent->OnAbilityCoolDownBegin.AddUniqueDynamic(this, &ThisClass::ReciveAbilityCoolDown);
}

void UAbilityIconSlotWidget::SetSlotImageToSoftTextureIcon(TSoftObjectPtr<UTexture2D> InTexture)
{
	if (InTexture.ToSoftObjectPath().IsValid()) {
		BladeImage->SetBrushFromSoftTexture(InTexture);
		BladeImage->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else {//UnLink
		BladeImage->SetVisibility(ESlateVisibility::Collapsed);
		CoolDownText->SetVisibility(ESlateVisibility::Collapsed);
		CancelCoolDown();
	}
}

void UAbilityIconSlotWidget::ReciveAbilityIconSlotUpdate(FGameplayTag InAbilityInputTag, TSoftObjectPtr<UTexture2D> SoftAbilityIcon)
{
	if (InAbilityInputTag != AbilityInputTag) {
		return;
	}
	SetSlotImageToSoftTextureIcon(SoftAbilityIcon);
}

void UAbilityIconSlotWidget::ReciveAbilityCoolDown(FGameplayTag InAbilityInputTag, float InTotalCoolDownTime, float RemainingCoolDownTime)
{
	if (InAbilityInputTag != AbilityInputTag) {
		return;
	}
	OutCoolDownSec = RemainingCoolDownTime;
	TotalCoolDownTime = InTotalCoolDownTime;
	CoolDownUUID= GetUniqueID();
	//Debug::Print("TotalCoolDownTime: " + FString::SanitizeFloat(TotalCoolDownTime) + " OutCoolDownSec: " + FString::SanitizeFloat(OutCoolDownSec));
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;  // 콜백이 실행될 타겟 (보통 자신)
	LatentInfo.ExecutionFunction = FName("OnCoolDown");  // 콜백 함수 이름
	LatentInfo.Linkage = 0;  // 연결 지점(중복되지 않도록 설정)
	LatentInfo.UUID = CoolDownUUID; // 고유 ID로 사용될 값
	UBladeFunctionLibrary::CoolDown(this, TotalCoolDownTime,0, OutCoolDownSec,EBladeCountDownActionInput::Start, OutBladeCountDownActionOutput, LatentInfo);
	UpdateCoolDownText(OutCoolDownSec);
}

void UAbilityIconSlotWidget::UpdateCoolDownText(float Sec)
{
	//CoolDownText->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (Sec > 0) {
		int32 RoundedSec = FMath::CeilToInt(Sec);   // 올림
		CoolDownText->SetText(FText::FromString(FString::FromInt(RoundedSec)));
		CoolDownText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else if(Sec<=0){
		CoolDownText->SetText(FText::GetEmpty());
		CoolDownProgress->SetPercent(0);
		CoolDownText->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAbilityIconSlotWidget::OnCoolDown()
{
	switch (OutBladeCountDownActionOutput)
	{
	case EBladeCountDownActionOutput::Updated:
		UpdateCoolDownText(OutCoolDownSec);
		CoolDownPercent = UKismetMathLibrary::MapRangeClamped(OutCoolDownSec, TotalCoolDownTime, 0, 1, 0);
		CoolDownProgress->SetPercent(CoolDownPercent);
		break;
	case EBladeCountDownActionOutput::Completed:
		InitSlot();
		break;
	case EBladeCountDownActionOutput::Cancelled:
		InitSlot();
		break;
	default:
		break;
	}
}

void UAbilityIconSlotWidget::CancelCoolDown()
{

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;  // 콜백이 실행될 타겟 (보통 자신)
	LatentInfo.ExecutionFunction = FName("OnCoolDown");  // 콜백 함수 이름
	LatentInfo.Linkage = 1;  // 연결 지점(중복되지 않도록 설정)
	LatentInfo.UUID = CoolDownUUID;// GetUniqueID();  // 고유 ID로 사용될 값
	UBladeFunctionLibrary::CoolDown(this, TotalCoolDownTime, 0.1, OutCoolDownSec, EBladeCountDownActionInput::Cancel, OutBladeCountDownActionOutput, LatentInfo);
}

void UAbilityIconSlotWidget::InitSlot()
{
	UpdateCoolDownText(-1);
}

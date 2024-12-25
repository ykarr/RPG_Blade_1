// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Slot/IconSlotWidget.h"
#include "GameplayTagContainer.h"
#include "BladeTypes/BladeEnumTypes.h"
#include "AbilityIconSlotWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class BLADE_API UAbilityIconSlotWidget : public UIconSlotWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="WidgetTag")
	FGameplayTag AbilityInputTag;
protected:
	virtual void NativeConstruct() override;
	virtual void OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningUIComponent) override;
	virtual void SetSlotImageToSoftTextureIcon(TSoftObjectPtr<UTexture2D> InTexture)override;
	UFUNCTION()
	void ReciveAbilityIconSlotUpdate(FGameplayTag InAbilityInputTag, TSoftObjectPtr<UTexture2D> SoftAbilityIcon);
	UFUNCTION()
	void ReciveAbilityCoolDown(FGameplayTag InAbilityInputTag, float InTotalCoolDownTime, float RemainingCoolDownTime);
	void UpdateCoolDownText(float Sec);
	UFUNCTION()
	void OnCoolDown();
	void CancelCoolDown();
	EBladeCountDownActionOutput OutBladeCountDownActionOutput;
	float OutCoolDownSec;
	float CoolDownPercent;
	float TotalCoolDownTime;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CoolDownText;
	UPROPERTY(meta = (BindWidget))
	UProgressBar* CoolDownProgress;

private:
	void InitSlot();
	int32 CoolDownUUID;
	//FLatentActionInfo LatentInfo;
};

// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "BladeProgressBarWidget.generated.h"

class USizeBox;
class UProgressBar;
/**
 * 
 */
UCLASS()
class BLADE_API UBladeProgressBarWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
public:
	void SetBladeProgressBarPercent(float value);

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> BladeSizeBox;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> BladeProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="UI")
	FLinearColor ProgressPreviewColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="UI")
	FLinearColor ProgressBaseColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="UI")
	FLinearColor ProgressWarningColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="UI")
	FLinearColor ProgressCriticalColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="UI")
	bool ToggleProgressBarColor = true;
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="UI")
	float TestValue = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="UI")
	float BladeSizeBoxWidth = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="UI")
	float BladeSizeBoxHeight = 200;
	virtual void NativeConstruct()override;
	virtual void NativePreConstruct()override;
private:
	void SetOverrideSize(float Width,float Height);
};

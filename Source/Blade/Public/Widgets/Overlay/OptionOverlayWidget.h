// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "BladeTypes/BladeEnumTypes.h"
#include "OptionOverlayWidget.generated.h"

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class BLADE_API UOptionOverlayWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta=(bindWidget))
	UButton* CloseBtn;
	//UPROPERTY(meta=(bindWidget))
	//UButton* DifficultyBtn;
	UPROPERTY(meta=(bindWidget))
	UButton* DifficultyForwardBtn;
	UPROPERTY(meta=(bindWidget))
	UButton* DifficultyBackBtn;
	UPROPERTY(meta=(bindWidget))
	UButton* BackToMainBtn;
	UPROPERTY(meta=(bindWidget))
	UButton* AcceptOptionsBtn;
	UPROPERTY(meta=(bindWidget))
	UTextBlock* DifficultyText;
private:
	UFUNCTION()
	void CloseOptionWidget();
	UFUNCTION()
	void SetDifficultyText();
	UFUNCTION()
	void OnDifficultyBack();
	UFUNCTION()
	void OnDifficultyForward();
	UFUNCTION()
	void OnOptionsAccept();
	
	EBladeGameDifficulty CurrentGameDifficulty;
};

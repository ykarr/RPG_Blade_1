// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "LostScreenOverlayWidget.generated.h"

class UButton;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class BLADE_API ULostScreenOverlayWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(meta = (bindWidget))
	UButton* TryAgainBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* MainMenuBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* QuitBtn;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnimation;

	UFUNCTION()
	void OnRestartGame();
	UFUNCTION()
	void OnMainMenu();
	UFUNCTION()
	void OnQuitGame();
};

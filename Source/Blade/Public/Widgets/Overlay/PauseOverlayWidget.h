// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "PauseOverlayWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class BLADE_API UPauseOverlayWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(meta = (bindWidget))
	UButton* BackToGameBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* MainMenuBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* QuitBtn;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnimation;


private:
	UFUNCTION()
	void OnBackToGame();
	UFUNCTION()
	void OnMainMenu();
	UFUNCTION()
	void OnQuitGame();
};

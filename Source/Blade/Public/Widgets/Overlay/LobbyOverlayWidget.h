// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/BladeWidgetBase.h"
#include "LobbyOverlayWidget.generated.h"

class UButton;
class UOptionOverlayWidget;
/**
 * 
 */
UCLASS()
class BLADE_API ULobbyOverlayWidget : public UBladeWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(meta = (bindWidget))
	UButton* GameStartBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* OptionsBtn;
	UPROPERTY(meta = (bindWidget))
	UButton* QuitBtn;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnimation;

	UPROPERTY(EditDefaultsOnly,Category="Options")
	TSubclassOf<UOptionOverlayWidget> OptionOverlayWidgetClass;
private:
	UFUNCTION()
	void OnGameStart();
	UFUNCTION()
	void OnOptions();
	UFUNCTION()
	void OnQuitGame();
};

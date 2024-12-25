// YunMinSeong. All right reserved. 


#include "Widgets/Overlay/LobbyOverlayWidget.h"
#include "BladeFunctionLibrary.h"
#include "Components/Button.h"
#include "BladeGameInstance.h"
#include "BladeGameplayTags.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Widgets/Overlay/OptionOverlayWidget.h"
void ULobbyOverlayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	GameStartBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::OnGameStart);
	OptionsBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::OnOptions);
	QuitBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::OnQuitGame);
}

void ULobbyOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FadeInAnimation);
	UBladeFunctionLibrary::ToggleInputMode(GetWorld(), EBladeInputMode::UIOnly);
}

void ULobbyOverlayWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULobbyOverlayWidget::OnGameStart()
{
	TSoftObjectPtr<UWorld> SurvivalMap = UBladeFunctionLibrary::GetBladeGameInstance(GetWorld())->GetGameLevelByTag(BladeGameplayTags::GameData_Level_SurvivalGameModeMap);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),SurvivalMap);
	UBladeFunctionLibrary::ToggleInputMode(GetWorld(), EBladeInputMode::GameOnly);
	RemoveFromParent();
}

void ULobbyOverlayWidget::OnOptions()
{
	CreateWidget<UOptionOverlayWidget>(GetWorld(), OptionOverlayWidgetClass)->AddToViewport(10);
}

void ULobbyOverlayWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,true);
}

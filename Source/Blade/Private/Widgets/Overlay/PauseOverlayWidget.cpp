// YunMinSeong. All right reserved. 


#include "Widgets/Overlay/PauseOverlayWidget.h"
#include "Components/Button.h"
#include "BladeFunctionLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet\KismetSystemLibrary.h"
#include "BladeGameplayTags.h"
#include "BladeGameInstance.h"
void UPauseOverlayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BackToGameBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnBackToGame);
	MainMenuBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnMainMenu);
	QuitBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnQuitGame);
}

void UPauseOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UGameplayStatics::SetGamePaused(GetWorld(),true);
	UBladeFunctionLibrary::ToggleInputMode(GetWorld(),EBladeInputMode::UIOnly);
}

void UPauseOverlayWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
}

void UPauseOverlayWidget::OnBackToGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	UBladeFunctionLibrary::ToggleInputMode(GetWorld(), EBladeInputMode::GameOnly);
	RemoveFromParent();
}

void UPauseOverlayWidget::OnMainMenu()
{
	TSoftObjectPtr<UWorld> LobbyMap = UBladeFunctionLibrary::GetBladeGameInstance(GetWorld())->GetGameLevelByTag(BladeGameplayTags::GameData_Level_MainMenuMap);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LobbyMap);
	RemoveFromParent();
}

void UPauseOverlayWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,true);
}

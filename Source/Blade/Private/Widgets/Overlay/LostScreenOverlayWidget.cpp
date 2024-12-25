// YunMinSeong. All right reserved. 


#include "Widgets/Overlay/LostScreenOverlayWidget.h"
#include "Components/Button.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "BladeGameplayTags.h"
//GameInstance
#include "BladeFunctionLibrary.h"
#include "BladeGameInstance.h"
//#include "BladeTypes\BladeEnumTypes.h"
//Debug
#include "CustomDebugHelper.h"
void ULostScreenOverlayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	TryAgainBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::OnRestartGame);
	MainMenuBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::OnMainMenu);
	QuitBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::OnQuitGame);
	
	
}

void ULostScreenOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(FadeInAnimation);
	UBladeFunctionLibrary::ToggleInputMode(GetWorld(), EBladeInputMode::UIOnly);
}

void ULostScreenOverlayWidget::NativeDestruct()
{
	Super::NativeDestruct();
	UBladeFunctionLibrary::ToggleInputMode(GetWorld(), EBladeInputMode::GameOnly);


}

void ULostScreenOverlayWidget::OnRestartGame()
{
	TSoftObjectPtr<UWorld> World= UBladeFunctionLibrary::GetBladeGameInstance(GetWorld())->GetGameLevelByTag(BladeGameplayTags::GameData_Level_SurvivalGameModeMap);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), World);
	RemoveFromParent();
}

void ULostScreenOverlayWidget::OnMainMenu()
{
	TSoftObjectPtr<UWorld> LobbyMap = UBladeFunctionLibrary::GetBladeGameInstance(GetWorld())->GetGameLevelByTag(BladeGameplayTags::GameData_Level_MainMenuMap);
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LobbyMap);
	RemoveFromParent();
}

void ULostScreenOverlayWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}

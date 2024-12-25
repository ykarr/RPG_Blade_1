// YunMinSeong. All right reserved. 


#include "Widgets/Overlay/OptionOverlayWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BladeFunctionLibrary.h"
void UOptionOverlayWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	DifficultyForwardBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnDifficultyForward);
	DifficultyBackBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnDifficultyBack);
	AcceptOptionsBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::OnOptionsAccept);
	BackToMainBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CloseOptionWidget);
	CloseBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CloseOptionWidget);
}

void UOptionOverlayWidget::NativeConstruct()
{//= EBladeGameDifficulty::Easy
	Super::NativeConstruct();
	
	bool bWasLoadSuccess = UBladeFunctionLibrary::TryLoadSaveGameDifficulty(CurrentGameDifficulty);
	if (!bWasLoadSuccess) {
		UBladeFunctionLibrary::SaveCurrentGameDifficulty(EBladeGameDifficulty::Easy);
		CurrentGameDifficulty = EBladeGameDifficulty::Easy;
	}
	SetDifficultyText();
}

void UOptionOverlayWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UOptionOverlayWidget::CloseOptionWidget()
{

	RemoveFromParent();
}

void UOptionOverlayWidget::SetDifficultyText()
{
	switch (CurrentGameDifficulty)
	{
	case EBladeGameDifficulty::Easy:
		DifficultyText->SetText(FText::FromString("Easy"));
		break;
	case EBladeGameDifficulty::Normal:
		DifficultyText->SetText(FText::FromString("Normal"));
		break;
	case EBladeGameDifficulty::Hard:
		DifficultyText->SetText(FText::FromString("Hard"));
		break;
	case EBladeGameDifficulty::VeryHard:
		DifficultyText->SetText(FText::FromString("VeryHard"));
		break;
	default:
		break;
	}
}

void UOptionOverlayWidget::OnDifficultyBack()
{
	switch (CurrentGameDifficulty)
	{
	case EBladeGameDifficulty::Easy:
		CurrentGameDifficulty = EBladeGameDifficulty::Easy;
		break;
	case EBladeGameDifficulty::Normal:
		CurrentGameDifficulty = EBladeGameDifficulty::Easy;
		break;
	case EBladeGameDifficulty::Hard:
		CurrentGameDifficulty = EBladeGameDifficulty::Normal;
		break;
	case EBladeGameDifficulty::VeryHard:
		CurrentGameDifficulty = EBladeGameDifficulty::Hard;
		break;
	default:
		break;
	}
	SetDifficultyText();
}

void UOptionOverlayWidget::OnDifficultyForward()
{
	switch (CurrentGameDifficulty)
	{
	case EBladeGameDifficulty::Easy:
		CurrentGameDifficulty = EBladeGameDifficulty::Normal;
		break;
		
	case EBladeGameDifficulty::Normal:
		CurrentGameDifficulty = EBladeGameDifficulty::Hard;
		break;
	case EBladeGameDifficulty::Hard:
		CurrentGameDifficulty = EBladeGameDifficulty::VeryHard;
		break;
	case EBladeGameDifficulty::VeryHard:
		CurrentGameDifficulty = EBladeGameDifficulty::VeryHard;
		break;
	default:
		break;
	}
	SetDifficultyText();
}

void UOptionOverlayWidget::OnOptionsAccept()
{
	UBladeFunctionLibrary::SaveCurrentGameDifficulty(CurrentGameDifficulty);
	RemoveFromParent();
}

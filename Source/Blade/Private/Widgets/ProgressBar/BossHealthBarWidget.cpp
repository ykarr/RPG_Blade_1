// YunMinSeong. All right reserved. 


#include "Widgets/ProgressBar/BossHealthBarWidget.h"
#include "Widgets/ProgressBar/BladeProgressBarWidget.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Components\TextBlock.h"

#include "CustomDebugHelper.h"
void UBossHealthBarWidget::SetBossName(const FText& InBossName)
{
	BossName->SetText(InBossName);
}
void UBossHealthBarWidget::OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningUIComponent)
{
	
	Super::OnOwningEnemyUIComponentInitialized(OwningUIComponent);
	OwningUIComponent->OnCurrentHealthChanged.AddDynamic(this, &ThisClass::CurrentHealthChanged);
	OwningUIComponent->RegisterEnemyWidget(this);
	//EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	
}


void UBossHealthBarWidget::CurrentHealthChanged(float value)
{
	EnemyHealthBar->SetBladeProgressBarPercent(value);
	if (value <= 0.f ) {
		//처음 시작할때 값이 변해서인지 HealthBar가 바로 안사라짐.
		EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		EnemyHealthBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	//5초동안 값이 변하지 않으면 숨김.
	//GetWorld()->GetTimerManager().ClearTimer(InvisibleTimerHandle);
	//GetWorld()->GetTimerManager().SetTimer(InvisibleTimerHandle, FTimerDelegate::CreateLambda([this]() {
	//	EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	//	}), 5.f, false);
}

void UBossHealthBarWidget::NativeDestruct()
{
	Super::NativeDestruct();
	Debug::Print("EnemyHealthBar Remove");
}

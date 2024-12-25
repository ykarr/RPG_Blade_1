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
		//ó�� �����Ҷ� ���� ���ؼ����� HealthBar�� �ٷ� �Ȼ����.
		EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		EnemyHealthBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	//5�ʵ��� ���� ������ ������ ����.
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

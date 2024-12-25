// YunMinSeong. All right reserved. 


#include "Widgets/ProgressBar/EnemyHealthBarBase.h"
#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/ProgressBar/BladeProgressBarWidget.h"
void UEnemyHealthBarBase::OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningUIComponent)
{
	Super::OnOwningEnemyUIComponentInitialized( OwningUIComponent);
	OwningUIComponent->OnCurrentHealthChanged.AddDynamic(this,&ThisClass::CurrentHealthChanged);

	EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
}

void UEnemyHealthBarBase::CurrentHealthChanged(float value)
{
	EnemyHealthBar->SetBladeProgressBarPercent(value);
	if (value == 0.f|| FirstHide) {
		//ó�� �����Ҷ� ���� ���ؼ����� HealthBar�� �ٷ� �Ȼ����.
		FirstHide = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		EnemyHealthBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	//5�ʵ��� ���� ������ ������ ����.
	GetWorld()->GetTimerManager().ClearTimer(InvisibleTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(InvisibleTimerHandle,FTimerDelegate::CreateLambda([this](){
		EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}),5.f,false);
}

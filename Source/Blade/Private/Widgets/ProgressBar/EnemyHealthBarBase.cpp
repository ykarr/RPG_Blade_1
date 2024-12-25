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
		//처음 시작할때 값이 변해서인지 HealthBar가 바로 안사라짐.
		FirstHide = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		EnemyHealthBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	//5초동안 값이 변하지 않으면 숨김.
	GetWorld()->GetTimerManager().ClearTimer(InvisibleTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(InvisibleTimerHandle,FTimerDelegate::CreateLambda([this](){
		EnemyHealthBar->SetVisibility(ESlateVisibility::Collapsed);
	}),5.f,false);
}

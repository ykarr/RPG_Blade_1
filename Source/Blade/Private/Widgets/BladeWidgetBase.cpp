// YunMinSeong. All right reserved. 


#include "Widgets/BladeWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/PlayerUIComponent.h"
#include "Components/UI/EnemyUIComponent.h"
void UBladeWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn())) {
		
		if (UPlayerUIComponent* PlayerUIComponent = PawnUIInterface->GetPlayerUIComponent()) {
			OnOwningPlayerUIComponentInitialized(PlayerUIComponent);
		}
	}
}



void UBladeWidgetBase::InitEnemyCreatedWidget(AActor* OwningEnemyActor)
{
	if (IPawnUIInterface* PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor)) {
		UEnemyUIComponent*EnemyUIComponent = PawnUIInterface->GetEnemyUIComponent();
		checkf(EnemyUIComponent,TEXT("EnemyUIComponent Error") );
		OnOwningEnemyUIComponentInitialized(EnemyUIComponent);
	}
}
void UBladeWidgetBase::OnOwningPlayerUIComponentInitialized(UPlayerUIComponent* OwningUIComponent)
{
}

void UBladeWidgetBase::OnOwningEnemyUIComponentInitialized(UEnemyUIComponent* OwningUIComponent)
{
}
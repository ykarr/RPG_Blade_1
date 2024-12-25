// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/EnemyDrawBossBarAbility.h"
#include "Widgets/ProgressBar/BossHealthBarWidget.h"
#include "Characters\BladeEnemyCharacterBase.h"
void UEnemyDrawBossBarAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility( Handle, ActorInfo, ActivationInfo,TriggerEventData);
	
	BossHealthBarWidget=CreateWidget<UBossHealthBarWidget>(GetWorld(), BossHealthBarWidgetClass);
	BossHealthBarWidget->SetBossName(BossName);
	BossHealthBarWidget->AddToViewport(0);
	BossHealthBarWidget->InitEnemyCreatedWidget(GetEnemyCharacterFromActorInfo());
	//bReplicateEndAbility 네트워크 상에서 복제되어야 하는지.
	//bWasCancelled 정상적으로 종료된것인지 취소된것인지. false가 정상.
	EndAbility(Handle, ActorInfo, ActivationInfo,false,false);
}

void UEnemyDrawBossBarAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo,bReplicateEndAbility,bWasCancelled);

}

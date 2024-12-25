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
	//bReplicateEndAbility ��Ʈ��ũ �󿡼� �����Ǿ�� �ϴ���.
	//bWasCancelled ���������� ����Ȱ����� ��ҵȰ�����. false�� ����.
	EndAbility(Handle, ActorInfo, ActivationInfo,false,false);
}

void UEnemyDrawBossBarAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo,bReplicateEndAbility,bWasCancelled);

}

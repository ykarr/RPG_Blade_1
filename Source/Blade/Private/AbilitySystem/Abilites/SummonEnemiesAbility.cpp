// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/SummonEnemiesAbility.h"
#include "Abilities\Tasks\AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"
#include "Characters\BladeEnemyCharacterBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "BladeGameplayTags.h"
//GameMode
#include "GameModes/BladeSurvialGameMode.h"
void USummonEnemiesAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,FName(""), PlayToMontage);
	PlayMontageAndWait->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageBlendOut);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &ThisClass::OnMontageCompleted);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageInterrupted);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &ThisClass::OnMontageCancelled);
	PlayMontageAndWait->ReadyForActivation();
	FVector SpawnLoc=GetEnemyCharacterFromActorInfo()->GetActorLocation();
	UAbilityTask_WaitSpawnEnemies* WaitSpawnEnemies = UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(this,BladeGameplayTags::Enemy_Event_SummonEnemies, SoftEnemyClassToSpawn, SpawnLoc, NumToSpawn, RandomSpawnRadius);
	WaitSpawnEnemies->OnSpawnFinished.AddDynamic(this,&ThisClass::OnSpawnFinished);
	WaitSpawnEnemies->ReadyForActivation();
}

void USummonEnemiesAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void USummonEnemiesAbility::OnMontageCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, false, false);
}

void USummonEnemiesAbility::OnMontageBlendOut()
{
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, false, false);
}

void USummonEnemiesAbility::OnMontageInterrupted()
{
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, false, false);
}

void USummonEnemiesAbility::OnMontageCancelled()
{
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, false, false);
}

void USummonEnemiesAbility::OnSpawnFinished(const TArray<ABladeEnemyCharacterBase*>& SpawnedEnemy)
{
	UBlackboardComponent* BlackboardComponent=UAIBlueprintHelperLibrary::GetBlackboard(GetOwningActorFromActorInfo());
	BlackboardComponent->SetValueAsBool(FName("HasSpawnedEnemies"),true);
	
	if (ABladeSurvialGameMode* SurvialGameMode = Cast<ABladeSurvialGameMode>(GetWorld()->GetAuthGameMode())) {
		SurvialGameMode->RegisterSpawnEnemies(SpawnedEnemy);
	}
}

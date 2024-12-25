// YunMinSeong. All right reserved. 


#include "AbilitySystem/AbilityTasks/AbilityTask_WaitSpawnEnemies.h"
#include "Characters/BladeEnemyCharacterBase.h"
#include "Engine/AssetManager.h"
#include "AbilitySystemComponent.h"
#include "NavigationSystem.h"
//Debug
#include "CustomDebugHelper.h"
UAbilityTask_WaitSpawnEnemies* UAbilityTask_WaitSpawnEnemies::WaitSpawnEnemies(UGameplayAbility* OwningAbility, FGameplayTag EventTag, TSoftClassPtr<ABladeEnemyCharacterBase> SoftEnemyClassToSpawn, const FVector& SpawnOrigin,  int32 NumToSpawn, float RandomSpawnRadius)
{
	UAbilityTask_WaitSpawnEnemies* Node = NewAbilityTask<UAbilityTask_WaitSpawnEnemies>(OwningAbility);
	Node->CachedEventTag = EventTag;
	Node->CachedSoftEnemyClassToSpawn = SoftEnemyClassToSpawn;
	Node->CachedSpawnOrigin = SpawnOrigin;
	Node->CachedNumToSpawn = NumToSpawn;
	Node->CachedRandomSpawnRadius = RandomSpawnRadius;
	return Node;
}

void UAbilityTask_WaitSpawnEnemies::Activate()
{
	Super::Activate();
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	DelegateHandle=Delegate.AddUObject(this, &ThisClass::OnGameplayEventRecived);
}

void UAbilityTask_WaitSpawnEnemies::OnDestroy(bool AbilityEnded)
{
	FGameplayEventMulticastDelegate& Delegate = AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(CachedEventTag);
	Delegate.Remove(DelegateHandle);
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_WaitSpawnEnemies::OnGameplayEventRecived(const FGameplayEventData* InPayload)
{
	//check�� ����������, ensure�� ������ ��忡���� ������ ��� ����ǰ� �Ѵٴ� ������ ���̰� ����.
	if (ensure(!CachedSoftEnemyClassToSpawn.IsNull())) {
		Debug::Print("WaitSpawnEnemies");
		//CreateUObject: C++ ��� �Լ��� ��������Ʈ�� ���.	CreateUFunction: UFunction�̳� �������Ʈ �Լ��� ��������Ʈ�� ���.
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
			CachedSoftEnemyClassToSpawn.ToSoftObjectPath(), 
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnEnemyClassLoaded)
		);
	}
	else {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			DidNotSpawn.Broadcast(TArray<ABladeEnemyCharacterBase*>());
		}
		EndTask();
	}
}


void UAbilityTask_WaitSpawnEnemies::OnEnemyClassLoaded()
{
	UClass* LoadedClass = CachedSoftEnemyClassToSpawn.Get();
	UWorld* World= GetWorld();
	if (!LoadedClass || !World) {
		if (ShouldBroadcastAbilityTaskDelegates()) {
			DidNotSpawn.Broadcast(TArray<ABladeEnemyCharacterBase*>());
		}
		EndTask();
		return;
	}
	TArray<ABladeEnemyCharacterBase*> SpawnEnemies;

	FActorSpawnParameters SpawnParms;
	SpawnParms.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	for (int32 i = 0; i < CachedNumToSpawn; i++) {
		FVector RandomLocation;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(this,CachedSpawnOrigin, RandomLocation, CachedRandomSpawnRadius);
		//���� ������ �ʵ��� ������ ����.
		RandomLocation += FVector(0, 0, 200.f);
		FRotator SpawnFacingRot = AbilitySystemComponent->GetAvatarActor()->GetActorRotation();
		ABladeEnemyCharacterBase* SpawnEnemy=World->SpawnActor<ABladeEnemyCharacterBase>(LoadedClass, RandomLocation, SpawnFacingRot, SpawnParms);
		if (SpawnEnemy) {
			SpawnEnemies.Add(SpawnEnemy);
		}
	}
	if (ShouldBroadcastAbilityTaskDelegates()) {
		if (!SpawnEnemies.IsEmpty()) {
			OnSpawnFinished.Broadcast(SpawnEnemies);
		}
		else {
			DidNotSpawn.Broadcast(TArray<ABladeEnemyCharacterBase*>());
		}
	}
	EndTask();
}
// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/Pickup/EnemyItemDropAbility.h"
#include "Characters\BladeEnemyCharacterBase.h"
#include "NavigationSystem.h"
#include "Kismet\KismetMathLibrary.h"
#include "Engine\AssetManager.h"
void UEnemyItemDropAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	FVector LocVec = GetEnemyCharacterFromActorInfo()->GetActorLocation();
	FVector UpVec = GetEnemyCharacterFromActorInfo()->GetActorUpVector();
	FVector DownVec = (UpVec * -200) + LocVec;
	FHitResult CheckSpawnFloor;
	
	FVector SpawnOriginLoc;
	FVector SpawnRandomLoc;
	FCollisionObjectQueryParams CollisionObjectQueryParams;
	CollisionObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	FCollisionQueryParams CollisionQueryParams;
	//CollisionQueryParams.
	if (GetWorld()->LineTraceSingleByObjectType(CheckSpawnFloor, LocVec, DownVec, CollisionObjectQueryParams, CollisionQueryParams)) {
		SpawnOriginLoc =CheckSpawnFloor.ImpactPoint;
	}
	
	UNavigationSystemV1* NavigationSystem=FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FNavLocation NavLoc;
	NavigationSystem->GetRandomPointInNavigableRadius(SpawnOriginLoc,200.f, NavLoc);
	SpawnRandomLoc=NavLoc.Location;
	
	bool Randbool= UKismetMathLibrary::RandomBoolWithWeight(0.5);
	if (Randbool) {
		PotionClass= ManyPotionClass[0];
	}
	else {
		PotionClass= ManyPotionClass[1];
	}
	bool bSouldSpawnPotion= UKismetMathLibrary::RandomBoolWithWeight(InScalableFloat.GetValueAtLevel(GetAbilityLevel()));
	if (bSouldSpawnPotion) {
		UAssetManager::GetStreamableManager().RequestAsyncLoad(PotionClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this, SpawnRandomLoc]() {
			TSubclassOf<AActor> SpawnClass = PotionClass.Get();
			if (!SpawnClass) {
				EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), false, false);
			}
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			GetWorld()->SpawnActor<AActor>(SpawnClass, SpawnRandomLoc,FRotator::ZeroRotator, SpawnParameters);
		}));
	}
}

void UEnemyItemDropAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

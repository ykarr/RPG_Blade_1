// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/Attacks/PlayerProjectileAttackAbility.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "BladeGameplayTags.h"
#include "Items\Projectile\BladeProjectileBase.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "CustomDebugHelper.h"
void UPlayerProjectileAttackAbility::StartProjectileSpawn(FGameplayEventData Data)
{
	checkf(ProjectileObjectClass, TEXT("Pease FIll ProjectileObject"));
	//AActor* SpawnActor = Cast<AActor>(GetEnemyCharacterFromActorInfo());
	AActor* SpawnActor = GetAvatarActorFromActorInfo();
	FActorSpawnParameters SpawnParm;
	SpawnParm.Owner = GetAvatarActorFromActorInfo();
	SpawnParm.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	FVector SpawnLoc = SpawnActor->GetActorLocation() + SpawnActor->GetActorForwardVector() * 200;
	FRotator SpawnRot = SpawnActor->GetActorRotation();

	if (bSpawnInSocket) {
		if (GetOwningComponentFromActorInfo()->DoesSocketExist(SpawnInSocket)) {
			SpawnLoc = GetOwningComponentFromActorInfo()->GetSocketLocation(SpawnInSocket);
			SpawnRot = SpawnActor->GetActorForwardVector().ToOrientationRotator();
		}
	}
	//

	//오브젝트를 스폰할 때 충돌이 발생해도 해당 위치에 오브젝트를 스폰할 수 있습니다.
	SpawnParm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ABladeProjectileBase* projectile = Cast<ABladeProjectileBase>(GetWorld()->SpawnActor<AActor>(ProjectileObjectClass, SpawnLoc, SpawnRot, SpawnParm));
	projectile->ProjectileDamageEffectSpecHandle = MakePlayerDamageEffectSpecHandle(DealingEffectClass, GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquipWeaponDamageAtLevel(GetAbilityLevel()), AttackTypeTag, 3);
}

void UPlayerProjectileAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, BladeGameplayTags::Shared_Event_SpawnProjectile);
	WaitGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::StartProjectileSpawn);
	WaitGameplayEvent->ReadyForActivation();
}

void UPlayerProjectileAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

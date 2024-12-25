// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/SpawnWeaponAbility.h"
#include "Items/Weapons/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/Combat/BaseCombatComponent.h"

//Debug
#include "CustomDebugHelper.h"
USpawnWeaponAbility::USpawnWeaponAbility()
{
	AbilityActivationPolicy = EBladeAbilityActivationPolicy::OnGiven;
}

void USpawnWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//능력이 활성화되면 무기를 생성.
	FActorSpawnParameters SpawnParm;
	SpawnParm.Owner= GetAvatarActorFromActorInfo();
	SpawnParm.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	//오브젝트를 스폰할 때 충돌이 발생해도 해당 위치에 오브젝트를 스폰할 수 있습니다.
	SpawnParm.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClassToSpawn, SpawnParm);
	if (SpawnWeapon) {
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld,true);
		SpawnWeapon->AttachToComponent(GetOwningComponentFromActorInfo(), AttachmentRules, SocketName);
		/*if (WeaponTagToRegister.IsValid()) {
			GetPawnCombatComponentFromActoInfo()->RegisterSpawnWeapon(WeaponTagToRegister, SpawnWeapon, RegisterAsEquippedWeapon);
		}
		else {
			Debug::Print(TEXT("SpawnWeapon Tag is Not Valid"));
		}*/
		GetPawnCombatComponentFromActoInfo()->RegisterSpawnWeapon(WeaponTagToRegister, SpawnWeapon, RegisterAsEquippedWeapon);
		SpawnWeapon ->GetWeaponWeaponMesh()->SetScalarParameterValueOnMaterials(FName("DissolveAmount"), 1);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void USpawnWeaponAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{//bReplicateEndAbility (옵션): 이 능력이 네트워크 상에서 종료되었음을 다른 클라이언트에 복제할지 여부. 기본값은 true입니다.
	//bWasCancelled (옵션): 능력이 취소되었는지 여부를 나타내는 플래그. 기본값은 false입니다.
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

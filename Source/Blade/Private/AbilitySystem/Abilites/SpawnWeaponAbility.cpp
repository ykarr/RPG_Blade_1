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
	//�ɷ��� Ȱ��ȭ�Ǹ� ���⸦ ����.
	FActorSpawnParameters SpawnParm;
	SpawnParm.Owner= GetAvatarActorFromActorInfo();
	SpawnParm.Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	//������Ʈ�� ������ �� �浹�� �߻��ص� �ش� ��ġ�� ������Ʈ�� ������ �� �ֽ��ϴ�.
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
{//bReplicateEndAbility (�ɼ�): �� �ɷ��� ��Ʈ��ũ �󿡼� ����Ǿ����� �ٸ� Ŭ���̾�Ʈ�� �������� ����. �⺻���� true�Դϴ�.
	//bWasCancelled (�ɼ�): �ɷ��� ��ҵǾ����� ���θ� ��Ÿ���� �÷���. �⺻���� false�Դϴ�.
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

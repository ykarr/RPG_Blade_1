// YunMinSeong. All right reserved. 


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items\Weapons\PlayerWeapon.h"
#include "BladeGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "CustomDebugHelper.h"
APlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{//�������Ʈ������ �� �� ������ Cast�� C++���� �ϴ°��� �� ������ �޸� ȿ����
	return Cast<APlayerWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

APlayerWeapon* UPlayerCombatComponent::GetPlayerCurrentEquipWeapon() const
{
	return Cast<APlayerWeapon>(GetCharacterCurrentEquippedWeapon());
}

float UPlayerCombatComponent::GetPlayerCurrentEquipWeaponDamageAtLevel(float InLevel) const
{
	return GetPlayerCurrentEquipWeapon()->PlayerWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UPlayerCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);
	if (OverlappedActors.Contains(HitActor)) {
		//���ݴ� �� ���� ��� ���Ϳ��� ���ظ� �� �� �ֵ���.
		return;
	}
	Debug::Print("AddOnHit");
	OverlappedActors.AddUnique(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BladeGameplayTags::Shared_Event_MeleeHit, Data);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BladeGameplayTags::Player_Event_HitPause, FGameplayEventData());
}

void UPlayerCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
	Super::OnWeaponPulledFromTargetActor(InteractedActor);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), BladeGameplayTags::Player_Event_HitPause, FGameplayEventData());
}


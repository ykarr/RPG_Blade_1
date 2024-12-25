// YunMinSeong. All right reserved. 


#include "Components/Combat/BaseCombatComponent.h"
#include "Components\BoxComponent.h"
#include "Items/Weapons/WeaponBase.h"
//Debug
#include "CustomDebugHelper.h"
void UBaseCombatComponent::RegisterSpawnWeapon(FGameplayTag InWeaponTagToRegister, AWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister), TEXT("A Named %s has already been added as carried Weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	/*Dynamic Multicast Delegate�� �ƴ� �׳� Delegate�̱� ������ UFUNCTION�� �ʿ����� ����.*/
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	//Emplace Vector���� Push_Back()�̶� ���.  InWeaponTagToRegisterŰ InWeaponToRegister value�� �ǰڴ�.
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);
	if (bRegisterAsEquippedWeapon) {//Equipped Weapon�� ���°�� ���� ����� ���⸦ Equipped Weapon���� ����.
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
}

AWeaponBase* UBaseCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet)) {
		//Find�Լ��� Ű�� �ش��ϴ� ���� ã�� �ش簪�� ���� �����͸� ��ȯ.
		//����Ű�� ������ ��ü�� ������ �� ������, �ش� �����Ͱ� ����Ű�� ��� ��ü�� ������ �� �ִ�
		if (AWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet)) {
			//*FoundWeapon�� ���� ��ü�� ���ٰ���.
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AWeaponBase* UBaseCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid()) return nullptr;
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UBaseCombatComponent::ToggleWeaponCollisionEnable(bool bShouldEnable, EToggleDamageType ToogleDamageType)
{
	if (ToogleDamageType == EToggleDamageType::CurrentWeapon) {
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	else {
		ToggleCurrentBodyCollision(bShouldEnable,ToogleDamageType);
	}
	
}

void UBaseCombatComponent::OnHitTargetActor(AActor* HitActor)
{
}

void UBaseCombatComponent::OnWeaponPulledFromTargetActor(AActor* InteractedActor)
{
}

void UBaseCombatComponent::ToggleCurrentEquippedWeaponCollision(bool bShouldEnable)
{
	AWeaponBase* WeaponToToggle = GetCharacterCurrentEquippedWeapon();
	check(WeaponToToggle);
	if (bShouldEnable) {
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		//Debug::Print(WeaponToToggle->GetName() + "Collision Enable", FColor::Green);
	}
	else {
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Debug::Print(WeaponToToggle->GetName() + "Collision Disable", FColor::Red);
		OverlappedActors.Empty();
	}
}

void UBaseCombatComponent::ToggleCurrentBodyCollision(bool bShouldEnable, EToggleDamageType ToogleDamageType)
{

}



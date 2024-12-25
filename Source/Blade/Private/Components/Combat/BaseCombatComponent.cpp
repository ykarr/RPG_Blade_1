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

	/*Dynamic Multicast Delegate가 아닌 그냥 Delegate이기 때문에 UFUNCTION이 필요하지 않음.*/
	InWeaponToRegister->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActor);
	InWeaponToRegister->OnWeaponPulledFromTarget.BindUObject(this, &ThisClass::OnWeaponPulledFromTargetActor);

	//Emplace Vector에서 Push_Back()이랑 비슷.  InWeaponTagToRegister키 InWeaponToRegister value쯤 되겠다.
	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);
	if (bRegisterAsEquippedWeapon) {//Equipped Weapon이 없는경우 현재 등록한 무기를 Equipped Weapon으로 설정.
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
}

AWeaponBase* UBaseCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet)) {
		//Find함수는 키에 해당하는 값을 찾아 해당값에 대한 포인터를 반환.
		//가리키는 포인터 자체를 변경할 수 없지만, 해당 포인터가 가리키는 대상 객체는 변경할 수 있다
		if (AWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet)) {
			//*FoundWeapon로 무기 객체에 접근가능.
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



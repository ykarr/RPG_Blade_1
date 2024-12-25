// YunMinSeong. All right reserved. 


#include "Components/Combat/PlayerCombatComponent.h"
#include "Items\Weapons\PlayerWeapon.h"
#include "BladeGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "CustomDebugHelper.h"
APlayerWeapon* UPlayerCombatComponent::GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{//블루프린트에서도 할 수 있지만 Cast를 C++에서 하는것이 더 빠르고 메모리 효율적
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
		//공격당 한 번만 대상 액터에게 피해를 줄 수 있도록.
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


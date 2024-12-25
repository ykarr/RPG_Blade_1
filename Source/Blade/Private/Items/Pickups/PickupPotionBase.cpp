// YunMinSeong. All right reserved. 


#include "Items/Pickups/PickupPotionBase.h"
#include "Characters\BladePlayerCharacter.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"
#include "BladeGameplayTags.h"
void APickupPotionBase::Consume(UBladeAbilitySystemComponent* ASC, int32 ApplyLevel)
{
	check(PotionEffectClass);
	
	ASC->ApplyGameplayEffectToSelf(PotionEffectClass->GetDefaultObject<UGameplayEffect>(), ApplyLevel,ASC->MakeEffectContext());
	BP_OnPotionConsumed();
}
void APickupPotionBase::OnPickupCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABladePlayerCharacter* OverlappedPlayerCharacter=Cast<ABladePlayerCharacter>(OtherActor)) {
		OverlappedPlayerCharacter->GetBladeAbilitySystemComponent()->TryActivateAbilityByTag(BladeGameplayTags::Player_Ability_PickUp_Potions);

	}

}

// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/Combat/BaseCombatComponent.h"
#include "GameplayTagContainer.h"
#include "PlayerCombatComponent.generated.h"

class APlayerWeapon;
/**
 * 
 */
UCLASS()
class BLADE_API UPlayerCombatComponent : public UBaseCombatComponent
{
	GENERATED_BODY()
public:

	UFUNCTION(Blueprintcallable, Category = "Blade|Combat")
	APlayerWeapon* GetPlayerCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(Blueprintcallable, Category = "Blade|Combat")
	APlayerWeapon* GetPlayerCurrentEquipWeapon() const;
	
	UFUNCTION(Blueprintcallable, Category = "Blade|Combat")
	float GetPlayerCurrentEquipWeaponDamageAtLevel(float InLevel) const;

	virtual void OnHitTargetActor(AActor* HitActor)override;
	virtual void OnWeaponPulledFromTargetActor(AActor* InteractedActor)override;
};

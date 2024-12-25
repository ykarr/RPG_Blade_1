// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "BladeTypes/BladeStructTypes.h"
#include "PlayerUnequipAbility.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UPlayerUnequipAbility : public UBladePlayerGameplayAbility
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
	void DetachWeaponToPlayerHand();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FGameplayTag InWeaponTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName SocketName;

	void UnLinkWeaponData();
};

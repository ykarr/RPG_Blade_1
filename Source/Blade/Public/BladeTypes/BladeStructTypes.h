// YunMinSeong. All right reserved. 

#pragma once
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "BladeStructTypes.generated.h"

class UBladePlayerLinkedAnimLayer;
class UBladePlayerGameplayAbility;
class UInputMappingContext;
class UTexture2D;
struct FScalableFloat;

USTRUCT(BlueprintType)
struct FBladePlayerAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBladePlayerGameplayAbility> AbilityToGrant;//어빌리티 부여

	bool IsValid() const;
}; 
USTRUCT(BlueprintType)
struct FBladePlayerSpecialAbilitySet :public FBladePlayerAbilitySet {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> AbilityIconMaterial;
	//TSoftObjectPtr<UMaterialInterface> AbilityIconMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta = (Categories="Player.CoolDown"))
	FGameplayTag AbilityCoolDownTag;
};
USTRUCT(BlueprintType)
struct FBladePlayerWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, blueprintReadonly)
	TSubclassOf<UBladePlayerLinkedAnimLayer> WeaponAnimLayerToLink;
	
	UPROPERTY(EditDefaultsOnly, blueprintReadonly)
	UInputMappingContext* WeaponInputMappingContext;

	UPROPERTY(EditDefaultsOnly, blueprintReadonly,meta=(TitleProperty="InputTag"))
	TArray<FBladePlayerAbilitySet> DefaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, blueprintReadonly,meta=(TitleProperty="InputTag"))
	TArray<FBladePlayerSpecialAbilitySet> SpecialWeaponAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;
};
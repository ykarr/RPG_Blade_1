// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "GameplayTagContainer.h"
#include "PlayerUIComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdateDelegate, FGameplayTag, InAbilityInputTag, TSoftObjectPtr<UTexture2D>, SoftAbilityIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCoolDownBeginDelegate, FGameplayTag, InAbilityInputTag, float, TotalCoolDownTime, float , RemainingCoolDownTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPotionInteractedDelegate , bool, bShouldDisplayKeyGuide);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdateDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UMaterialInterface>, SoftAbilityIconMaterial);

/**
 * 
 */
UCLASS()
class BLADE_API UPlayerUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnCurrentRageChanged;
	UPROPERTY(BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChanged;
	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnAbilityIconSlotUpdateDelegate OnAbilityIconSlotUpdate;
	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnAbilityCoolDownBeginDelegate OnAbilityCoolDownBegin;
	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnPotionInteractedDelegate OnPotionInteractedDelegate;
};

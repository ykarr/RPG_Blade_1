// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_StartUpDataBase.generated.h"

class UBladeAbilitySystemComponent;
class UBladeGameplayAbility;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class BLADE_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()
public: 
	virtual void GiveToAbilitySystemComponent(UBladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
protected:
	//부여받은 직후 활성화해야 하는 능력.
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UBladeGameplayAbility>> ActivateOnGivenAbilities;
	//특정 조건에서 발동되는 적중 반응이나 사망과 같은 능력.
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UBladeGameplayAbility>> ReactiveAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartupGameplayEffect;

	void GrantAbilities(const TArray<TSubclassOf<UBladeGameplayAbility>>& InAbilitiesToGive, UBladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};

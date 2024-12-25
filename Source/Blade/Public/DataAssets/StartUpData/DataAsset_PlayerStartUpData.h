// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "BladeTypes/BladeStructTypes.h"
#include "DataAsset_PlayerStartUpData.generated.h"


/**
 * 
 */
UCLASS()
class BLADE_API UDataAsset_PlayerStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()
public:
	virtual void GiveToAbilitySystemComponent(UBladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1)override;
private:
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	TArray<FBladePlayerAbilitySet> PlayerStartUpAbilitySets;
	//UPROPERTY(EditDefaultsOnly, Category = "StartUpData", meta = (TitleProperty = "InputTag"))
	//TArray<UBladePlayerGameplayAbility> BladePlayerGameplayAbility;
};

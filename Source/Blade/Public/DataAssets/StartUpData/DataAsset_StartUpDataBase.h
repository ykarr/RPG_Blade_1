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
	//�ο����� ���� Ȱ��ȭ�ؾ� �ϴ� �ɷ�.
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UBladeGameplayAbility>> ActivateOnGivenAbilities;
	//Ư�� ���ǿ��� �ߵ��Ǵ� ���� �����̳� ����� ���� �ɷ�.
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UBladeGameplayAbility>> ReactiveAbilities;
	UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
	TArray<TSubclassOf<UGameplayEffect>> StartupGameplayEffect;

	void GrantAbilities(const TArray<TSubclassOf<UBladeGameplayAbility>>& InAbilitiesToGive, UBladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel = 1);
};

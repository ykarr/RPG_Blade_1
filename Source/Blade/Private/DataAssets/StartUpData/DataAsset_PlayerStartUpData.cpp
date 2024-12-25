// YunMinSeong. All right reserved. 


#include "DataAssets/StartUpData/DataAsset_PlayerStartUpData.h"
//#include "AbilitySystem/Abilites/BladeGameplayAbility.h"
#include "AbilitySystem\Abilites\BladePlayerGameplayAbility.h"
#include "AbilitySystem/BladeAbilitySystemComponent.h"
//#include "BladeTypes/BladeStructTypes.h"
void UDataAsset_PlayerStartUpData::GiveToAbilitySystemComponent(UBladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	for (const FBladePlayerAbilitySet& AbilitySet:PlayerStartUpAbilitySets) {
		if (!AbilitySet.IsValid()) continue;//AbilitySet�� ��ȿ���� �������.(FGameplayTag,UBladeGameplayAbility)
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		//DynamicAbilityTags�� �ɷ¿� �������� �ο��ǰų� ���ŵǴ� �±׵��� �����ϴ� ������ �մϴ�. �̸� ���� �ɷ��� ���³� ȿ���� �������� ������ �� �ֽ��ϴ�.
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		InASCToGive->GiveAbility(AbilitySpec);
	}
}


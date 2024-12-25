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
		if (!AbilitySet.IsValid()) continue;//AbilitySet이 유효하지 않은경우.(FGameplayTag,UBladeGameplayAbility)
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		//DynamicAbilityTags는 능력에 동적으로 부여되거나 제거되는 태그들을 관리하는 역할을 합니다. 이를 통해 능력의 상태나 효과를 동적으로 제어할 수 있습니다.
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);
		InASCToGive->GiveAbility(AbilitySpec);
	}
}


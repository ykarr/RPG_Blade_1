// YunMinSeong. All right reserved. 


#include "DataAssets/StartUpData/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"
#include "AbilitySystem\Abilites\BladeEnemyGameplayAbility.h"
void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UBladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InASCToGive, ApplyLevel);
	if (!EnemyCombatAbility.IsEmpty()) {
		for (class TSubclassOf<UBladeEnemyGameplayAbility>& AbilityClass:EnemyCombatAbility) {
			if (!AbilityClass) continue;

			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.Level = ApplyLevel;
			AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();

			InASCToGive->GiveAbility(AbilitySpec);
		}
	}
}

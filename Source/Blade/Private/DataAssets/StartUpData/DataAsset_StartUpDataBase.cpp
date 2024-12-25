// YunMinSeong. All right reserved. 


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/BladeAbilitySystemComponent.h"
#include "AbilitySystem/Abilites/BladeGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UBladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);
	GrantAbilities(ActivateOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	if (!StartupGameplayEffect.IsEmpty()) {
		for (const TSubclassOf<UGameplayEffect>& EfectClass : StartupGameplayEffect) {
			if (!EfectClass) continue;
			UGameplayEffect* EfectCDO = EfectClass->GetDefaultObject<UGameplayEffect>();
			InASCToGive->ApplyGameplayEffectToSelf(
				EfectCDO, 
				ApplyLevel,
				InASCToGive->MakeEffectContext()
			);
		}
	}
	

}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UBladeGameplayAbility>>& InAbilitiesToGive, UBladeAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty())return;
	//참조자를 사용해서 복사하지 않고 참조하여 사용.
	for (const TSubclassOf<UBladeGameplayAbility>& Ability : InAbilitiesToGive) {
		if (!Ability) continue;
		FGameplayAbilitySpec AbilitySpec(Ability);
		//능력의 출처가되는 객체. 능력이 어디에서 유래했는지, 어떤 객체에 의해 능력이 제공되었는지에 대한 정보.
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		InASCToGive->GiveAbility(AbilitySpec);
	}
}

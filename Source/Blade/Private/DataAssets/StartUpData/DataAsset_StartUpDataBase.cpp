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
	//�����ڸ� ����ؼ� �������� �ʰ� �����Ͽ� ���.
	for (const TSubclassOf<UBladeGameplayAbility>& Ability : InAbilitiesToGive) {
		if (!Ability) continue;
		FGameplayAbilitySpec AbilitySpec(Ability);
		//�ɷ��� ��ó���Ǵ� ��ü. �ɷ��� ��𿡼� �����ߴ���, � ��ü�� ���� �ɷ��� �����Ǿ������� ���� ����.
		AbilitySpec.SourceObject = InASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		InASCToGive->GiveAbility(AbilitySpec);
	}
}

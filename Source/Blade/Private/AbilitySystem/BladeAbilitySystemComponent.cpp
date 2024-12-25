// YunMinSeong. All right reserved. 


#include "AbilitySystem/BladeAbilitySystemComponent.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "BladeGameplayTags.h"
//#include "AbilitySystem/Abilites/BladeGameplayAbility.h"
void UBladeAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;
	
	for (const FGameplayAbilitySpec& AbilitySpec :GetActivatableAbilities()) {
		//HasTagExact는 정확히 동일한 태그를 확인하는 함수입니다.
		//HasTag는 상위 및 하위 태그 관계도 고려하여 일치 여부를 확인합니다.
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))continue;
		if (InputTag.MatchesTag(BladeGameplayTags::InputTag_Toggleable) && AbilitySpec.IsActive()) {
			CancelAbilityHandle(AbilitySpec.Handle);
		}
		else {
			TryActivateAbility(AbilitySpec.Handle);
		}
	
	}
}

void UBladeAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()|| !InputTag.MatchesTag(BladeGameplayTags::InputTag_MustBeHeld)) {
		return;
	}
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)&& AbilitySpec.IsActive()) {
			CancelAbilityHandle(AbilitySpec.Handle);
		}
	}
}

void UBladeAbilitySystemComponent::GrantPlayerWeaponAbilities(const TArray<FBladePlayerAbilitySet>& InDefaultWeaponAbilities, const TArray<FBladePlayerSpecialAbilitySet>& InSpecialWeaponAbilities, int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty())return;
	for (const FBladePlayerAbilitySet& AbilitySet:InDefaultWeaponAbilities) {
		if (!AbilitySet.IsValid()) continue;
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		//AbilitySet은 무기에 대한 어빌리티를 포함하고 있지만, 해당 어빌리티가 실제로 적용되는 대상은 **UBladeAbilitySystemComponent**가 부착된 캐릭터, 
		//즉 PlayerCharacter가 됩
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
	for (const FBladePlayerSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities) {
		if (!AbilitySet.IsValid()) continue;
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		//AbilitySet은 무기에 대한 어빌리티를 포함하고 있지만, 해당 어빌리티가 실제로 적용되는 대상은 **UBladeAbilitySystemComponent**가 부착된 캐릭터, 
		//즉 PlayerCharacter가 됩
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UBladeAbilitySystemComponent::RemoveGrantPlayerWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandleToRemove)
{
	if (InSpecHandleToRemove.IsEmpty())return;
	for (const FGameplayAbilitySpecHandle& SpecHandle: InSpecHandleToRemove) {
		if (SpecHandle.IsValid()) {
			ClearAbility(SpecHandle);
		}
	}
	InSpecHandleToRemove.Empty();
}

bool UBladeAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag AbilityTagToActivate)
{//이게 Weapon이었던거같은데
	check(AbilityTagToActivate.IsValid());
	TArray<FGameplayAbilitySpec*> FoundAbilitySpec;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpec);
	if (!FoundAbilitySpec.IsEmpty()) {
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpec.Num() - 1);
		FGameplayAbilitySpec* SpecToActive= FoundAbilitySpec[RandomAbilityIndex];
		check(SpecToActive);
		if (!SpecToActive->IsActive()) {
			return TryActivateAbility(SpecToActive->Handle);
		}
	}
	return false;
}

void UBladeAbilitySystemComponent::NativeAbilityActivate(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))continue;
		if (InputTag.MatchesTag(BladeGameplayTags::InputTag_Toggleable)) {
			if (AbilitySpec.IsActive()) {
				CancelAbilityHandle(AbilitySpec.Handle);
			}
			TryActivateAbility(AbilitySpec.Handle);
			//조건을 무시하고 강제로 실행....인데 왜 안되는거지?
			//InternalTryActivateAbility(AbilitySpec.Handle,FPredictionKey());
		}
	}
}

void UBladeAbilitySystemComponent::NativeAbilityDeActivate(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities()) {
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))continue;
		if (InputTag.MatchesTag(BladeGameplayTags::InputTag_Toggleable)) {
			if (AbilitySpec.IsActive()) {
				CancelAbilityHandle(AbilitySpec.Handle);
			}
			//TryActivateAbility(AbilitySpec.Handle);
			//조건을 무시하고 강제로 실행....인데 왜 안되는거지?
			//InternalTryActivateAbility(AbilitySpec.Handle,FPredictionKey());
		}
	}
}

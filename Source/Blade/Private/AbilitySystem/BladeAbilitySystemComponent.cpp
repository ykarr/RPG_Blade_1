// YunMinSeong. All right reserved. 


#include "AbilitySystem/BladeAbilitySystemComponent.h"
#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "BladeGameplayTags.h"
//#include "AbilitySystem/Abilites/BladeGameplayAbility.h"
void UBladeAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid())return;
	
	for (const FGameplayAbilitySpec& AbilitySpec :GetActivatableAbilities()) {
		//HasTagExact�� ��Ȯ�� ������ �±׸� Ȯ���ϴ� �Լ��Դϴ�.
		//HasTag�� ���� �� ���� �±� ���赵 ����Ͽ� ��ġ ���θ� Ȯ���մϴ�.
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
		//AbilitySet�� ���⿡ ���� �����Ƽ�� �����ϰ� ������, �ش� �����Ƽ�� ������ ����Ǵ� ����� **UBladeAbilitySystemComponent**�� ������ ĳ����, 
		//�� PlayerCharacter�� ��
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
	for (const FBladePlayerSpecialAbilitySet& AbilitySet : InSpecialWeaponAbilities) {
		if (!AbilitySet.IsValid()) continue;
		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		//AbilitySet�� ���⿡ ���� �����Ƽ�� �����ϰ� ������, �ش� �����Ƽ�� ������ ����Ǵ� ����� **UBladeAbilitySystemComponent**�� ������ ĳ����, 
		//�� PlayerCharacter�� ��
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
{//�̰� Weapon�̾����Ű�����
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
			//������ �����ϰ� ������ ����....�ε� �� �ȵǴ°���?
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
			//������ �����ϰ� ������ ����....�ε� �� �ȵǴ°���?
			//InternalTryActivateAbility(AbilitySpec.Handle,FPredictionKey());
		}
	}
}

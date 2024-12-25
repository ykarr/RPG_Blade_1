// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "Characters/BladePlayerCharacter.h"
#include "Controllers/BladeController.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"

#include "BladeGameplayTags.h"
ABladePlayerCharacter* UBladePlayerGameplayAbility::GetPlayerCharacterFromActorInfo() 
{//�ѹ� ���� ������ ������ �ٽ� Cast���� �ʵ���.
	if (!CachedBladePlayerCharacter.IsValid()) {
		//Cach�� �����Ͱ� ���°�� ����.
		CachedBladePlayerCharacter = Cast<ABladePlayerCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedBladePlayerCharacter.IsValid()? CachedBladePlayerCharacter.Get() : nullptr;
}

ABladeController* UBladePlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedBladePlayerController.IsValid()) {
		//Cach�� �����Ͱ� ���°�� ����.
		CachedBladePlayerController = Cast<ABladeController>(CurrentActorInfo->PlayerController);
	}
	return CachedBladePlayerController.IsValid() ? CachedBladePlayerController.Get() : nullptr;
}

UPlayerCombatComponent* UBladePlayerGameplayAbility::GetPlayerCombatComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerCombatComponent();
}

FGameplayEffectSpecHandle UBladePlayerGameplayAbility::MakePlayerDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponDamage, FGameplayTag InCurrentAttackTypeTag, int32 InCurrentComboCount)
{
	check(EffectClass);
	// Gameplay Effect�� �߻��� **��Ȳ�� ����(���ؽ�Ʈ)**�� �����ϴ� ����ü
	//����, ���, ��� �� ȿ���� �ߵ��ߴ����� ���� ������ ���Ե˴ϴ�. �ַ� �߻�ü, ������, ������ ���� ������ ����
	FGameplayEffectContextHandle ContextHandle = GetBladeAbilitySystemComponentFromActoInfo()->MakeEffectContext();
	//����Ʈ�� �ߵ��� ��ü�� ������ ����.
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

	//������ ���� ����, �±� ���� ������ ������� Gameplay Effect�� ��� �۵������� ����.
	FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);
	//SetSetByCallerMagnitude(FGameplayTag, float): Ư�� �±׷� ���ǵ� ���� ����.
	//GetDuration() : �� ȿ���� ���� �ð��� ��ȯ.
	//GetMagnitude() : �� ȿ���� �Ӽ��� �󸶳� ������ �ִ���(��) ��ȯ.
	EffectSpecHandle.Data ->SetSetByCallerMagnitude(
		BladeGameplayTags::Shared_SetByCaller_BaseDamage,
		InWeaponDamage
	);
	if (InCurrentAttackTypeTag.IsValid()) {
		EffectSpecHandle.Data -> SetSetByCallerMagnitude(InCurrentAttackTypeTag, InCurrentComboCount);
	}
	return EffectSpecHandle;
}

bool UBladePlayerGameplayAbility::GetAbilityRemainingCooldownByTag(FGameplayTag InCoolDownTag, float& TotalCooldownTime, float& RemainingCooldownTime)
{
	if (!InCoolDownTag.IsValid()) {
		return false;
	}
	FGameplayEffectQuery CooldownQuery=FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCoolDownTag.GetSingleTagContainer());
	TArray<TPair<float,float>> TimeRemainingAndDuration= GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);
	if (!TimeRemainingAndDuration.IsEmpty()) {
		RemainingCooldownTime=TimeRemainingAndDuration[0].Key;
		TotalCooldownTime = TimeRemainingAndDuration[0].Value;
	}
	return RemainingCooldownTime>0.f;
}

UPlayerUIComponent* UBladePlayerGameplayAbility::GetPlayerUIComponentFromActorInfo()
{
	return GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent();
}

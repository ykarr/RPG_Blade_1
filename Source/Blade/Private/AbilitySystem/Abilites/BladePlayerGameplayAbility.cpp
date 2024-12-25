// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/BladePlayerGameplayAbility.h"
#include "Characters/BladePlayerCharacter.h"
#include "Controllers/BladeController.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"

#include "BladeGameplayTags.h"
ABladePlayerCharacter* UBladePlayerGameplayAbility::GetPlayerCharacterFromActorInfo() 
{//한번 값을 가지고 있으면 다시 Cast하지 않도록.
	if (!CachedBladePlayerCharacter.IsValid()) {
		//Cach된 데이터가 없는경우 실행.
		CachedBladePlayerCharacter = Cast<ABladePlayerCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedBladePlayerCharacter.IsValid()? CachedBladePlayerCharacter.Get() : nullptr;
}

ABladeController* UBladePlayerGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if (!CachedBladePlayerController.IsValid()) {
		//Cach된 데이터가 없는경우 실행.
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
	// Gameplay Effect가 발생한 **상황적 정보(컨텍스트)**를 관리하는 구조체
	//누가, 어디서, 어떻게 이 효과를 발동했는지에 대한 정보가 포함됩니다. 주로 발사체, 공격자, 피해자 등의 정보가 포함
	FGameplayEffectContextHandle ContextHandle = GetBladeAbilitySystemComponentFromActoInfo()->MakeEffectContext();
	//이펙트를 발동한 주체와 원인을 설정.
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(),GetAvatarActorFromActorInfo());
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

	//레벨과 스택 정보, 태그 등의 정보를 기반으로 Gameplay Effect가 어떻게 작동할지를 설정.
	FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
	);
	//SetSetByCallerMagnitude(FGameplayTag, float): 특정 태그로 정의된 값을 설정.
	//GetDuration() : 이 효과의 지속 시간을 반환.
	//GetMagnitude() : 이 효과가 속성에 얼마나 영향을 주는지(값) 반환.
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

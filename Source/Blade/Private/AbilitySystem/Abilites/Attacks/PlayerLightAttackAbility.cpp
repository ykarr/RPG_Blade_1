// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/Attacks/PlayerLightAttackAbility.h"
#include "BladeFunctionLibrary.h"
#include "CustomDebugHelper.h"
#include "GameplayEffect.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Characters/BladePlayerCharacter.h"
#include "Components/UI/PlayerUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystemComponent.h"
#include "BladeGameplayTags.h"
UAnimMontage* UPlayerLightAttackAbility::FindCurrentAttackMontage()
{
	UAnimMontage*const* FindMontage= AttackMontageMap.Find(CurrentLightAttackComboCount);
	if (FindMontage != nullptr) {
		return *FindMontage;
	}
	return nullptr;
}

void UPlayerLightAttackAbility::CalcComboCount()
{
	int32 FullAttackCount = AttackMontageMap.Num();
	if (FullAttackCount == CurrentLightAttackComboCount) {
		ResetAttackComboCount();
	}
	else {
		if ((FullAttackCount -1) ==  CurrentLightAttackComboCount) {
			//Debug::Print("You Can FinishAttack");
			UBladeFunctionLibrary::AddGameplayTagToActorIfNone(GetOwningActorFromActorInfo(),FGameplayTag::RequestGameplayTag(FName("Player.State.FinishAttack")));
		}
		CurrentLightAttackComboCount++;
	}
}
void UPlayerLightAttackAbility::ResetAttackComboCount()
{
	CurrentLightAttackComboCount = 1;
	//Debug::Print("FinishAttack Reset");
	UBladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetOwningActorFromActorInfo(), FGameplayTag::RequestGameplayTag(FName("Player.State.FinishAttack")));
}

void UPlayerLightAttackAbility::DamageDealing(FGameplayEventData Payload)
{
	const AActor* Target = Payload.Target;
	AActor* TargetActor = const_cast<AActor*>(Target);

	float Damage=GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
	if (GEDeal_DamageToken&& InCurrentAttackTypeTag.IsValid()) {
		//CachedAttackComboCount블루프린트에서 설정.
		//Debug::Print("CachedAttackComboCount", CachedAttackComboCount);
		FGameplayEffectSpecHandle AbilitySpecHandle =  MakePlayerDamageEffectSpecHandle(GEDeal_DamageToken, Damage, InCurrentAttackTypeTag, CachedAttackComboCount);
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, AbilitySpecHandle);
		if (ActiveGameplayEffectHandle.WasSuccessfullyApplied()) {
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor,BladeGameplayTags::Shared_Event_HitReact, Payload);
			ApplyGameplayEffectToOwner(GetCurrentAbilitySpecHandle(),CurrentActorInfo,CurrentActivationInfo, GainRageClass->GetDefaultObject<UGameplayEffect>(),GetAbilityLevel());
		}
		//TODO: 나중에 수정.
	}
}

void UPlayerLightAttackAbility::HandleApplyDamage(FGameplayEventData Payload)
{
	const AActor* Target = Payload.Target;
	AActor* TargetActor = const_cast<AActor*>(Target);
	float Damage = GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
	FGameplayEffectSpecHandle EffectSpecHandle = MakePlayerDamageEffectSpecHandle(GEDeal_DamageToken, Damage, InCurrentAttackTypeTag,4);

	if (UBladeFunctionLibrary::ApplyGameEffectSpecHandleToTargetActor(GetOwningActorFromActorInfo(), TargetActor, EffectSpecHandle)) {
		UAbilitySystemComponent* const AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo_Checked();
		AbilitySystemComponent->ExecuteGameplayCue(GameplayCueTag, FGameplayCueParameters());

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, BladeGameplayTags::Shared_Event_HitReact, Payload);
	}
}

void UPlayerLightAttackAbility::CoolDownStart()
{
	CommitAbility(GetCurrentAbilitySpecHandle(),CurrentActorInfo,CurrentActivationInfo);
	GetPlayerCharacterFromActorInfo()->GetPlayerUIComponent()->OnAbilityCoolDownBegin.Broadcast(AbilityCoolDownTag,GetCooldownTimeRemaining(), GetCooldownTimeRemaining());
}

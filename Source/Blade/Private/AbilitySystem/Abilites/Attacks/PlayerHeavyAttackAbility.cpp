// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/Attacks/PlayerHeavyAttackAbility.h"
#include "BladeFunctionLibrary.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BladeGameplayTags.h"
//Debug
#include "CustomDebugHelper.h"
UAnimMontage* UPlayerHeavyAttackAbility::FindCurrentAttackMontage()
{
	if (UAnimMontage** FindMontage = AttackMontageMap.Find(CurrentHeavyAttackComboCount))
	{
		return *FindMontage;
	}
	return nullptr;
}


void UPlayerHeavyAttackAbility::CalcComboCount()
{
	if (AttackMontageMap.Num()== CurrentHeavyAttackComboCount) {
		ResetAttackComboCount();
	}
	else {
		CurrentHeavyAttackComboCount++;
	}
}

void UPlayerHeavyAttackAbility::ResetAttackComboCount()
{
	CurrentHeavyAttackComboCount = 1;
	UBladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetOwningActorFromActorInfo(), FGameplayTag::RequestGameplayTag(FName("Player.State.FinishAttack")));
}

void UPlayerHeavyAttackAbility::PlayerCanFinishAttack()
{
	if (UBladeFunctionLibrary::NativeDoesActorHaveTag(GetOwningActorFromActorInfo(), FGameplayTag::RequestGameplayTag(FName("Player.Statdsdfe.FinishAttack")))) {
		CurrentHeavyAttackComboCount = AttackMontageMap.Num();
	}
}

void UPlayerHeavyAttackAbility::DealingDamage(FGameplayEventData Payload)
{
	const AActor* Target = Payload.Target;
	AActor* TargetActor = const_cast<AActor*>(Target);

	float Damage = GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
	if (GEDeal_DamageToken&& InCurrentAttackTypeTag.IsValid()) {
		FGameplayEffectSpecHandle EffectSpecHandle = MakePlayerDamageEffectSpecHandle(GEDeal_DamageToken,Damage, InCurrentAttackTypeTag, CachedAttackComboCount);
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor,EffectSpecHandle);

		if (ActiveGameplayEffectHandle.WasSuccessfullyApplied()) {
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, BladeGameplayTags::Shared_Event_HitReact, Payload);
			ApplyGameplayEffectToOwner(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, GainRageClass->GetDefaultObject<UGameplayEffect>(), GetAbilityLevel());
		}
	}
}

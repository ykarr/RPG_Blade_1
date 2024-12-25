// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/Attacks/EnemyMeleeAttackAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BladeGameplayTags.h"

#include "CustomDebugHelper.h"
void UEnemyMeleeAttackAbility::DealingDamage(FGameplayEventData Payload)
{
	const AActor* Target = Payload.Target;
	AActor* TargetActor = const_cast<AActor*>(Target);
	if (GEDeal_DamageToken) {
		FGameplayEffectSpecHandle EffectSpecHandle = MakeEnemyDamageEffectSpecHandle(GEDeal_DamageToken, InDamageScalableFloat);
		//ActiveEffectHandle까지 하면 데미지는 들어감.
		FActiveGameplayEffectHandle ActiveEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, EffectSpecHandle);
		if (ActiveEffectHandle.WasSuccessfullyApplied()) {
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, BladeGameplayTags::Shared_Event_HitReact, Payload);
		}
	}
}

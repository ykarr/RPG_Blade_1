// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/BladeEnemyGameplayAbility.h"
#include "Characters/BladeEnemyCharacterBase.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"

#include "BladeGameplayTags.h"

ABladeEnemyCharacterBase* UBladeEnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedEnemyCharacter.IsValid()) {
		CachedEnemyCharacter=Cast<ABladeEnemyCharacterBase>(CurrentActorInfo->AvatarActor);
	}
	return CachedEnemyCharacter.IsValid()? CachedEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UBladeEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	return GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponent();
}

void UBladeEnemyGameplayAbility::OnEnemyDied()
{
	if (GetEnemyCharacterFromActorInfo()->GetClass()->ImplementsInterface(UEnemyDieInterface::StaticClass())) {
		IEnemyDieInterface* TargetActor = GetEnemyCharacterFromActorInfo();
		TargetActor->OnEnemyDied();
		//Enemy�� ������
	}
}

FGameplayEffectSpecHandle UBladeEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InDamageScalableFloat)
{
	check(EffectClass);
	// **Gameplay Effect(�����÷��� ȿ��)**�� ������ ��, �ش� ȿ���� ���õ� �ƶ�(Context) ������ �����ϰ� �����ϴ� �� ���Ǵ� ����ü
	FGameplayEffectContextHandle ContextHandle = GetBladeAbilitySystemComponentFromActoInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

	FGameplayEffectSpecHandle EffectSpecHandle = GetBladeAbilitySystemComponentFromActoInfo()->MakeOutgoingSpec(EffectClass,GetAbilityLevel(), ContextHandle);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(BladeGameplayTags::Shared_SetByCaller_BaseDamage, InDamageScalableFloat.GetValueAtLevel(GetAbilityLevel()));
	
	return EffectSpecHandle;
}

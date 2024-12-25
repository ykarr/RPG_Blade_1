// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/BladeGameplayAbility.h"
#include "AbilitySystem/BladeAbilitySystemComponent.h"
#include "Components/Combat/BaseCombatComponent.h"
#include "BladeGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BladeFunctionLibrary.h"
void UBladeGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	//능력이 부여된 후 활성화하겠음.
	if (AbilityActivationPolicy == EBladeAbilityActivationPolicy::OnGiven) {
		if (ActorInfo && !Spec.IsActive()) {
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UBladeGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (AbilityActivationPolicy == EBladeAbilityActivationPolicy::OnGiven) {
		if (ActorInfo) {
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

UBaseCombatComponent* UBladeGameplayAbility::GetPawnCombatComponentFromActoInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UBaseCombatComponent>();
}

UBladeAbilitySystemComponent* UBladeGameplayAbility::GetBladeAbilitySystemComponentFromActoInfo() const
{
	return Cast<UBladeAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UBladeGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	//check();
	if (TargetASC && InSpecHandle.IsValid()) {
		return GetBladeAbilitySystemComponentFromActoInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data,TargetASC);
	}
	else {
		return FActiveGameplayEffectHandle();
	}
}

FActiveGameplayEffectHandle UBladeGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, EBladeSuccessType& OutSuccessType)
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(TargetActor, InSpecHandle);
	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? EBladeSuccessType::Successful : EBladeSuccessType::Failed;
	return ActiveGameplayEffectHandle;
}

void UBladeGameplayAbility::NativeApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const TArray<FHitResult>&InHitResults)
{
	if (InHitResults.IsEmpty()) {
		return;
	}
	TArray<AActor*> SameActor;
	APawn* OwningPawn=Cast<APawn>(GetAvatarActorFromActorInfo());
	for (const FHitResult& HitResult : InHitResults) {
		if (SameActor.Contains(HitResult.GetActor())) continue;
		SameActor.Add(HitResult.GetActor());
		if (APawn* HitPawn = Cast<APawn>(HitResult.GetActor())) {
			if (UBladeFunctionLibrary::IsTargetPawnHostile(OwningPawn, HitPawn)) {
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle =NativeApplyEffectSpecHandleToTarget(HitPawn,InSpecHandle);
				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied()) {
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target = HitPawn;
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitPawn, BladeGameplayTags::Shared_Event_HitReact, Data);
				}
			}
		}
	}
	SameActor.Empty();
}

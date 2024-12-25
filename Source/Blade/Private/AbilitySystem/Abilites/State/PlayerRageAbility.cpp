// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/State/PlayerRageAbility.h"
#include "Abilities\Tasks\AbilityTask_PlayMontageAndWait.h"
#include "Abilities\Tasks\AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayTag.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"
#include "BladeGameplayTags.h"
#include "BladeFunctionLibrary.h"

#include "CustomDebugHelper.h"
void UPlayerRageAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UBladeFunctionLibrary::AddGameplayTagToActorIfNone(GetOwningActorFromActorInfo(), BladeGameplayTags::Player_State_Rage_Activating);
	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this,FName("RageActivating"), PlayMontage);
	PlayMontageAndWait->OnCancelled.AddDynamic(this, &ThisClass::OnMontageEnded);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &ThisClass::OnMontageEnded);
	PlayMontageAndWait->OnInterrupted.AddDynamic(this, &ThisClass::OnMontageEnded);
	PlayMontageAndWait->OnBlendOut.AddDynamic(this, &ThisClass::OnMontageEnded);
	PlayMontageAndWait->Activate();
	//PlayMontageAndWait->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitBeginGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this,BladeGameplayTags::Player_Event_ActivateRage);
	WaitBeginGameplayEvent->EventReceived.AddDynamic(this,&ThisClass::OnRecivedBeginEvent);
	WaitBeginGameplayEvent->ReadyForActivation();

	UAbilityTask_WaitGameplayTagAdded* WaitEndGameplayEvent = UAbilityTask_WaitGameplayTagAdded::WaitGameplayTagAdd(this,BladeGameplayTags::Player_State_Rage_None);
	//WaitEndGameplayEvent->Added.Add(&ThisClass::OnRecivedEndEvent);
	WaitEndGameplayEvent->Added.AddDynamic(this,&ThisClass::OnRecivedEndEvent);
	WaitEndGameplayEvent->ReadyForActivation();

	ActiveGameplayEffectHandle = BP_ApplyGameplayEffectToOwner(RageCost, GetAbilityLevel());
}



void UPlayerRageAbility::OnMontageEnded()
{
	UBladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetOwningActorFromActorInfo(), BladeGameplayTags::Player_State_Rage_Activating);
}

void UPlayerRageAbility::OnRecivedBeginEvent(FGameplayEventData Data)
{
	FGameplayCueParameters CueParms;
	CueParms.TargetAttachComponent = GetOwningComponentFromActorInfo();
	K2_AddGameplayCueWithParams(FGameplayTag::RequestGameplayTag(FName("GameplayCue.FX.Rage.Activating")), CueParms);

	
}

void UPlayerRageAbility::OnRecivedEndEvent()
{
	Debug::Print("OnRecivedEndEvent");
	Debug::Print("OnRecivedEndEvent");
	Debug::Print("OnRecivedEndEvent");
	CancelAbility(GetCurrentAbilitySpecHandle(),CurrentActorInfo,GetCurrentActivationInfo(),0 );
}
void UPlayerRageAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	//ActiveGameplayEffectHandle.
	BP_RemoveGameplayEffectFromOwnerWithHandle(ActiveGameplayEffectHandle);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
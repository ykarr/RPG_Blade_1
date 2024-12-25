// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/Pickup/PlayerPickupPotionAbility.h"
#include "Characters\BladePlayerCharacter.h"
#include "Kismet\KismetSystemLibrary.h"
#include "Items\Pickups\PickupPotionBase.h"
#include "Components/UI/PlayerUIComponent.h"
void UPlayerPickupPotionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	GetPlayerUIComponentFromActorInfo()->OnPotionInteractedDelegate.Broadcast(true);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}



void UPlayerPickupPotionAbility::CollectStones()
{
	CollectedPotions.Empty();
	ABladePlayerCharacter* PlayerCharacter = GetPlayerCharacterFromActorInfo();
	FVector ActorLocVec = PlayerCharacter->GetActorLocation();
	FVector ActorDownVec = PlayerCharacter->GetActorLocation()- PlayerCharacter->GetActorForwardVector()* BoxTraceDistance;
	FRotator ActorRot = PlayerCharacter->GetActorRotation();
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMultiForObjects(PlayerCharacter, ActorLocVec, ActorDownVec, TraceBoxSize/2.f, ActorRot, PotionTraceChannel,false,TArray<AActor*>(), TraceDebug?EDrawDebugTrace::ForDuration:EDrawDebugTrace::None, HitResults,true);
	
	for (const FHitResult& TraceHit: HitResults) {
		if (APickupPotionBase* FoundPotion = Cast<APickupPotionBase>(TraceHit.GetActor())) {
			CollectedPotions.AddUnique(FoundPotion);
		}
	}
	if (CollectedPotions.IsEmpty()) {
		CancelAbility(GetCurrentAbilitySpecHandle(),CurrentActorInfo,CurrentActivationInfo,true);
	}
}

void UPlayerPickupPotionAbility::ConsumePotions()
{
	if (CollectedPotions.IsEmpty()) {
		CancelAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true);
		return;
	}
	for (APickupPotionBase* FoundPotion :CollectedPotions) {
		FoundPotion->Consume(GetBladeAbilitySystemComponentFromActoInfo(), GetAbilityLevel());
	}
}
void UPlayerPickupPotionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	GetPlayerUIComponentFromActorInfo()->OnPotionInteractedDelegate.Broadcast(false);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
// YunMinSeong. All right reserved. 


#include "Components/Combat/EnemyCombatComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BladeGameplayTags.h"
#include "BladeFunctionLibrary.h"
#include "Characters/BladeEnemyCharacterBase.h"
#include "Components/BoxComponent.h"
//Debug
#include "CustomDebugHelper.h"
void UEnemyCombatComponent::OnHitTargetActor(AActor* HitActor)
{
	Super::OnHitTargetActor(HitActor);
	if (HitActor) {
		Debug::Print(GetOwningPawn()->GetActorNameOrLabel()+" "+HitActor->GetActorNameOrLabel());
	}
	if (OverlappedActors.Contains(HitActor)) {
		return;
	}
	OverlappedActors.AddUnique(HitActor);
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking = UBladeFunctionLibrary::NativeDoesActorHaveTag(HitActor,BladeGameplayTags::Player_State_Blocking);
	//const bool bIsMyAttackUnBlockable = false;
	const bool bIsMyAttackUnBlockable = UBladeFunctionLibrary::NativeDoesActorHaveTag(GetOwningPawn() , BladeGameplayTags::Enemy_Status_Unblockable);
	if (bIsPlayerBlocking&& !bIsMyAttackUnBlockable) {
		//���� �� �ִ� ������ �÷��̾ �������.
		bIsValidBlock=UBladeFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}

	FGameplayEventData EventData;
	EventData.Instigator = GetOwningPawn();
	EventData.Target = HitActor;

	if (bIsValidBlock) {
		//�������.
		//Debug::Print("Block");
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(HitActor,BladeGameplayTags::Player_Event_SuccessfullBlock,EventData);
	}
	else {
		//���� ���� ���.
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			BladeGameplayTags::Shared_Event_MeleeHit,
			EventData
		);
	}
}

void UEnemyCombatComponent::ToggleCurrentBodyCollision(bool bShouldEnable, EToggleDamageType ToogleDamageType)
{
	ABladeEnemyCharacterBase* OwningEnemyCharacter = GetOwningPawn<ABladeEnemyCharacterBase>();
	check(OwningEnemyCharacter);
		
	UBoxComponent* RightHand= OwningEnemyCharacter->GetRightHandCollisionBox();
	UBoxComponent* LeftHand= OwningEnemyCharacter->GetLeftHandCollisionBox();
	switch (ToogleDamageType)
	{
	case EToggleDamageType::RightHand:
		RightHand->SetCollisionEnabled(bShouldEnable?ECollisionEnabled::QueryOnly: ECollisionEnabled::NoCollision);
		break;
	case EToggleDamageType::LeftHand:
		LeftHand->SetCollisionEnabled(bShouldEnable?ECollisionEnabled::QueryOnly: ECollisionEnabled::NoCollision);
		break;
	default:
		break;
	}
	if (!bShouldEnable) {
		OverlappedActors.Empty();
	}
}

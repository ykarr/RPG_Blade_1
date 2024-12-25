// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/State/EnemyHitReactAbility.h"
#include "Characters/BladeEnemyCharacterBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Kismet\KismetMathLibrary.h"

#include "GameFramework\CharacterMovementComponent.h"
//Debug
#include "CustomDebugHelper.h"
void UEnemyHitReactAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ActivateEvent(TriggerEventData);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UEnemyHitReactAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetMaterials();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEnemyHitReactAbility::SetMaterials()
{
	GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1);
	if (bHasChildMesh) {
		TArray<USceneComponent*> ChildrenComponents;
		GetOwningComponentFromActorInfo()->GetChildrenComponents(false, ChildrenComponents);
		for (USceneComponent* Child : ChildrenComponents) {
			USkeletalMeshComponent* ChildStaticMesh = Cast<USkeletalMeshComponent>(Child);
			if (!ChildStaticMesh) continue;
			ChildStaticMesh->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 1);
		}
	}
}

void UEnemyHitReactAbility::ResetMaterials()
{
	GetOwningComponentFromActorInfo()->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0);
	if (bHasChildMesh) {
		TArray<USceneComponent*> ChildrenComponents;
		GetOwningComponentFromActorInfo()->GetChildrenComponents(false, ChildrenComponents);
		for (USceneComponent* Child : ChildrenComponents) {
			USkeletalMeshComponent* ChildStaticMesh = Cast<USkeletalMeshComponent>(Child);
			if (!ChildStaticMesh) continue;
			ChildStaticMesh->SetScalarParameterValueOnMaterials(FName("HitFxSwitch"), 0);
		}
	}
	
}

void UEnemyHitReactAbility::ActivateEvent(const FGameplayEventData* TriggerEventData)
{
	const AActor* InstigatorActor= TriggerEventData->Instigator;

	if (bFaceAttacker) {
		//때린 Attacker를 바라보도록.
		FRotator LookToFace =UKismetMathLibrary::FindLookAtRotation(GetEnemyCharacterFromActorInfo()->GetActorLocation(), InstigatorActor->GetActorLocation());

		if (UCharacterMovementComponent* CharacterMovementComp = GetEnemyCharacterFromActorInfo()->GetCharacterMovement()) {
			TSharedPtr<FRootMotionSource_ConstantForce> ApplyRootMotionConstantForce = MakeShared<FRootMotionSource_ConstantForce>();
			//ApplyRootMotionConstantForceTmp = *ApplyRootMotionConstantForce;
			//ApplyRootMotionConstantForce = TSharedPtr<FRootMotionSource_ConstantForce>();
			ApplyRootMotionConstantForce->InstanceName = "ConstantForce";
			ApplyRootMotionConstantForce->AccumulateMode = ERootMotionAccumulateMode::Additive;
			ApplyRootMotionConstantForce->Force = GetOwningActorFromActorInfo()->GetActorForwardVector() * -1 * 160;//뒤로 밀림.
			ApplyRootMotionConstantForce->Duration = 0.2f;
			//ApplyRootMotionConstantForce->Priority = 1;
			CharacterMovementComp->ApplyRootMotionSource(ApplyRootMotionConstantForce);
		}
	}
	if (bHasHitReactMontagesToPlay) {
		//맞은 후 애니메이션.
		SetPlayMontage();
	}
	
	//맞았을때 Material변경.
	SetMaterials();

	if (!bHasHitReactMontagesToPlay) {
		FTimerHandle DelayTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayTimer,FTimerDelegate::CreateLambda([this](){
			EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, false, false);
		}), 0.2, false);
	}
	checkf(EnemyUnderAttack,TEXT("Fill EnemyHitReact"));
	
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(EnemyUnderAttack ,GetAbilityLevel());
	ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo , SpecHandle);
}

void UEnemyHitReactAbility::SetPlayMontage()
{
	if (MontageToPlay.Num() > 0) {
		// 배열에서 랜덤 인덱스 선택
		int32 RandomIndex = FMath::RandRange(0, MontageToPlay.Num() - 1);
		// 선택된 몽타주 가져오기
		UAnimMontage* SelectedMontage = MontageToPlay[RandomIndex];
		UAbilityTask_PlayMontageAndWait* PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("HitReact"), SelectedMontage);
		PlayMontageAndWait->OnBlendOut.AddDynamic(this,&ThisClass::OnMontageBlendOut);
		PlayMontageAndWait->OnCompleted.AddDynamic(this,&ThisClass::OnMontageCompleted);
		PlayMontageAndWait->OnInterrupted.AddDynamic(this,&ThisClass::OnMontageInterrupted);
		PlayMontageAndWait->OnCancelled.AddDynamic(this,&ThisClass::OnMontageCancelled);
		PlayMontageAndWait->ReadyForActivation();
	}
}

void UEnemyHitReactAbility::OnMontageCompleted()
{
	EndAbility(GetCurrentAbilitySpecHandle(),CurrentActorInfo,CurrentActivationInfo,false,false);
}

void UEnemyHitReactAbility::OnMontageBlendOut()
{
	EndAbility(GetCurrentAbilitySpecHandle(),CurrentActorInfo,CurrentActivationInfo,false,false);
}

void UEnemyHitReactAbility::OnMontageInterrupted()
{
	EndAbility(GetCurrentAbilitySpecHandle(),CurrentActorInfo,CurrentActivationInfo,false, true);
}

void UEnemyHitReactAbility::OnMontageCancelled()
{
	EndAbility(GetCurrentAbilitySpecHandle(),CurrentActorInfo,CurrentActivationInfo,false,false);
}

// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/Attacks/SpecialAttackAbility.h"
#include "BladeGameplayTags.h"
#include "Characters/BladePlayerCharacter.h"
//FunctionLibrary
#include "NiagaraFunctionLibrary.h"
#include "Kismet\KismetSystemLibrary.h"
//Task
#include "Abilities\Tasks\AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystem/AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"
//#include "Abilities\Tasks\AbilityTask_WaitGameplayEvent.h"
//Algorithm
#include "Algo\Sort.h"
//Components
#include "Components/CapsuleComponent.h"
#include "Camera\CameraComponent.h"
#include "Components/Combat/PlayerCombatComponent.h"
#include "Components/UI/PlayerUIComponent.h"
//Debug
#include "CustomDebugHelper.h"
void USpecialAttackAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
void USpecialAttackAbility::CheckDuplicate(TArray<FHitResult>& InHits)
{
	TArray<FHitResult> InHitResults;
	FVector PlayerLocation = GetOwningActorFromActorInfo()->GetActorLocation();
	float MaxDistance= 2e9f;
	for (const FHitResult& hit : InHits) {
		if (hit.GetActor() && !InHitResults.ContainsByPredicate([&](const FHitResult& ExistingHit) {
			return ExistingHit.GetActor() == hit.GetActor();
		})) {
			InHitResults.Add(hit);
		}
	}
	/*
	FVector PlayerLoc = GetOwningActorFromActorInfo()->GetActorLocation();
	Algo::Sort(InHitResults,[&](const FHitResult& A, const FHitResult& B){
		float DistanceA = FVector::DistSquared(PlayerLoc, A.ImpactPoint);
		float DistanceB = FVector::DistSquared(PlayerLoc, B.ImpactPoint);
		return DistanceA < DistanceB;
	});*/
	InHits = InHitResults;
}
void USpecialAttackAbility::BlinkDash(float DashDistance, UNiagaraSystem* InNiagaraSystem, UAnimMontage* InAnimMontage, bool SpawnStartEmitter , bool SpawnEndEmitter)
{
	AActor* OwningActor = GetOwningActorFromActorInfo();
	//벡터가 가리키는 방향을 기반으로 하는 회전 값이 필요할 때
	FRotator OrientRot = OwningActor->GetActorForwardVector().ToOrientationRotator(); //FRotator OrientRot = FRotator(0.f, CurrentActorInfo->PlayerController->GetControlRotation().Yaw, 0.f);
	//Attack Animation Set
	AttackAnimSet(InAnimMontage);
	
	GetOwningComponentFromActorInfo()->SetHiddenInGame(true);

	/* Blink Start Emitter */
	if (SpawnStartEmitter) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InNiagaraSystem, OwningActor->GetActorLocation(), OrientRot);
	}

	/* Trace */
	TryTraceFunc(OrientRot, DashDistance);

	/*Blink Move*/
	IsMoving = true;
	PlayerCameraComponent = GetPlayerCharacterFromActorInfo()->GeCameraComponent();
	FLatentActionInfo Info;
	Info.CallbackTarget = this;
	Info.UUID = 0;
	Info.Linkage = 0;
	Info.ExecutionFunction = FName("OnBlinkFinish");
	PlayerCapsuleComponent = GetPlayerCharacterFromActorInfo()->GetCapsuleComponent();
	UKismetSystemLibrary::MoveComponentTo(PlayerCapsuleComponent, TargetPos, OwningActor->GetActorRotation(), false, false, 0.4, false, EMoveComponentAction::Move, Info);

	/*Blink End Spawn Emitter*/
	if (SpawnEndEmitter) {
		FTimerHandle DelaySpawn;
		GetWorld()->GetTimerManager().SetTimer(DelaySpawn, FTimerDelegate::CreateLambda([this, InNiagaraSystem, OrientRot]() {UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), InNiagaraSystem, TargetPos, OrientRot); }), 0.2f, false);
	}
	/*Blink Move Tick*/
	UAbilityTask_ExecuteTaskOnTick* TickTask = UAbilityTask_ExecuteTaskOnTick::ExecuteTaskTick(this);
	TickTask->OnAbilityTaskTick.AddUniqueDynamic(this, &USpecialAttackAbility::OnBlinkDashTick);
	TickTask->ReadyForActivation();
	SetSpecialAttackCoolTime();
}

void USpecialAttackAbility::OnBlinkDashTick(float DeltaTime)
{
	if (IsMoving) {
		float NewFOV= FMath::InterpEaseOut(PlayerCameraComponent->FieldOfView,110.f,DeltaTime*2,2);
		PlayerCameraComponent->SetFieldOfView(NewFOV);
	}
	else {
		float NewFOV = FMath::InterpEaseOut(PlayerCameraComponent->FieldOfView, 90.f, DeltaTime * 2, 2);
		PlayerCameraComponent->SetFieldOfView(NewFOV);
	}
}

void USpecialAttackAbility::OnBlinkFinish()
{
	IsMoving = false;
	GetOwningComponentFromActorInfo()->SetHiddenInGame(false);
}
void USpecialAttackAbility::AttackAnimSet(UAnimMontage* InAnimMontage)
{
	if (InAnimMontage) {
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName("Attack"), InAnimMontage);
		MontageTask->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnAnimEnded);
		MontageTask->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnAnimEnded);
		MontageTask->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnAnimEnded);
		MontageTask->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnAnimEnded);
		MontageTask->Activate();
	}
}
void USpecialAttackAbility::OnAnimEnded()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void USpecialAttackAbility::TryTraceFunc( FRotator InOrient,float InDistance)
{
	AActor* OwningActor = GetOwningActorFromActorInfo();
	FVector Start = OwningActor->GetActorLocation();
	FVector End = Start + OwningActor->GetActorForwardVector() * InDistance;
	FVector HalfSize = FVector(50, 50, 200);
	TArray<FHitResult> Hits;
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(OwningActor);
	bool bHitSuccessed = UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), Start, End, HalfSize, InOrient, ObjectTypes, false, IgnoreActor, bEnableDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, Hits, true);
	if (bHitSuccessed) {
		CheckDuplicate(Hits);
		AActor* HitActor = Hits.Last().GetActor();
		FVector DirectionToEnemy = (HitActor->GetActorLocation() - GetOwningActorFromActorInfo()->GetActorLocation()).GetSafeNormal();
		TargetPos = HitActor->GetActorLocation() + DirectionToEnemy * 100.f;
		ApplyAreaOfEffect(Hits);
	}
	else {
		TargetPos = OwningActor->GetActorLocation() + OwningActor->GetActorForwardVector() * InDistance;
	}
}
void USpecialAttackAbility::ApplyAreaOfEffect(const TArray<FHitResult>& InHitResults)
{
	if (EffectClass) {
		float WeaponDamage = GetPlayerCombatComponentFromActorInfo()->GetPlayerCurrentEquipWeaponDamageAtLevel(GetAbilityLevel());
		FGameplayEffectSpecHandle EffectSpecHandle = MakePlayerDamageEffectSpecHandle(EffectClass, WeaponDamage, BladeGameplayTags::Player_SetByCaller_AttackType_Heavy, 10);
		NativeApplyGameplayEffectSpecHandleToHitResults(EffectSpecHandle, InHitResults);
	}
}

void USpecialAttackAbility::SetSpecialAttackCoolTime()
{
	if (ensure(SpecialAttackCoolDownTag.IsValid())) {
		CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo());
		GetPlayerUIComponentFromActorInfo()->OnAbilityCoolDownBegin.Broadcast(SpecialAttackCoolDownTag, GetCooldownTimeRemaining(), GetCooldownTimeRemaining());
	}
	else {
		Debug::Print("CoolDownTag is Not Valid");
	}
}

void USpecialAttackAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

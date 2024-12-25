// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/PlayerTargetLockAbility.h"
#include "Characters\BladePlayerCharacter.h"
#include "Characters/BladeEnemyCharacterBase.h"
#include "Controllers/BladeController.h"
//Kismet
#include "Kismet\KismetSystemLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet\KismetMathLibrary.h"
//Widget
#include "Widgets/TargetLockWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Components/SizeBox.h"
#include "AbilitySystem/AbilityTasks/AbilityTask_ExecuteTaskOnTick.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

#include "BladeFunctionLibrary.h"
#include "BladeGameplayTags.h"
//input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//Debug
#include "CustomDebugHelper.h"
void UPlayerTargetLockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();
	InitTargetLockInputMappingContext();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	//UAbilityTask_ExecuteTaskOnTick* TickTask=UAbilityTask_ExecuteTaskOnTick::ExecuteTaskOnTick(this);
	UAbilityTask_ExecuteTaskOnTick* TickTask=UAbilityTask_ExecuteTaskOnTick::ExecuteTaskTick(this);
	TickTask->OnAbilityTaskTick.AddUniqueDynamic(this, &ThisClass::OnTargetLockTick);
	TickTask->ReadyForActivation();


	check(SwichTargetTag.IsValid());
	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, SwichTargetTag,nullptr,false,false);
	WaitGameplayEvent->EventReceived.AddUniqueDynamic(this, &ThisClass::SwitchTarget);
	WaitGameplayEvent->ReadyForActivation();
}
void UPlayerTargetLockAbility::TryLockOnTarget()
{
	GetAvailableActorsToTarget();
	if (AvailableActorsToLock.IsEmpty()) {
		CancelTargetLockAbility();
		return;
	}
	CurrentLockActor = GetNearestTargetFromAvailableActors(AvailableActorsToLock);
	if (CurrentLockActor) {
		DrawTargetLockWidget();
		SetTargetLockWidgetLoc();
	}
	else {
		CancelTargetLockAbility();
	}
}
void UPlayerTargetLockAbility::GetAvailableActorsToTarget()
{//TargetLock할 Actor탐색.
	AvailableActorsToLock.Empty();
	TArray<FHitResult> BoxTraceHits;
	AActor* OwningCharacter=GetOwningActorFromActorInfo();
	UKismetSystemLibrary::BoxTraceMultiForObjects(
		OwningCharacter,
		OwningCharacter->GetActorLocation(),
		OwningCharacter->GetActorLocation() + OwningCharacter->GetActorForwardVector() * TraceDistance,
		TraceBoxSize / 2.f,
		OwningCharacter->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel, false,
		TArray<AActor*>(),
		bIsDebugging ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxTraceHits, true
	);
	for (const FHitResult& BoxHit : BoxTraceHits) {
		if (AActor* HitActor = BoxHit.GetActor()) {
			//if (HitActor != GetPlayerCharacterFromActorInfo()) {
			if (HitActor->IsA(ABladeEnemyCharacterBase::StaticClass())) {
				AvailableActorsToLock.AddUnique(HitActor);
			}
		}
	}
}
void UPlayerTargetLockAbility::OnTargetLockTick(float DeltaTime)
{
	//TargetLock된 Actor가 없거나 TargetLock된 Actor가 죽었거나, 플레이어가 죽었다면 이 어빌리티를 종료.
	if (!CurrentLockActor ||
		UBladeFunctionLibrary::NativeDoesActorHaveTag(CurrentLockActor, BladeGameplayTags::Shared_Status_Dead) ||
		UBladeFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), BladeGameplayTags::Shared_Status_Dead)) {
		CancelTargetLockAbility();
		return;
	}
	SetTargetLockWidgetLoc();
	//구르지 않고 Blocking중이지 않은 경우.
	const bool bShouldOverrideRotation = !ThisActorHasTag(BladeGameplayTags::Player_State_Rolling) && !ThisActorHasTag(BladeGameplayTags::Player_State_Blocking);
	if (bShouldOverrideRotation) {
		FVector Start = GetPlayerCharacterFromActorInfo()->GetActorLocation();
		FVector End = CurrentLockActor->GetActorLocation();
		FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(Start, End);

		const FRotator CurrentRot = GetPlayerControllerFromActorInfo()->GetControlRotation();
		const FRotator TargetRot = FMath::RInterpTo(CurrentRot, LookRot, DeltaTime, TargetLockRotationInterpSpeed);
		LookRot -= FRotator(TargetLockCameraOffsetDistance, 0.f, 0.f);
		GetPlayerControllerFromActorInfo()->SetControlRotation(FRotator(TargetRot.Pitch, TargetRot.Yaw, 0.f));
		GetPlayerCharacterFromActorInfo()->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
	}
}

bool UPlayerTargetLockAbility::ThisActorHasTag(FGameplayTag InTag)
{//액터가 특정 태그를 가지고있는지 확인.
	return UBladeFunctionLibrary::NativeDoesActorHaveTag(GetPlayerCharacterFromActorInfo(), InTag);
}
AActor* UPlayerTargetLockAbility::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActors)
{//가장 가까운 Target찾기
	float ClosestDistance = 0.f;
	return UGameplayStatics::FindNearestActor(GetPlayerCharacterFromActorInfo()->GetActorLocation(), InAvailableActors, ClosestDistance);
}
void UPlayerTargetLockAbility::DrawTargetLockWidget()
{//TargetLoc Widget추가.
	if (!TargetLockWidget) {
		checkf(TargetLockWidgetClass, TEXT("TargetLockWidgetClass is none"));
		TargetLockWidget = CreateWidget<UTargetLockWidget>(GetPlayerControllerFromActorInfo(), TargetLockWidgetClass);
		if (TargetLockWidget) {
			TargetLockWidget->AddToViewport();
		}
	}
}
void UPlayerTargetLockAbility::SetTargetLockWidgetLoc()
{//TargetLock 위젯의 위치조절.
	if (!TargetLockWidget || !CurrentLockActor) {
		CancelTargetLockAbility();
		return;
	}
	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetPlayerControllerFromActorInfo(),
		CurrentLockActor->GetActorLocation(),
		ScreenPosition,
		true
	);
	if (TargetLockBoxSIze == FVector2D::ZeroVector) {
		TargetLockBoxSIze.X = TargetLockWidget->TargetLockBox->GetWidthOverride();
		TargetLockBoxSIze.Y = TargetLockWidget->TargetLockBox->GetHeightOverride();
	}
	ScreenPosition -= TargetLockBoxSIze / 2.f;
	TargetLockWidget->SetPositionInViewport(ScreenPosition,false);
}
void UPlayerTargetLockAbility::InitTargetLockMovement()
{
	//지울예정.
}
void UPlayerTargetLockAbility::ResetTargetLockMovement()
{
	//지울예정.
}
#pragma region Input | SwitchTarget
void UPlayerTargetLockAbility::InitTargetLockInputMappingContext()
{
	if (GetPlayerControllerFromActorInfo()) {
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerControllerFromActorInfo()->GetLocalPlayer());
		check(subsystem);
		subsystem->AddMappingContext(TargetLockMappingContext, 2);
	}
}
void UPlayerTargetLockAbility::ResetTargetLockInputMappingContext()
{
	if (GetPlayerControllerFromActorInfo()) {
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetPlayerControllerFromActorInfo()->GetLocalPlayer());
		check(subsystem);
		subsystem->RemoveMappingContext(TargetLockMappingContext);
	}
}
void UPlayerTargetLockAbility::SwitchTarget(FGameplayEventData Data)
{
	GetAvailableActorsToTarget();
	FGameplayTag InSwitchTag = Data.EventTag;
	TArray<AActor*> ActorOnLeft;
	TArray<AActor*> ActorOnRight;
	AActor* NewTargetToLock = nullptr;
	GetAvailableActorsAroundTarget(ActorOnLeft, ActorOnRight);
	//Debug::Print(Data.EventTag.ToString());
	if (InSwitchTag==BladeGameplayTags::Player_Event_SwitchTarget_Left) {
		NewTargetToLock=GetNearestTargetFromAvailableActors(ActorOnLeft);
	}
	else {
		NewTargetToLock=GetNearestTargetFromAvailableActors(ActorOnRight);
	}
	if (NewTargetToLock) {
		CurrentLockActor = NewTargetToLock;
	}
}
void UPlayerTargetLockAbility::GetAvailableActorsAroundTarget(TArray<AActor*>& OutActorsOnLeft, TArray<AActor*>& OutActorsOnRight)
{
	if (!CurrentLockActor || AvailableActorsToLock.IsEmpty()) {
		CancelTargetLockAbility();
		return;
	}
	const FVector PlayerLocation = GetPlayerCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockActor->GetActorLocation()- PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor: AvailableActorsToLock) {
		if (!AvailableActor|| AvailableActor==CurrentLockActor) {
			continue;
		}
		const FVector PlayerToAvailableNormalized= (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossResult= FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);
		if (CrossResult.Z > 0.f) {
			OutActorsOnRight.AddUnique(AvailableActor);
		}
		else {
			OutActorsOnLeft.AddUnique(AvailableActor);
		}
	}
}
#pragma endregion
void UPlayerTargetLockAbility::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}
void UPlayerTargetLockAbility::CleanUp()
{
	AvailableActorsToLock.Empty();
	CurrentLockActor = nullptr;
	if (TargetLockWidget) {
		TargetLockWidget->RemoveFromParent();
	}
	TargetLockWidget = nullptr;
	TargetLockBoxSIze = FVector2D::ZeroVector;
}
void UPlayerTargetLockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanUp();
	ResetTargetLockInputMappingContext();
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
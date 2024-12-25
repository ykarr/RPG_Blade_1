// YunMinSeong. All right reserved. 


#include "AbilitySystem/Abilites/Block/PlayerBlockAbility.h"
#include "AbilitySystem\BladeAbilitySystemComponent.h"
#include "Kismet\KismetMathLibrary.h"
#include "Characters/BladePlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BladeFunctionLibrary.h"
#include "BladeGameplayTags.h"
#include "Controllers\BladeController.h"
//Debug
#include "CustomDebugHelper.h"
void UPlayerBlockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	BladeAbilitySystemComp = GetBladeAbilitySystemComponentFromActoInfo();
	Super::ActivateAbility(Handle,  ActorInfo, ActivationInfo, TriggerEventData);
	SetBlockActiveTIme();
	
	FRotator ActorRot = GetOwningActorFromActorInfo()->GetActorRotation();
	ActorRot.Yaw= GetPlayerControllerFromActorInfo()->GetControlRotation().Yaw;
	GetOwningActorFromActorInfo()->SetActorRotation(ActorRot);
}



void UPlayerBlockAbility::SetBlockActiveTIme()
{
	BlockActiveTime = GetWorld()->GetTimeSeconds();
}

void UPlayerBlockAbility::SetGameplayMagicshieldParms()
{//1
	FGameplayTag GameplayCueTag = FGameplayTag::RequestGameplayTag(FXMagicShieldTag);
	checkf(GameplayCueTag.IsValid(), TEXT("PlayerBlock FXMagicShieldTag is Not Valid"));
	FGameplayCueParameters CueParams;
	CueParams.TargetAttachComponent = GetOwningComponentFromActorInfo();
	BladeAbilitySystemComp->AddGameplayCue(GameplayCueTag,CueParams);

	if (bIsRemoveTagToEnd) {
		TrackedGameplayCues.Add(GameplayCueTag);
	}
}
void UPlayerBlockAbility::ReceiveBlock(FGameplayEventData Payload)
{
	//Debug::Print("ss");
	//PerfectBlock조건.
	bIsPerfectBlock = (GetWorld()->GetTimeSeconds() - BlockActiveTime) <= 0.5;
	ABladePlayerCharacter* OwningActor = GetPlayerCharacterFromActorInfo();

	FVector Start = OwningActor->GetActorLocation();
	FVector End = Payload.Instigator->GetActorLocation();
	//GetBladeAbilitySystemComponentFromActoInfo()->NativeAbilityActivate(BladeGameplayTags::InputTag_Toggleable_TargetLock);
	//방어에 성공했다면 마주보도록 회전.
	FRotator TargetRot=UKismetMathLibrary::FindLookAtRotation(Start, End);
	//bool bRot= OwningActor->bUseControllerRotationYaw;
	//OwningActor->bUseControllerRotationYaw = false;
	OwningActor ->SetActorRotation(TargetRot);
	//OwningActor->GetController()->SetControlRotation(TargetRot);
	//OwningActor->bUseControllerRotationYaw = bRot;

	//Block중 공격을 받으면 반대쪽으로 밀림.
	if (UCharacterMovementComponent* CharacterMovementComp = OwningActor->GetCharacterMovement()) {
		TSharedPtr<FRootMotionSource_ConstantForce> ApplyRootMotionConstantForce = MakeShared<FRootMotionSource_ConstantForce>();
		//ApplyRootMotionConstantForceTmp = *ApplyRootMotionConstantForce;
		//ApplyRootMotionConstantForce = TSharedPtr<FRootMotionSource_ConstantForce>();
		ApplyRootMotionConstantForce->InstanceName = "ConstantForce";
		ApplyRootMotionConstantForce->AccumulateMode = ERootMotionAccumulateMode::Additive;
		ApplyRootMotionConstantForce->Force = OwningActor->GetActorForwardVector() * -1*Strength;//뒤로 밀림.
		ApplyRootMotionConstantForce->Duration = 0.2f;
		//ApplyRootMotionConstantForce->Priority = 1;
		CharacterMovementComp->ApplyRootMotionSource(ApplyRootMotionConstantForce);
	}
	//ExcuteGameplayCue
	FGameplayTag SuccessBlockTag = FGameplayTag::RequestGameplayTag(ExecutSuccessfulBlockTag);
	checkf(SuccessBlockTag.IsValid(),TEXT("PlayerBlock ExecuteMagicShieldTag is Not Valid"));

	FGameplayCueParameters CueParams;
	CueParams.AbilityLevel = GetAbilityLevel();
	CueParams.TargetAttachComponent = GetOwningComponentFromActorInfo();
	BladeAbilitySystemComp->ExecuteGameplayCue(SuccessBlockTag, CueParams);

	if (bIsPerfectBlock) {
		UCharacterMovementComponent* CharacterMovementComp = OwningActor->GetCharacterMovement();
		//CharacterMovementComp->RemoveRootMotionSourceByID(RootMotionSourceHandle);
		CharacterMovementComp->RemoveRootMotionSource("ConstantForce");

		UBladeFunctionLibrary::AddGameplayTagToActorIfNone(OwningActor, BladeGameplayTags::Player_State_FinishAttack);
		FGameplayTag PerfectBlockTag = FGameplayTag::RequestGameplayTag(ExecutePerfectBlockTag);
		BladeAbilitySystemComp->ExecuteGameplayCue(PerfectBlockTag, CueParams);
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.1);
		FTimerHandle DelayTimerHandle;
		//GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
		//SetGlobalTimeDilation으로 1초후에 실행됨
		GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle,FTimerDelegate::CreateLambda([this, OwningActor]()
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
				StartResetFinishAttackTimer();
				OwningActor->CustomTimeDilation = 1.3;
			}), 1*0.1, false);
	}
}
void UPlayerBlockAbility::StartResetFinishAttackTimer()
{
	//if (!GetWorld()->GetTimerManager().IsTimerActive(ResetStateTimer)) {
	FTimerHandle ResetStateTimer;
	GetWorld()->GetTimerManager().SetTimer(ResetStateTimer, FTimerDelegate::CreateLambda([this]() {
		UBladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetPlayerCharacterFromActorInfo(), BladeGameplayTags::Player_State_FinishAttack);
		//Debug::Print("StartResetFinishAttackTimer");
		//GetBladeAbilitySystemComponentFromActoInfo()->NativeAbilityDeActivate(BladeGameplayTags::InputTag_Toggleable_TargetLock);
		GetPlayerCharacterFromActorInfo()->CustomTimeDilation = 1;
	}), 1, false);
	//}
}
void UPlayerBlockAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	for (const FGameplayTag & CueTag:TrackedGameplayCues) {
		if (!CueTag.IsValid()) continue;
		BladeAbilitySystemComp->RemoveGameplayCue(CueTag);
	}
	TrackedGameplayCues.Empty();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
	StartResetFinishAttackTimer();
	
	//GetPlayerCharacterFromActorInfo()->GetCharacterMovement()
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
}
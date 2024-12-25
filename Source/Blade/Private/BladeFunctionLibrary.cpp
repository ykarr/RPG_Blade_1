// YunMinSeong. All right reserved. 


#include "BladeFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BladeAbilitySystemComponent.h"
#include "Interfaces\CombatComponentInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "BladeGameplayTags.h"
#include "BladeTypes\BladeCountDownAction.h"
#include "BladeGameInstance.h"
#include "Kismet\GameplayStatics.h"
#include "SaveGame/BladeSaveGame.h"
//Debug
#include "CustomDebugHelper.h"
UBladeAbilitySystemComponent* UBladeFunctionLibrary::NativeGetBladeASCFromActor(AActor* InActor)
{
	check(InActor);

	//UAbilitySystemComponent* Ÿ���� �����͸� ��ȯ�մϴ�. ���� �ش� ���Ͱ� AbilitySystemComponent�� ������ ���� �ʴٸ�, nullptr�� ��ȯ�մϴ�.
	return CastChecked<UBladeAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
	
}

void UBladeFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UBladeAbilitySystemComponent* ASC =NativeGetBladeASCFromActor(InActor);
	if (!ASC->HasMatchingGameplayTag(TagToAdd)) {
		ASC->AddLooseGameplayTag(TagToAdd);
	}
}

void UBladeFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UBladeAbilitySystemComponent* ASC = NativeGetBladeASCFromActor(InActor);
	if (ASC->HasMatchingGameplayTag(TagToRemove)) {
		ASC->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UBladeFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UBladeAbilitySystemComponent* ASC = NativeGetBladeASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

bool UBladeFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{//#include "GenericTeamAgentInterface.h"
	check(QueryPawn && TargetPawn);
	IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
	if (QueryTeamAgent&& TargetTeamAgent) {
		//���� �ʴٸ� ������.
		return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	return false;
}
UBaseCombatComponent* UBladeFunctionLibrary::NativeGetCombatComponent(AActor* InActor)
{
	check(InActor);
	if (ICombatComponentInterface* PawnCombatComponent = Cast<ICombatComponentInterface>(InActor)) {
		return PawnCombatComponent->GetCombatComponent();
	}
	return nullptr;
}
UBaseCombatComponent* UBladeFunctionLibrary::BP_GetCombatComponent(AActor* InActor, EBladehValidType& ValidType)
{
	UBaseCombatComponent* BaseCombatComponent = NativeGetCombatComponent(InActor);
	ValidType = BaseCombatComponent ? EBladehValidType::Valid : EBladehValidType::InValid;
	return BaseCombatComponent;
}

float UBladeFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, float InLevel)
{
	return InScalableFloat.GetValueAtLevel(InLevel);
}

FGameplayTag UBladeFunctionLibrary::ComputeHitReactDirectionTag(AActor* InAttacker, AActor* InVictim, float& OutAngleDifference)
{
	check(InAttacker&&InVictim);

	const FVector VictimForward = InVictim->GetActorForwardVector();
	const FVector VictimToAttackerNormalized = (InAttacker->GetActorLocation() - InVictim->GetActorLocation()).GetSafeNormal();
	
	const float DotResult=FVector::DotProduct(VictimForward, VictimToAttackerNormalized);
	OutAngleDifference =UKismetMathLibrary::DegAcos(DotResult);
	

	const FVector CrossResult= FVector::CrossProduct(VictimForward, VictimToAttackerNormalized);
	if (CrossResult.Z<0.f) {//����
		OutAngleDifference *= -1.f;
	}
	if (OutAngleDifference>=-45.f&& OutAngleDifference<=45.f) {
		return BladeGameplayTags::Shared_Status_HitReact_Front;
	}
	else if (OutAngleDifference < -45.f && OutAngleDifference >= -135.f) {
		return BladeGameplayTags::Shared_Status_HitReact_Left;
	}
	else if (OutAngleDifference > 45.f && OutAngleDifference <= 135.f) {
		return BladeGameplayTags::Shared_Status_HitReact_Right;
	}
	else if (OutAngleDifference > 135.f || OutAngleDifference < -135.f) {
		return BladeGameplayTags::Shared_Status_HitReact_Back;
	}
	return BladeGameplayTags::Shared_Status_HitReact_Front;
}

bool UBladeFunctionLibrary::IsValidBlock(AActor* InAttacker, AActor* InDefender)
{
	check(InAttacker && InDefender);
	const float DotResult=FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());
	// 1�� ������ �� ���Ͱ� ���� ����,0�̸� �� ���Ͱ� ����(90��)���� ����,-1�̸� �� ���Ͱ� �ݴ� ����
	return DotResult<-0.1f;
}

void UBladeFunctionLibrary::SaveCurrentGameDifficulty(EBladeGameDifficulty InDifficulty)
{
	USaveGame* SaveGameObject= UGameplayStatics::CreateSaveGameObject(UBladeSaveGame::StaticClass());
	if (UBladeSaveGame* BladeSaveGameObject =Cast<UBladeSaveGame>(SaveGameObject)) {
		BladeSaveGameObject->SaveGameDifficulty = InDifficulty;
		const bool bWasSaved= UGameplayStatics::SaveGameToSlot(BladeSaveGameObject,BladeGameplayTags::GameData_SaveGame_Slot1.GetTag().ToString(),0);
		Debug::Print("Save_Difficulty");
	}
}

bool UBladeFunctionLibrary::TryLoadSaveGameDifficulty(EBladeGameDifficulty& OutSaveDifficulty)
{
	if (UGameplayStatics::DoesSaveGameExist(BladeGameplayTags::GameData_SaveGame_Slot1.GetTag().ToString(), 0)) {
		USaveGame* SaveGameObject=UGameplayStatics::LoadGameFromSlot(BladeGameplayTags::GameData_SaveGame_Slot1.GetTag().ToString(), 0);
		if (UBladeSaveGame * BladeSaveGameObject=Cast<UBladeSaveGame>(SaveGameObject)) {
			OutSaveDifficulty=BladeSaveGameObject->SaveGameDifficulty;
			Debug::Print("Load_Difficulty");
			return true;
		}
	}
	
	return false;
}

void UBladeFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EBladeInputMode InInputMode)
{
	APlayerController* Controller = nullptr;
	if (GEngine) {
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
			Controller=World->GetFirstPlayerController();
		}
	}
	if (!Controller) {
		return;
	}
	switch (InInputMode)
	{
	case EBladeInputMode::GameOnly:
		Controller->SetInputMode(FInputModeGameOnly());
		Controller->bShowMouseCursor = false;
		break;
	case EBladeInputMode::UIOnly:
		Controller->SetInputMode(FInputModeUIOnly());
		Controller->bShowMouseCursor = true;
		break;
	default:
		break;
	}
}

UBladeGameInstance* UBladeFunctionLibrary::GetBladeGameInstance(const UObject* WorldContextObject)
{
	if (GEngine) {
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull)) {
			return World->GetGameInstance<UBladeGameInstance>();
		}
		
	}
	return nullptr;
}

bool UBladeFunctionLibrary::ApplyGameEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, const FGameplayEffectSpecHandle& InEffectSpecHandle)
{
	UBladeAbilitySystemComponent* SourceASC=  NativeGetBladeASCFromActor(InInstigator);
	UBladeAbilitySystemComponent* TargetASC = NativeGetBladeASCFromActor(InTargetActor);
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*InEffectSpecHandle.Data, TargetASC);
	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}



void UBladeFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EBladeConfirmType& OutConfirmType)
{
	
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EBladeConfirmType::YES : EBladeConfirmType::NO;
}
void UBladeFunctionLibrary::CoolDown(const UObject* WorldContextObject, float TotalTime, float UpdateInterval, float& OutRemainingTime, EBladeCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EBladeCountDownActionOutput& CountDownOutput, FLatentActionInfo LatentInfo) {
	UWorld* World = nullptr;
	if (GEngine) {
		World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	}
	if (!World) {
		return;
	}
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	//LatentInfo.CallbackTarget:Ÿ�� ��ü��, ���� �۾��� �Ϸ�� �� �ݹ� �Լ��� ����� ��ü�� ��Ÿ���ϴ�.
	//LatentInfo.UUID:**���� �ĺ���(UUID)** ��, Ư�� ���� �۾��� �ĺ��ϴ� �� ���˴ϴ�.���� ���� ���� �۾��� �ϳ��� Ÿ�� ��ü�� ��ϵ� �� �ֱ� ������, �� �۾��� �����ϱ� ���� ���� ID�� �ʿ���
	FBladeCountDownAction* FoundAction=LatentActionManager.FindExistingAction<FBladeCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);
	if (CountDownInput==EBladeCountDownActionInput::Start) {
		if (!FoundAction) {
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID,new FBladeCountDownAction(TotalTime,UpdateInterval,OutRemainingTime,CountDownOutput,LatentInfo));
		}
	}
	if (CountDownInput == EBladeCountDownActionInput::Cancel) {
		if (FoundAction) {
			FoundAction->CancelAction();
		}
	}
}
// YunMinSeong. All right reserved. 


#include "AI/BTService_OrientToTargetActor.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "AIController.h"
#include "Kismet\KismetMathLibrary.h"
UBTService_OrientToTargetActor::UBTService_OrientToTargetActor()
{
	NodeName = TEXT("Native Orient Rotation to Target Actor");

	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	RotationInterpSpeed = 5.f;
	//실행 간격?
	Interval = 0.f;
	//실행간격에서 RandomDeviation+-
	RandomDeviation = 0.f;

	IntargetActorKey.AddObjectFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass, IntargetActorKey),AActor::StaticClass());
}

void UBTService_OrientToTargetActor::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset=GetBlackboardAsset()) {
		IntargetActorKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_OrientToTargetActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp,NodeMemory,DeltaSeconds);
	UObject* ActorObject= OwnerComp.GetBlackboardComponent()->GetValueAsObject(IntargetActorKey.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(ActorObject);
	APawn* OwningPawn= OwnerComp.GetAIOwner()->GetPawn();
	if (OwningPawn&& TargetActor) {
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(),TargetActor->GetActorLocation());
		const FRotator TargetRot = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRot, DeltaSeconds,RotationInterpSpeed);
		
		OwningPawn->SetActorRotation(TargetRot);
	}
}

FString UBTService_OrientToTargetActor::GetStaticDescription() const
{
	const FString KeyDescription = IntargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(TEXT("Orient rotation to %s Key %s"), *KeyDescription, *GetStaticServiceDescription());
}

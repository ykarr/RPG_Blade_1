// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_OrientToTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class BLADE_API UBTService_OrientToTargetActor : public UBTService
{
	GENERATED_BODY()

	UBTService_OrientToTargetActor();
	//UBTNode Interface
	/// <summary>
	///  AI Behavior Tree 또는 블루프린트 노드와 같은 특정 노드의 설명을 반환하는 함수입니다. 
	///  이 함수는 주로 디버깅이나 에디터 상에서 시각적인 설명을 제공할 때 사용됩니다.
	/// </summary>
	/// <returns></returns>
	virtual FString GetStaticDescription() const override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	UPROPERTY(EditAnywhere,Category="Target")
	FBlackboardKeySelector IntargetActorKey;
	
	UPROPERTY(EditAnywhere,Category="Target")
	float RotationInterpSpeed;
};

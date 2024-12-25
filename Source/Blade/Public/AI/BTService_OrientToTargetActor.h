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
	///  AI Behavior Tree �Ǵ� �������Ʈ ���� ���� Ư�� ����� ������ ��ȯ�ϴ� �Լ��Դϴ�. 
	///  �� �Լ��� �ַ� ������̳� ������ �󿡼� �ð����� ������ ������ �� ���˴ϴ�.
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

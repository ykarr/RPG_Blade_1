// YunMinSeong. All right reserved. 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BladeAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class BLADE_API ABladeAIController : public AAIController
{
	GENERATED_BODY()
public:
	//상위 클래스 내부의 구성 요소를 재정의하기 위해.
	ABladeAIController(const FObjectInitializer& ObjectInitializer);
	//IGenericTeamAgentInterface
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
protected:
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AIData")
	UBehaviorTree* BladeBehaviorTree;
private:
	UPROPERTY(EditDefaultsOnly,Category="Detour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance = true;
	//bEnableDetourCrowdAvoidance가 True인경우에만 조작가능.
	UPROPERTY(EditDefaultsOnly,Category="Detour Crowd Avoidance Config",meta=(EditCondition="bEnableDetourCrowdAvoidance",UIMin=1,UIMax=4))
	int32 DetourCrowdAvoidanceQuality = 4;
	UPROPERTY(EditDefaultsOnly,Category="Detour Crowd Avoidance Config",meta=(EditCondition="bEnableDetourCrowdAvoidance"))
	float CollisionQueryRange = 600.f;
};

	//UPathFollowingComponent
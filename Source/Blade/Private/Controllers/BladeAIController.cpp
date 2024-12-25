// YunMinSeong. All right reserved. 


#include "Controllers/BladeAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
//Perception
#include "Perception\AIPerceptionComponent.h"
#include "Perception\AISenseConfig_Sight.h"
//AI Behavior
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
//
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "GenericTeamAgentInterface.h"
#include "CustomDebugHelper.h"
ABladeAIController::ABladeAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	
	
	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AISenseConfig_Sight"));
	/* DetectionByAffiliation: AI가 감지할 대상의 소속(Affiliation)을 설정할 수 있는 구조체
	** bDetectEnemies: 적대적인 대상을 감지할지 여부를 나타내는 불리언 값입니다.
	** bDetectFriendlies: 아군 대상을 감지할지 여부를 나타내는 불리언 값입니다.
	** bDetectFriendlies: 중립 대상을 감지할지 여부를 나타내는 불리언 값입니다.
	**/
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	//시야 거리.
	AISenseConfig_Sight->SightRadius = 5000.f;
	//적이 플레이어를 놓치지 않도록 설정.
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	//시야 각도.
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	// AI Perception 시스템에서 **주 감각(Dominant Sense)**을 설정하는 코드
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	// AI Perception Component에서 특정 **타겟(Actor)**에 대한 감각이 업데이트될 때 호출되는 델리게이트입니다
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this,&ThisClass::OnEnemyPerceptionUpdated);
	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type ABladeAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	Super::GetTeamAttitudeTowards(Other);
	const APawn* PawnToCheck= Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	//Player는 0으로 설정했고 Enemy는 1로 설정했음.
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId()<GetGenericTeamId()) {
		//적대관계
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void ABladeAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent()) {
		
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor"))) {
			//TargetActor가 설정되어 있지 않을때.
			if (Stimulus.WasSuccessfullySensed() && Actor) {
				//한번만 TargetActor를 업데이트하고 계속 유지함.
				BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
			}
		}
	}
	
}

void ABladeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BladeBehaviorTree) {
		RunBehaviorTree(BladeBehaviorTree);
		Blackboard->SetValueAsFloat(FName("DefaultMaxWalkSpeed"), Cast<ACharacter>(InPawn)->GetCharacterMovement()->MaxWalkSpeed);
	}
	//Debug::Print("Onpppppp");
}

void ABladeAIController::BeginPlay()
{
	Super::BeginPlay();
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent())) {
		
		//CrowdComp->SetCrowdSimulationState(ECrowdSimulationState::);
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance?ECrowdSimulationState::Enabled:ECrowdSimulationState::Disabled);
		switch (DetourCrowdAvoidanceQuality)
		{
		case 1:CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);break;
		case 2:CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium);break;
		case 3:CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);break;
		case 4:CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);break;
		default:break;
		}
		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
	else {
		Debug::Print("CrowdComp is Not Valid");
	}
}

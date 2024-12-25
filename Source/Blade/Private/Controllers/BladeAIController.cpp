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
	/* DetectionByAffiliation: AI�� ������ ����� �Ҽ�(Affiliation)�� ������ �� �ִ� ����ü
	** bDetectEnemies: �������� ����� �������� ���θ� ��Ÿ���� �Ҹ��� ���Դϴ�.
	** bDetectFriendlies: �Ʊ� ����� �������� ���θ� ��Ÿ���� �Ҹ��� ���Դϴ�.
	** bDetectFriendlies: �߸� ����� �������� ���θ� ��Ÿ���� �Ҹ��� ���Դϴ�.
	**/
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	//�þ� �Ÿ�.
	AISenseConfig_Sight->SightRadius = 5000.f;
	//���� �÷��̾ ��ġ�� �ʵ��� ����.
	AISenseConfig_Sight->LoseSightRadius = 0.f;
	//�þ� ����.
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	// AI Perception �ý��ۿ��� **�� ����(Dominant Sense)**�� �����ϴ� �ڵ�
	EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	// AI Perception Component���� Ư�� **Ÿ��(Actor)**�� ���� ������ ������Ʈ�� �� ȣ��Ǵ� ��������Ʈ�Դϴ�
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this,&ThisClass::OnEnemyPerceptionUpdated);
	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type ABladeAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	Super::GetTeamAttitudeTowards(Other);
	const APawn* PawnToCheck= Cast<const APawn>(&Other);
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());
	//Player�� 0���� �����߰� Enemy�� 1�� ��������.
	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId()<GetGenericTeamId()) {
		//�������
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void ABladeAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent()) {
		
		if (!BlackboardComponent->GetValueAsObject(FName("TargetActor"))) {
			//TargetActor�� �����Ǿ� ���� ������.
			if (Stimulus.WasSuccessfullySensed() && Actor) {
				//�ѹ��� TargetActor�� ������Ʈ�ϰ� ��� ������.
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

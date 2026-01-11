#include "CAIController.h"
#include "RPG/Utilities/CLog.h"
#include "CEnemy_AI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPG/Components/CAIBehaviorComponent.h"
#include "Particles/ParticleEventManager.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"

#include "Perception/AIPerceptionSystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISenseConfig_Team.h"
#include "RPG/Characters/CPlayer.h"

ACAIController::ACAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	Blackboard = this->CreateDefaultSubobject<UBlackboardComponent>("Blackboard");
	Perception = this->CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600;
	Sight->LoseSightRadius = 800;
	Sight->PeripheralVisionAngleDegrees = 60;
	Sight->SetMaxAge(3);
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	
	Perception->ConfigureSense(*Sight);

	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing");
	Hearing->HearingRange = 2000;
	Hearing->SetMaxAge(5);
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;

	Perception->ConfigureSense(*Hearing);


	Damage = CreateDefaultSubobject<UAISenseConfig_Damage>("Damage");
	Damage->SetMaxAge(5);
	Perception->ConfigureSense(*Damage);
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
	if (bDrawDebug == true)
	{
		FVector start = GetPawn()->GetActorLocation();
		start.Z -= 50;

		FVector end = start;

		DrawDebugCylinder(GetPawn()->GetWorld(), start, end, Sight->SightRadius, 10, FColor::Red, false, 3);
	}
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	State = Cast<UCStateComponent>(Enemy->GetComponentByClass(UCStateComponent::StaticClass()));
	MyTeam = Enemy->GetTeamID();
	SetGenericTeamId(MyTeam);

	if(Enemy->GetBehaviorTree() == nullptr)
		return;

	UBlackboardComponent* blackboard = Blackboard.Get();
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, blackboard);

	Behavior = Cast<UCAIBehaviorComponent>(Enemy->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	Behavior->SetBlackboard(blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}
bool ACAIController::GetPoint()
{
	return bPoint;
}

void ACAIController::NotPoint()
{
	bPoint = false;
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(State->IsDeadMode() == true)
	{
		Blackboard->SetValueAsObject("Target", nullptr);
		StopMovement();
		return;
	}
}

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray < AActor* > actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	if(actors.Num() <= 0)
	{
		Blackboard->SetValueAsObject("Target", nullptr);
		return;
	}

	for (AActor* actor : actors)
	{
		ACCharacter* character = Cast<ACCharacter>(actor);
		float team = character->GetTeam();
		
		if (team != MyTeam)
		{
			Behavior->SetTeamID(team);
			Blackboard->SetValueAsObject("Target", actor);
			
			return;
		}
	}

}

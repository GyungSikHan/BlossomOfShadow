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
	Sight->DetectionByAffiliation.bDetectEnemies = true;//적감지
	Sight->DetectionByAffiliation.bDetectNeutrals = true;//중립감지
	Sight->DetectionByAffiliation.bDetectFriendlies = true;//팀 감지
	
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

	//Team = CreateDefaultSubobject<UAISenseConfig_Team>("Team");
	//Perception->ConfigureSense(*Team);
	
	//Perception->SetDominantSense(*Sight->GetSenseImplementation());
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
	//Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ACAIController::OnTargetPerceptionUpdated);
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

//void ACAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	TArray<AActor*> actors;
//	TSubclassOf<UAISense> aiSense;
//	AAIController* controller = Cast<AAIController>(Actor->GetInstigatorController());
//	
//
//	aiSense = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
//
//	if (aiSense == UAISense_Hearing::StaticClass() || aiSense == UAISense_Damage::StaticClass())
//	{
//		bPoint = true;
//		Behavior->SetPointOfInterestLocation(Stimulus.StimulusLocation);
//		return;
//	}
//	else if (aiSense == UAISense_Sight::StaticClass())
//		Perception->GetCurrentlyPerceivedActors(aiSense, actors);
//
//	if (actors.Num() <= 0)
//	{
//		Blackboard->SetValueAsObject("Target", nullptr);
//		return;
//	}
//
//	for (AActor* actor : actors)
//		Blackboard->SetValueAsObject("Target", actor);
//}

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
	/*ACEnemy_AI* ai{};
	ACPlayer* player;*/
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
		/*ai = Cast<ACEnemy_AI>(actor);
		if (ai == nullptr)
		{
			player = Cast<ACPlayer>(actor);
			if (player != nullptr)
				team = player->GetTeam();
		}
		else
			team = ai->GetTeamID();*/

		if (team != MyTeam)
		{
			Behavior->SetTeamID(team);
			Blackboard->SetValueAsObject("Target", actor);
			
			return;
		}
	}

}













////void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& updatedActors)
////{
////	TArray<AActor*> actors;
////	TSubclassOf<UAISense> aiSense;
////
////	for (AActor* actor : updatedActors)
////	{
////		FActorPerceptionBlueprintInfo info;
////		if (Perception->GetActorsPerception(actor, info) == true)
////		{
////			for (FAIStimulus stimulu : info.LastSensedStimuli)
////			{
////				aiSense = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), stimulu);
////				if (aiSense == UAISense_Sight::StaticClass())
////					Perception->GetCurrentlyPerceivedActors(aiSense, actors);
////				//else if (aiSense == UAISense_Hearing::StaticClass())
////				//{
////				//	bPoint = true;
////				//	PointLocation = stimulu.StimulusLocation;
////				//}
////				//else if (aiSense == UAISense_Damage::StaticClass())
////				//	Perception->GetCurrentlyPerceivedActors(aiSense, actors);
////			}
////		}
////	}
////
////	if(actors.Num() <= 0)
////	{
////		Blackboard->SetValueAsObject("Target", nullptr);
////		return;
////	}
////	for (AActor* a : actors)
////		Blackboard->SetValueAsObject("Target", a);
////	
////}
//
////void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& updatedActors)
////{
////	//TArray<AActor*> actors;
////	//
////	//Perception->GetCurrentlyPerceivedActors(nullptr, actors);
////
////	//float myTeam = Behavior->GetMyTeam();
////
////	//if(actors.Num() <= 0)
////	//{
////	//	Blackboard->SetValueAsObject("Target", nullptr);
////	//	return;
////	//}
////
////	//for (AActor* a : actors)
////	//{
////	//	//float team = Behavior->GetTeam(Cast<ACharacter>(a));
////	//	//if((myTeam != team && team != 0))
////	//	//{
////	//	//}
////
////	//		Blackboard->SetValueAsObject("Target", a);
////	//}
////
////	////float team = Behavior->GetTeam(Cast<ACharacter>(actors[0]));
////	////if ((myTeam != team && team != 0) || Damage->IsEnabled() == true)
////	////{
////	////	Blackboard->SetValueAsObject("Target", actors[0]);
////	////	return;
////	////}
////
////
////
////	TArray < AActor* > actors;
////	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
////
////	if (actors.Num() > 0)
////	{
////		//CLog::Log(actors[0]->GetName());
////		Blackboard->SetValueAsObject("Target", actors[0]);
////
////		return;
////	}
////
////	Blackboard->SetValueAsObject("Target", nullptr);
////	
////}
//
//void ACAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	UKismetSystemLibrary::PrintString(GetOwner(), Actor->GetName());
//
//	//ACharacter* other = Cast<ACharacter>(Actor);
//	//float otherTeam = Behavior->GetTeam(other);
//
//	//if (other == nullptr)
//	//	return;
//	//else if (otherTeam == MyTeam || otherTeam == 0)
//	//	return;
//	
//	TArray<AActor*> actors;
//	TSubclassOf<UAISense> aiSense;
//
//	aiSense = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
//	bPoint = true;
//
//	if (aiSense == UAISense_Hearing::StaticClass())
//		PointLocation = Stimulus.StimulusLocation;
//	else if (aiSense == UAISense_Damage::StaticClass())
//		PointLocation = Stimulus.StimulusLocation;
//	else if(aiSense == UAISense_Sight::StaticClass())
//		Perception->GetCurrentlyPerceivedActors(aiSense, actors);
//
//	if (actors.Num() <= 0)
//	{
//		Blackboard->SetValueAsObject("Target", nullptr);
//		return;
//	}
//	for (AActor* a : actors)
//		Blackboard->SetValueAsObject("Target", a);
//}

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CEnemy_AI.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"

#include "CAIController.generated.h"

UCLASS()
class RPG_API ACAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug;
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* Perception;

public:
	ACAIController();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& updatedActors);
	//UFUNCTION()
	//void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

public:
	FORCEINLINE float GetLoseSightRadius() { return Sight->LoseSightRadius; }

public:
	bool GetPoint();
	//FVector GetPointLocation();
	void NotPoint();
	virtual void Tick(float DeltaTime) override;

private:
	class ACEnemy_AI* Enemy;
	class UCAIBehaviorComponent* Behavior;
	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Damage* Damage;
	class UAISenseConfig_Hearing* Hearing;
	class UAISenseConfig_Team* Team;

	bool bSensed;

	bool bPoint;
	float MyTeam;

	class UCStateComponent* State;

};



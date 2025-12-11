#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPortal.generated.h"

UCLASS()
class RPG_API ACPortal : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* Particle;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;					 
	UPROPERTY(VisibleAnywhere)
	class ALevelSequenceActor* LevelSequenceActor;
	UPROPERTY(EditAnywhere)
	class ULevelSequence* LevelSequence;
	UPROPERTY(VisibleAnywhere)
	class ULevelSequencePlayer* LevelSequencePlayer;

public:	
	ACPortal();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void End();

private:
	ACharacter* Player;
	APlayerController* Controller;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CCinematicActor.generated.h"

UCLASS()
class RPG_API ACCinematicActor : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere)
	class ALevelSequenceActor* LevelSequenceActor;
	UPROPERTY(EditAnywhere)
	class ULevelSequence* LevelSequence;
	UPROPERTY(VisibleAnywhere)
	class ULevelSequencePlayer* LevelSequencePlayer;
private:
	UPROPERTY(EditAnywhere)
	TArray<AActor*> DestroyActor;
public:	
	// Sets default values for this actor's properties
	ACCinematicActor();

protected:
	// Called when the game starts or when spawned
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

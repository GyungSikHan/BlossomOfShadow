// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

UCLASS()
class RPG_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Loop")
	bool bLoop;

	UPROPERTY(EditAnywhere, Category = "Path")
	int32 Index;

	UPROPERTY(EditAnywhere, Category = "Path")
	bool bReverse;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	class USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
	class UTextRenderComponent* Text;

public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }


public:
	ACPatrolPath();

protected:
	virtual void BeginPlay() override;
	void OnConstruction(const FTransform& Transform) override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	FVector GetMoveTo();
	void UpdateIndex();
};

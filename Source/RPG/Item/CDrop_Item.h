// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CDrop_Item.generated.h"



UCLASS(Blueprintable)
class RPG_API ACDrop_Item : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* Paeticle;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;
public:	
	ACDrop_Item();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};

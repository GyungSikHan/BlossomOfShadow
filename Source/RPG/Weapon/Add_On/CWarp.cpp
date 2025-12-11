// Fill out your copyright notice in the Description page of Project Settings.


#include "CWarp.h"

#include "Components/DecalComponent.h"

ACWarp::ACWarp()
{
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject<USceneComponent>(L"Scene");
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(Scene);


	DecalComponent->SetWorldRotation(FRotator(-90,0,0));
	DecalComponent->DecalSize = FVector(10, 64, 64);
}

void ACWarp::BeginPlay()
{
	Super::BeginPlay();
	
}

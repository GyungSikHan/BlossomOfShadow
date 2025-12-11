// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CDrop_Item.h"
#include "Utilities/CLog.h"
#include "Init/CGameInstance.h"
#include "Characters/CPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
ACDrop_Item::ACDrop_Item()
{
	Paeticle = CreateDefaultSubobject<UParticleSystemComponent>("Mesh");
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(RootComponent);
	Paeticle->SetupAttachment(Box);
}

// Called when the game starts or when spawned
void ACDrop_Item::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACDrop_Item::OnComponentBeginOverlap);
}

void ACDrop_Item::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UCGameInstance* gameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetOwner()->GetWorld()));
	gameInstance->ShowResultUI(GetOwner()->GetWorld(), false);
}




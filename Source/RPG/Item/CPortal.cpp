// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CPortal.h"
#include "EngineUtils.h"
#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Utilities/CLog.h"
#include "Characters/CPlayer.h"
#include "Characters/AI/CAIController.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Weapon/CAttachment.h"
#include "Widget/CUserWidget_PlayerInterface.h"

// Sets default values
ACPortal::ACPortal()
{
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(RootComponent);
	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	Particle->SetupAttachment(Box);

	LevelSequence = CreateDefaultSubobject<ULevelSequence>(L"LevelSequence");

	//static ConstructorHelpers::FObjectFinder<ULevelSequence> ls(L"/Script/LevelSequence.LevelSequence'/Game/Cinematics/Main1.Main1'");
	//if (ls.Succeeded())
	//	LevelSequence = ls.Object;

	LevelSequenceActor = CreateDefaultSubobject<ALevelSequenceActor>(L"LevelSequnceActor");
}

// Called when the game starts or when spawned
void ACPortal::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACPortal::OnComponentBeginOverlap);

	FMovieSceneSequencePlaybackSettings setting;
	setting.bAutoPlay = false;
	setting.bPauseAtEnd = true;
	if(LevelSequence == nullptr)
		return;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, setting, LevelSequenceActor);

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}


void ACPortal::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACPlayer>(OtherActor) != Player)
		return;
	if(LevelSequence == nullptr)
	{
		End();
		return;
	}
	//player->GetMesh()->SetVisibility(false);
	//PauseAllPawn();

	LevelSequencePlayer->OnFinished.AddDynamic(this, &ACPortal::End);
	LevelSequencePlayer->Play();

	Player->DisableInput(Controller);
	SetActorEnableCollision(false);
	TArray<AActor*> findActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), findActor);

	for (AActor* actor : findActor)
	{
		UClass* actorClass = actor->GetClass();
		if(UKismetMathLibrary::ClassIsChildOf(actorClass, ACharacter::StaticClass()) == true
			|| UKismetMathLibrary::ClassIsChildOf(actorClass, UUserWidget::StaticClass()) == true
			|| UKismetMathLibrary::ClassIsChildOf(actorClass, ACAttachment::StaticClass()) == true)
		{
			actor->SetActorHiddenInGame(true);
			actor->SetActorTickEnabled(false);
		}

	}
}

void ACPortal::End()
{
	UGameplayStatics::OpenLevel(GetWorld(), "BossMap");
	ACPlayer* player = Cast<ACPlayer>(Player);
	if (player != nullptr)
		player->SaveHP();
}

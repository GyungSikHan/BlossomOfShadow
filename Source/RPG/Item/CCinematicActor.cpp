// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/CCinematicActor.h"

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Blueprint/UserWidget.h"
#include "Characters/CPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/CAttachment.h"

// Sets default values
ACCinematicActor::ACCinematicActor()
{
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(RootComponent);

	LevelSequence = CreateDefaultSubobject<ULevelSequence>(L"LevelSequence");
	//static ConstructorHelpers::FObjectFinder<ULevelSequence> ls(L"/Script/LevelSequence.LevelSequence'/Game/Cinematics/Main2.Main2'");
	//if (ls.Succeeded())
	//	LevelSequence = ls.Object;

	LevelSequenceActor = CreateDefaultSubobject<ALevelSequenceActor>(L"LevelSequnceActor");
}

// Called when the game starts or when spawned
void ACCinematicActor::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACCinematicActor::OnComponentBeginOverlap);
	FMovieSceneSequencePlaybackSettings setting;
	setting.bAutoPlay = false;
	setting.bPauseAtEnd = true;
	if(LevelSequence == nullptr)
		return;
	LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, setting, LevelSequenceActor);

	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	Controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
}

void ACCinematicActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<ACPlayer>(OtherActor) != Player)
		return;
	if(LevelSequence == nullptr)
	{
		End();
		return;
	}
	LevelSequencePlayer->OnFinished.AddDynamic(this, &ACCinematicActor::End);
	LevelSequencePlayer->Play();
	
	Player->DisableInput(Controller);
	SetActorEnableCollision(false);
	TArray<AActor*> findActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), findActor);

	for (AActor* actor : findActor)
	{
		UClass* actorClass = actor->GetClass();
		if (UKismetMathLibrary::ClassIsChildOf(actorClass, ACharacter::StaticClass()) == true
			|| UKismetMathLibrary::ClassIsChildOf(actorClass, UUserWidget::StaticClass()) == true
			|| UKismetMathLibrary::ClassIsChildOf(actorClass, ACAttachment::StaticClass()) == true)
		{
			actor->SetActorHiddenInGame(true);
			actor->SetActorTickEnabled(false);
		}
	}
}

void ACCinematicActor::End()
{
	Player->EnableInput(Controller);
	SetActorEnableCollision(false);
	TArray<AActor*> findActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), findActor);

	for (AActor* actor : DestroyActor)
	{
		actor->SetActorHiddenInGame(true);
		actor->SetActorEnableCollision(false);
	}

	for (AActor* actor : findActor)
	{
		UClass* actorClass = actor->GetClass();
		if (UKismetMathLibrary::ClassIsChildOf(actorClass, ACharacter::StaticClass()) == true
			|| UKismetMathLibrary::ClassIsChildOf(actorClass, UUserWidget::StaticClass()) == true
			|| UKismetMathLibrary::ClassIsChildOf(actorClass, ACAttachment::StaticClass()) == true)
		{
			actor->SetActorHiddenInGame(false);
			actor->SetActorTickEnabled(true);
		}
	}
	LevelSequencePlayer->Stop();
}

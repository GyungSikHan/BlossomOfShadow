#include "CAura.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ACAura::ACAura()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Niagara = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
	Niagara->SetupAttachment(Root);
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Root);

	UNiagaraSystem* niagara;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> asset(L"/Script/Niagara.NiagaraSystem'/Game/Effects/sA_StylizedSwordSet/Fx/NS_Ulti_lv1.NS_Ulti_lv1'");
	niagara = asset.Object;
	Niagara->SetAsset(niagara);
}

// Called when the game starts or when spawned
void ACAura::BeginPlay()
{
	Super::BeginPlay();
	Niagara->SetVariableObject("Mesh_Scale", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACAura::OnNiagaraSystemFinished);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACAura::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACAura::OnComponentEndOverlap);

	FTimerDelegate delegate = FTimerDelegate::CreateLambda([&]()
		{
			for (int32 i = Hitted.Num() - 1; i >= 0; i--)
				HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
		}
	);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, delegate, DamageInterval, true, 0);
	if(SpawnLife != 0)
		GetWorldTimerManager().SetTimer(TimerHandle2, this, &ACAura::ActorDestroy, SpawnLife);
}

void ACAura::OnNiagaraSystemFinished(UNiagaraComponent* PSystem)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle2);
	Destroy();
}

void ACAura::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
		Hitted.AddUnique(character);
}

void ACAura::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
		Hitted.Remove(character);
}

void ACAura::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem,
	const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);

	Box->SetRelativeScale3D(Data[0].Position);

	FVector location = Box->GetScaledBoxExtent();
	location.Y = 0;

	Box->SetRelativeLocation(location);
}

void ACAura::ActorDestroy()
{
	Destroy();
}


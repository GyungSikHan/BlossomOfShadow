#include "CSmash.h"
#include "NiagaraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

ACSmash::ACSmash()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Niagara = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
	Niagara->SetupAttachment(Root);
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(Root);

	//UNiagaraSystem* niagara{};
	//ConstructorHelpers::FObjectFinder<UNiagaraSystem> asset(L"/Script/Niagara.NiagaraSystem'/Game/Effects/sA_Megapack_v1/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_AOE_Attack_1.NS_AOE_Attack_1'");
	//niagara = asset.Object;
	//Niagara->SetAsset(niagara);

}

// Called when the game starts or when spawned
void ACSmash::BeginPlay()
{
	Super::BeginPlay();
	Super::BeginPlay();
	Niagara->SetVariableObject("Mesh_Scale", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACSmash::OnNiagaraSystemFinished);

	Box->OnComponentBeginOverlap.AddDynamic(this, &ACSmash::OnComponentBeginOverlap);
	Box->OnComponentEndOverlap.AddDynamic(this, &ACSmash::OnComponentEndOverlap);

}

void ACSmash::OnNiagaraSystemFinished(UNiagaraComponent* PSystem)
{
	Destroy();
}

void ACSmash::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
		Hitted.AddUnique(character);
}

void ACSmash::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character)
		Hitted.Remove(character);
}

void ACSmash::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem,
	const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);
}



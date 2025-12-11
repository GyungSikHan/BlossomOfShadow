#include "CRainArrow.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ACRainArrow::ACRainArrow()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Niagara = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
	Niagara->SetupAttachment(Root);
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->SetupAttachment(Root);
	Projectile = this->CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");

	Projectile->ProjectileGravityScale = 0;

	Capsule->SetCapsuleRadius(20);
	Capsule->SetCapsuleHalfHeight(20);

	Projectile->InitialSpeed = 2000;
	
	UNiagaraSystem* niagara;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> asset(L"/Script/Niagara.NiagaraSystem'/Game/Effects/sA_Megapack_v1/sA_StylizedAttacksPack/FX/NiagaraSystems/NS_ArrowAttack.NS_ArrowAttack'");
	niagara = asset.Object;
	Niagara->SetAsset(niagara);
}

// Called when the game starts or when spawned
void ACRainArrow::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetVariableObject("Collision", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACRainArrow::OnSystemFinished);

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACRainArrow::OnComponentBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ACRainArrow::OnComponentEndOverlap);
}

void ACRainArrow::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data,
	UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);
}

void ACRainArrow::OnSystemFinished(UNiagaraComponent* System)
{
	Destroy();
}

void ACRainArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() == OtherActor)
		return;

	Niagara->SetVisibility(false);
	Projectile->Deactivate();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character != nullptr)
		Hitted.AddUnique(character);

	HitData.PlayEffect(GetWorld(),	GetActorLocation());

	for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);

	hit = true;
}

void ACRainArrow::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character != nullptr)
		Hitted.Remove(character);

	hit = false;
}

#include "CSkillArrow.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ACSkillArrow::ACSkillArrow()
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

void ACSkillArrow::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetVariableObject("Collision", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACSkillArrow::OnSystemFinished);

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACSkillArrow::OnComponentBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ACSkillArrow::OnComponentEndOverlap);
}

void ACSkillArrow::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem,
	const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);

	if(hit == false)
		return;

	if(CollisionEffect == nullptr)
		return;

	UFXSystemComponent* com = Cast<UFXSystemComponent>(Niagara);

	if(com == nullptr)
		return;

	com->Deactivate();



	FTransform transform = CollisionEffectTransform;
	transform.AddToTranslation(this->GetActorLocation());

	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(CollisionEffect);
	FVector location = transform.GetLocation();
	FRotator rotator = FRotator(transform.GetRotation());
	FVector scales = transform.GetScale3D();

	UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, Root, NAME_None, location, rotator, scales, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, location, rotator, scales);
}

void ACSkillArrow::OnSystemFinished(UNiagaraComponent* System)
{
	Destroy();
}

void ACSkillArrow::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GetOwner() == OtherActor)
		return;

	Projectile->Deactivate();
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character != nullptr)
		Hitted.AddUnique(character);

	for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);

	hit = true;
}

void ACSkillArrow::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character != nullptr)
		Hitted.Remove(character);

	hit = false;
}

#include "CArrow.h"
#include "RPG/Characters/AI/CEnemy_AI.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMeshSocket.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACArrow::ACArrow()
{
	Capsule = this->CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	SetRootComponent(Capsule);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Arrow");
	StaticMesh->SetupAttachment(Capsule);
	
	Projectile = this->CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");

	Projectile->ProjectileGravityScale = 0;
	Capsule->BodyInstance.bNotifyRigidBodyCollision = false;
	Capsule->SetCollisionProfileName("BlockAll");

	Capsule->SetCapsuleRadius(8);
	Capsule->SetCapsuleHalfHeight(8);

	Projectile->InitialSpeed = 2000;
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);

	Projectile->Deactivate();
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}

void ACArrow::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SetLifeSpan(10);
}

void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	//ACEnemy_AI* ai = Cast<ACEnemy_AI>(character);
	//ACEnemy_AI* ow = Cast<ACEnemy_AI>(GetOwner());
	//if (ai != nullptr && ai->GetTeamID() == ow->GetTeamID())
	//	return;

	Projectile->Deactivate();
	SetLifeSpan(LifeSpawnAfterCollision);
	for (AActor* actor : Ignores)
		if(actor == OtherActor)
			return;

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (!!character && OnHit.IsBound())
		OnHit.Broadcast(this, character);
}


#include "CMagicBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ACMagicBall::ACMagicBall()
{
	Sphere = this->CreateDefaultSubobject<USphereComponent>("Sphere");
	Particle = this->CreateDefaultSubobject<UParticleSystemComponent>("Particle");
	this->SetRootComponent(Sphere);
	Particle->SetupAttachment(Sphere);
	Projectile = this->CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");

	Projectile->ProjectileGravityScale = 0.0f;
	Projectile->InitialSpeed = 500.0f;
	Sphere->SetSphereRadius(32);

	InitialLifeSpan = 5.0f;

	HitData.Launch = 300.0f;
	HitData.Power = 8.0f;

	ConstructorHelpers::FObjectFinder<UAnimMontage> asset(L"/Script/Engine.AnimMontage'/Game/Character/Montage/HitandDead/HitReaction_Montage.HitReaction_Montage'");
	HitData.Montage = asset.Object;
}

void ACMagicBall::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACMagicBall::OnComponentBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACMagicBall::OnComponentEndOverlap);
}


void ACMagicBall::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();
}

void ACMagicBall::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character != nullptr)
		Hitted.AddUnique(character);
	if(Hitted.Num() > 0)
		SendDamage();
}
void ACMagicBall::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if(character == nullptr)
		return;

	if (character != nullptr)
		Hitted.Remove(character);

	Projectile->Deactivate();
	Destroy();
}

void ACMagicBall::SendDamage()
{
	if(Hitted.Num() <= 0)
		return;

	//for (int32 i = Hitted.Num() - 1; i >= 0; i--)
	for (int32 i = 0; i < Hitted.Num(); i++)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
}
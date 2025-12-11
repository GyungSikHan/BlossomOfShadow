#include "CAround.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ACAround::ACAround()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Particle = CreateDefaultSubobject<UParticleSystemComponent>("Particle");

	Particle->SetupAttachment(Capsule);

	Capsule->SetCapsuleHalfHeight(44);
	Capsule->SetCapsuleRadius(44);

	InitialLifeSpan = 5;

	HitData.Launch = 0;
	HitData.Power = 5;
}

// Called when the game starts or when spawned
void ACAround::BeginPlay()
{
	Super::BeginPlay();

	Angle = UKismetMathLibrary::RandomFloatInRange(0, 360);
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACAround::OnComponentBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &ACAround::OnComponentEndOverlap);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ACAround::SendDamage, DamageInteval, true);
}

// Called every frame
void ACAround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = GetOwner()->GetActorLocation();
	
	Angle += (bNegative ? -Speed : +Speed) * DeltaTime;
	if (FMath::IsNearlyEqual(Angle, bNegative ? -360 : +360))
		Angle = 0;
	
	FVector distance = FVector(Distance, 0, 0);
	FVector value = distance.RotateAngleAxis(Angle, FVector::UpVector);
	location += value;

	SetActorLocation(location);
	SetActorRotation(FRotator(0, Angle, 0));
}

void ACAround::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void ACAround::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(GetOwner() == OtherActor)
		return;

	ACharacter* other = Cast<ACharacter>(OtherActor);
	if (other != nullptr)
		Hitted.AddUnique(other);
}

void ACAround::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* other = Cast<ACharacter>(OtherActor);
	if (other != nullptr)
		Hitted.Remove(other);
}

void ACAround::SendDamage()
{
	for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
}

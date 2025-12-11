#include "CBlackHole.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Components/CapsuleComponent.h"
#include "RPG/Components/CStatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ACBlackHole::ACBlackHole()
{
 	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	DeadZone = CreateDefaultSubobject<UCapsuleComponent>("DeadZone");
	DeadZone->SetupAttachment(Root);
	Horizont = CreateDefaultSubobject<UCapsuleComponent>("Horizont");
	Horizont->SetupAttachment(DeadZone);
	Niagara1 = CreateDefaultSubobject<UNiagaraComponent>("Niagara1");
	Niagara1->SetupAttachment(Horizont);
	Niagara2 = CreateDefaultSubobject<UNiagaraComponent>("Niagara2");
	Niagara2->SetupAttachment(Horizont);
	Niagara3 = CreateDefaultSubobject<UNiagaraComponent>("Niagara3");
	Niagara3->SetupAttachment(Horizont);
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");

	Horizont->SetCapsuleRadius(100);
	Horizont->SetCapsuleHalfHeight(100);
	DeadZone->SetCapsuleRadius(150);
	DeadZone->SetCapsuleHalfHeight(150);

	UNiagaraSystem* niagara;
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> asset(L"/Script/Niagara.NiagaraSystem'/Game/Effects/Portals_VFXPack/Particles/P_AttractionEffect.P_AttractionEffect'");
	niagara = asset.Object;
	Niagara1->SetAsset(niagara);
	asset = (L"/Script/Niagara.NiagaraSystem'/Game/Effects/Portals_VFXPack/Particles/P_SphereDissolve_Effect.P_SphereDissolve_Effect'");
	niagara = asset.Object;
	Niagara2->SetAsset(niagara);
	asset = (L"/Script/Niagara.NiagaraSystem'/Game/Effects/Portals_VFXPack/Particles/P_Darkness_SpherePortal.P_Darkness_SpherePortal'");
	niagara = asset.Object;
	Niagara3->SetAsset(niagara);

	Projectile->ProjectileGravityScale = 0;
	Projectile->InitialSpeed = 100;
}

// Called when the game starts or when spawned
void ACBlackHole::BeginPlay()
{
	Super::BeginPlay();

	Niagara1->OnSystemFinished.AddDynamic(this, &ACBlackHole::OnSystemFinished);

	DeadZone->OnComponentBeginOverlap.AddDynamic(this, &ACBlackHole::OnComponentBeginOverlap);
	DeadZone->OnComponentEndOverlap.AddDynamic(this, &ACBlackHole::OnComponentEndOverlap);

	//GetWorldTimerManager().SetTimer(TimerHandle, this ,&ACBlackHole::Delegate, DamageInterval, true);
	GetWorldTimerManager().SetTimer(TimerHandle2, this ,&ACBlackHole::DestroyEffectPlay,SpawnLife);
}

// Called every frame
void ACBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> ignore;
	FVector start = GetActorLocation();
	FVector end = start;

	ignore.Add(GetOwner());

	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), start, end, Distance, "BlackHole", false, ignore, DrawDebug, hitResults, true);

	for (FHitResult hit : hitResults)
	{
		ACharacter* character = Cast<ACharacter>(hit.GetActor());
		if (character != nullptr)
		{
			if (character->GetDistanceTo(this) <= Distance)
			{
	
				FVector ends = character->GetActorLocation();
				FVector direction = (start - ends).GetSafeNormal2D();
				
				//character->LaunchCharacter(FVector(direction.X * Speed, direction.Y * Speed, direction.Z * Speed*10000), false, true);
				character->AddActorWorldOffset(direction * Speed, true);
			}
		}
	}
}

void ACBlackHole::OnSystemFinished(UNiagaraComponent* System)
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	for (ACharacter* hit : HitCharacters)
	{
		UCStatusComponent* status = Cast<UCStatusComponent>(hit->GetComponentByClass(UCStatusComponent::StaticClass()));

		if (status->GetHealth() <= 0)
			return;
		HitDatas.PlayMontage(hit);
		HitDatas.SendDamage(Cast<ACharacter>(Owner), this, hit);
	}

	Destroy();
}

void ACBlackHole::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character != nullptr)
	{
		Hitted.AddUnique(character);
		HitCharacters.Add(character);
	}

	GetWorldTimerManager().SetTimer(TimerHandle, this, &ACBlackHole::Delegate, DamageInterval, true);

	//for (int32 i = Hitted.Num() - 1; i >= 0; i--)
	//	HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
}

void ACBlackHole::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetOwner() == OtherActor)
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character != nullptr)
		Hitted.Remove(character);
}


void ACBlackHole::DestroyEffectPlay()
{
	if (CollisionEffect == nullptr)
		return;

	UFXSystemComponent* com = Cast<UFXSystemComponent>(Niagara1);
	UFXSystemComponent* com1 = Cast<UFXSystemComponent>(Niagara2);
	UFXSystemComponent* com2 = Cast<UFXSystemComponent>(Niagara3);

	if (com == nullptr)
		return;
	com->Deactivate();
	if (com1 == nullptr)
		return;
	com1->Deactivate();
	if (com2 == nullptr)
		return;
	com2->Deactivate();

	FTransform transform = CollisionEffectTransform;
	transform.AddToTranslation(this->GetActorLocation());

	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(CollisionEffect);
	FVector location = transform.GetLocation();
	FRotator rotator = FRotator(transform.GetRotation());
	FVector scales = transform.GetScale3D();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, location, rotator, scales);
}

void ACBlackHole::Delegate()
{
	for (int32 i = Hitted.Num() - 1; i >= 0; i--)
		HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, Hitted[i]);
}

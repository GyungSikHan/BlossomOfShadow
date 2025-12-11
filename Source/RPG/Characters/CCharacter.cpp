#include "Characters/CCharacter.h"

#include "CAnimInstance.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CTargetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Init/CGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Utilities/CLog.h"
#include "Weapon/CWeaponStructures.h"

ACCharacter::ACCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Component setting
	{
		Move = this->CreateDefaultSubobject<UCMovementComponent>("Movement");
		State = this->CreateDefaultSubobject<UCStateComponent>("State");
		Montage = this->CreateDefaultSubobject<UCMontagesComponent>("Montage");
		Weapon = this->CreateDefaultSubobject<UCWeaponComponent>("Weaopn");
		Status = this->CreateDefaultSubobject<UCStatusComponent>("Status");
		Target = this->CreateDefaultSubobject<UCTargetComponent>("Target");
		SceneAirAttack = this->CreateDefaultSubobject<USceneComponent>("SceneAirAttack");
		SceneAirAttack->SetupAttachment(GetCapsuleComponent());
		AirCollision = this->CreateDefaultSubobject<UArrowComponent>("AirCollision");
		AirCollision->SetupAttachment(GetCapsuleComponent());
		{
			SceneAirAttack->SetRelativeLocation(FVector(127, 0, 0));
			AirCollision->SetRelativeLocation(FVector(130, 0, 33));
			PrvAirCollisionLocation = AirCollision->GetComponentLocation();
		}
	}

	//mesh setting
	{
		USkeletalMesh* mesh{};
		ConstructorHelpers::FObjectFinder<USkeletalMesh> asset(
			L"/Script/Engine.SkeletalMesh'/Game/Character/Mannequins/Meshes/SKM_Manny.SKM_Manny'");
		mesh = asset.Object;
		GetMesh()->SetSkeletalMesh(mesh);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}

	//animinstance setting
	{
		TSubclassOf<UCAnimInstance> animInstance;
		ConstructorHelpers::FClassFinder<UCAnimInstance> animAsset(
			L"/Script/Engine.AnimBlueprint'/Game/ABP_CAnimInstance.ABP_CAnimInstance_C'");
		animInstance = animAsset.Class;
		GetMesh()->SetAnimClass(animInstance);
	}
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetRelativeLocation(FVector(0, 0, 90));
}

void ACCharacter::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	State->OnStateTypeChanged.AddDynamic(this, &ACCharacter::OnStateTypeChanged);
}

void ACCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	AirCollision->SetRelativeLocation(PrvAirCollisionLocation);
	if(bAirComboDamage == true)
	{
		bAirComboDamage = false;
		GetCapsuleComponent()->SetRelativeRotation(CapsuleRotation);
	}
}

void ACCharacter::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Hitted:
		Hitted();
		break;
	case EStateType::Dead:
		Dead();
		break;
	default:
		break;
	}
}

void ACCharacter::Hitted()
{
	Move->Stop();

	//ApplayDamage
	float damage = Damage.Power;
	//if (bDefence == true)
	//	damage *= 0.1f;
	Status->Damage(damage);
	Damage.Power = 0;

	if (Damage.Event != nullptr && Damage.Event->HitData != nullptr)// && State->PreveType() == EStateType::Idle)
	{
		FHitData* hitData = Damage.Event->HitData;
		//if (State->IsInterruptible() == false)
		{
			hitData->PlayMontage(this);

			if (State->IsDeadMode() == false)
			{
				FVector start = GetActorLocation();
				FVector target = Damage.Character->GetActorLocation();
				FVector direction = target - start;
				direction.Normalize();

				if (hitData->IsFalling == true)
				{
					FVector locaiton = Damage.Character->GetSceneComponent()->GetComponentLocation();
					AirBuffer(hitData->Launch);
					GetCapsuleComponent()->SetRelativeLocation(locaiton, false, nullptr, ETeleportType::ResetPhysics);
					CapsuleRotation = GetCapsuleComponent()->GetRelativeRotation();
					bAirComboDamage = true;
				}
				else if(hitData->IsFalling == false)
				{
					LaunchCharacter(-direction * hitData->Launch, false, false);
				}
				SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
			}
		}
		hitData->PlayHitStop(GetWorld());
		hitData->PlaySound(this);
		hitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		if (Weapon->GetEquip() == true)
			Weapon->SetHitEquip();
	}
	//bInterruptible = true;

	if (Status->IsDead() == true)
	{
		State->SetDeadMode();
		return;
	}

	if (this == nullptr)
	{
		CLog::Log("Error");
		return;
	}

	Damage.Character = nullptr;
	Damage.Causer = nullptr;
	Damage.Event = nullptr;
}

void ACCharacter::Dead()
{
	Montage->PlayDeadMode();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACCharacter::End_Hitted()
{
	IICharacter::End_Hitted();
	State->SetIdleMode();
	State->SetInterruptible(false);
}

void ACCharacter::End_Dead()
{
	IICharacter::End_Dead();
	Destroy();
}

float ACCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Damage.Power = damage;
	Damage.Character = Cast<ACCharacter>(EventInstigator->GetPawn());
	Damage.Causer = DamageCauser;
	Damage.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetHittednMode();

	return damage;
}

void ACCharacter::SetInterruptible(bool InValue)
{
	bInterruptible = InValue;
}

void ACCharacter::AirBuffer(float AirBuff)
{
	FVector start = GetActorLocation();
	FVector end = FVector(start.X, start.Y, start.Z - 1200.0f);
	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> ignores;
	ignores.Add(this);
	TArray<FHitResult> hit;
	bool result = UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), start, end, types, false, ignores, DebugType, hit, true);
	if (result == true)
		LaunchCharacter(FVector(0, 0, AirBuff), true, true);
}






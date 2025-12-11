#include "CMeteor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ACMeteor::ACMeteor()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Niagara = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
	Niagara->SetupAttachment(Root);
}

void ACMeteor::BeginPlay()
{
	Super::BeginPlay();

	Niagara->SetVariableObject("Collision", this);
	Niagara->OnSystemFinished.AddDynamic(this, &ACMeteor::OnSystemFinished);

	if(NiagaraMesh != nullptr)
	{
		FBox box = NiagaraMesh->GetBoundingBox();
		BoxExtent = (box.Min - box.Max).GetAbs() * 0.5;
	}
}

void ACMeteor::ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem,
	const FVector& SimulationPositionOffset)
{
	INiagaraParticleCallbackHandler::ReceiveParticleData_Implementation(Data, NiagaraSystem, SimulationPositionOffset);

	if(Data.Num() <= 0)
		return;

	static TArray<AActor*> ignores;
	ignores.AddUnique(GetOwner());

	static FHitResult hitResult;
	for (int32 i = Data.Num() - 1; i >= 0; i--)
	{
		FVector position = Data[i].Position;
		FVector scale = Data[i].Velocity * BoxExtent;

		UKismetSystemLibrary::BoxTraceSingleByProfile(GetWorld(), position, position, scale, NiagaraMeshRotation, "Pawn", false, ignores, DrawDebug, hitResult, true);
		if(hitResult.bBlockingHit == true)
		{
			if(CollisionEffect != nullptr)
			{
				FTransform transform = CollisionEffectTransform;
				transform.AddToTranslation(hitResult.Location);

				{
					UNiagaraSystem* niagara = Cast<UNiagaraSystem>(CollisionEffect);
					FVector location = transform.GetLocation();
					FRotator rotator = FRotator(transform.GetRotation());
					FVector scales = transform.GetScale3D();

					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), niagara, location, rotator, scales);
				}
			}

			ACharacter* character = Cast<ACharacter>(hitResult.GetActor());
			if (character != nullptr)
				HitData.SendDamage(Cast<ACharacter>(GetOwner()), this, character);
		}

	}

}

void ACMeteor::OnSystemFinished(UNiagaraComponent* System)
{
	Destroy();
}



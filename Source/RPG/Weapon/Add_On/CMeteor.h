#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraDataInterfaceExport.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CMeteor.generated.h"

UCLASS()
class RPG_API ACMeteor : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	FHitData HitData;
	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	class UStaticMesh* NiagaraMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	FRotator NiagaraMeshRotation;
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	class UFXSystemAsset* CollisionEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	FTransform CollisionEffectTransform;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* Root;
	UPROPERTY(VisibleDefaultsOnly)
	class UNiagaraComponent* Niagara;

	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

public:	
	ACMeteor();

protected:
	virtual void BeginPlay() override;

public:
	void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

private:
	UFUNCTION()
	void OnSystemFinished(UNiagaraComponent* System);

private:
	FVector BoxExtent;
};

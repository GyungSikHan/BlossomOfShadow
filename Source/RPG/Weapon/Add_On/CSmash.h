#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterfaceExport.h"
#include "GameFramework/Actor.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CSmash.generated.h"

UCLASS()
class RPG_API ACSmash : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FHitData HitData;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageInterval = 0.1;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* Niagara;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box;
public:	
	// Sets default values for this actor's properties
	ACSmash();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnNiagaraSystemFinished(UNiagaraComponent* PSystem);

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	virtual void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;

private:
	TArray<ACharacter*> Hitted;

};

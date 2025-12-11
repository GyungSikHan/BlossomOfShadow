// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterfaceExport.h"
#include "GameFramework/Actor.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CAura.generated.h"

UCLASS()
class RPG_API ACAura : public AActor, public INiagaraParticleCallbackHandler
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
	UPROPERTY(EditAnywhere)
	float SpawnLife;

public:
	ACAura();

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
	void ActorDestroy();

private:
	TArray<ACharacter*> Hitted;
	FTimerHandle TimerHandle;
	FTimerHandle TimerHandle2;
};


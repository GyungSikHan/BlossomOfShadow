#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterfaceExport.h"
#include "GameFramework/Actor.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CRainArrow.generated.h"

UCLASS()
class RPG_API ACRainArrow : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	   class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	   class UNiagaraComponent* Niagara;
	UPROPERTY(VisibleAnywhere)
	   class UCapsuleComponent* Capsule;
	UPROPERTY(VisibleDefaultsOnly)
	   class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	   FHitData HitData;
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	   class UFXSystemAsset* CollisionEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	   FTransform CollisionEffectTransform;

public:	
	ACRainArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void ReceiveParticleData_Implementation(const TArray<FBasicParticleData>& Data, UNiagaraSystem* NiagaraSystem, const FVector& SimulationPositionOffset) override;


private:
	UFUNCTION()
	void OnSystemFinished(UNiagaraComponent* System);
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	TArray<ACharacter*> Hitted;
	FTimerHandle TimerHandle;
	bool hit;


};
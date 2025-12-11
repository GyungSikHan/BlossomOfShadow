#pragma once

#include "CoreMinimal.h"
#include "NiagaraDataInterfaceExport.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CBlackHole.generated.h"

UCLASS()
class RPG_API ACBlackHole : public AActor, public INiagaraParticleCallbackHandler
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float SpawnLife;
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* Niagara1;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* Niagara2;
	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* Niagara3;
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* DeadZone;
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Horizont;	
	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* Projectile;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FHitData HitData;
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageInterval = 0.1;

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	class UFXSystemAsset* CollisionEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	FTransform CollisionEffectTransform;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float Distance = 500;
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float Speed = 20;
	UPROPERTY(EditAnywhere, Category = "Setting")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Setting")
	FHitData HitDatas;


public:	
	// Sets default values for this actor's properties
	ACBlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnSystemFinished(UNiagaraComponent* System);
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void DestroyEffectPlay();
private:
	TArray<ACharacter*> Hitted;
	FTimerHandle TimerHandle;
	FTimerHandle TimerHandle2;
	TArray<ACharacter*> HitCharacters;

	void Delegate();
};



#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CMagicBall.generated.h"

UCLASS()
class RPG_API ACMagicBall : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FHitData HitData;

	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* Sphere;
	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystemComponent* Particle;
	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* Projectile;

public:	
	ACMagicBall();

protected:
	virtual void BeginPlay() override;

public:
	void Shoot(const FVector& InForward);

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void SendDamage();

private:
	TArray<class ACharacter*> Hitted;
	TArray<AActor*> Ignores;
};



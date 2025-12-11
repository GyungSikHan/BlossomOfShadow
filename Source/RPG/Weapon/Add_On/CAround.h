#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CAround.generated.h"

UCLASS()
class RPG_API ACAround : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FHitData HitData;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float Speed = 300;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float Distance = 300;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	bool bNegative;
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float DamageInteval = 0.1f;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class UCapsuleComponent* Capsule;
	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystemComponent* Particle;

public:
	ACAround();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
public:
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void SendDamage();

private:
	float Angle;

	TArray<ACharacter*> Hitted;
	FTimerHandle TimerHandle;
};

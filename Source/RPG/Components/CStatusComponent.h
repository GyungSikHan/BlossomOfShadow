#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Health")
		float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, Category = "MP")
		float MaxMP = 100.0f;
		UPROPERTY(EditAnywhere, Category = "Stamina")
		float MaxStamina = 100.0f;
public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE bool IsDead() { return Health <= 0.0f; }

	FORCEINLINE float GetMaxMP() { return MaxMP; }
	FORCEINLINE float GetMP() { return MP; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }
	FORCEINLINE float GetStamina() { return Stamina; }

public:	
	UCStatusComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetHP(float InAmount);

	void Damage(float InAmount);
	void UseMp(float InAmount);
	void UseStamina(float InAmount);

	void RecorveryMaxHP(float InAmount);
	void RecorveryMp(float InAmount);
	void RecorveryStamina(float InAmount);
private:
	ACharacter* OwnerCharacter;
	float Health;
	float MP;
	float Stamina;

};



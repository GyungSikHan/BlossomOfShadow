// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Setting")
	float TraceDistance = 1000;
	UPROPERTY(EditAnywhere, Category = "Setting")
	float FinishAngle = 2;
	UPROPERTY(EditAnywhere, Category = "Setting")
	float InterpSpeed = 5;
	UPROPERTY(EditAnywhere, Category = "Setting")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;
	UPROPERTY(EditAnywhere, Category = "Setting")
	UParticleSystem* ParticleAsset;
	UPROPERTY(EditAnywhere, Category = "Setting")
	FVector ParticleLocation = FVector(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, Category = "Setting")
	FVector ParticleScael = FVector(1.0f, 1.0f, 1.0f);

public:
	FORCEINLINE bool GetTargetting() { return bTargetting; }
	FORCEINLINE ACharacter* GetTarget() { return  Target; }
public:
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Toggle_Target();
	void ChangeFocus(const struct FInputActionValue& Value);
	void EndTarget();

private:
	void BeginTarget();
	void TickTarget();
	void ChangeTarget(ACharacter* InCandidate);

private:
	ACharacter* Character;
	ACharacter* Target;
	UParticleSystemComponent* Particle;
	bool bMovingFocus;
	bool bTargetting;
		
};

#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "CSkills_Sword02.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSkills_Sword02 : public UCSkills
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float Distance = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float Speed = 200;
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FHitData HitData;

public:
	virtual void Skill_Pressed() override;
	virtual void Begin_Skill_Implementation() override;
	virtual void Tick_Implementation(float InDeltaTime) override;
	virtual void End_Skill_Implementation() override;
private:
	UFUNCTION()
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther);

private:
	bool bMoving;

	FVector Start;
	FVector End;

	TArray<class ACharacter*> Overlapped;
	TArray<class ACharacter*> Hitted;

};

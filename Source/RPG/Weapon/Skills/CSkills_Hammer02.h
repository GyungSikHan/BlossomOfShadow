#pragma once

#include "CoreMinimal.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "CSkills_Hammer02.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RPG_API UCSkills_Hammer02 : public UCSkills
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "Aura")
	TSubclassOf<class ACAura> AuraClass;
	UPROPERTY(EditDefaultsOnly, Category = "Aura")
	FVector AuraLocation;
	UPROPERTY(EditAnywhere, Category = "Setting")
	float TraceDistance = 1000;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float Speed = 1000;
	UPROPERTY(EditAnywhere, Category = "Setting")

	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;
public:
	virtual void Skill_Pressed() override;
	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;

private:
	class UCTargetComponent* Target;
	bool bMoving;

	FVector Start;
	FVector End;
};

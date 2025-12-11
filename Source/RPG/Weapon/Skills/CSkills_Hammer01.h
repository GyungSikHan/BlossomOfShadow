#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "CSkills_Hammer01.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSkills_Hammer01 : public UCSkills
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "Aura")
	TSubclassOf<AActor> ActorClass;
	UPROPERTY(EditDefaultsOnly, Category = "Aura")
	FVector ActorLocation;

public:
	virtual void Skill_Pressed() override;
	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;
};

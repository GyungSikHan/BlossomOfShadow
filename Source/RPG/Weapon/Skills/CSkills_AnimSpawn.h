#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "CSkills_AnimSpawn.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSkills_AnimSpawn : public UCSkills
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "AActor")
	TArray<TSubclassOf<AActor>> ActorClass;

public:
	virtual void Skill_Pressed() override;

	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;
};

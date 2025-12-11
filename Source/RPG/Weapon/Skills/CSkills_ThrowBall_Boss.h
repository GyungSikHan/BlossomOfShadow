#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/Skills/CSkills.h"
#include "CSkills_ThrowBall_Boss.generated.h"

UCLASS(Blueprintable)
class RPG_API UCSkills_ThrowBall_Boss : public UCSkills
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "MagicBall")
	TSubclassOf<class ACMagicBall> MagicBallClass;

public:
	virtual void Skill_Pressed() override;
	virtual void Begin_Skill_Implementation() override;
	virtual void End_Skill_Implementation() override;

private:
	ACMagicBall* MagicBall;

};

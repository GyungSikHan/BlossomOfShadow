#pragma once

#include "CoreMinimal.h"
#include "RPG/Weapon/CDoAction.h"
#include "CDoAction_Magic.generated.h"

UCLASS(Blueprintable)
class RPG_API UCDoAction_Magic : public UCDoAction
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "MagicBall")
	TSubclassOf<class ACMagicBall> MagicBallClass;

public:
	UFUNCTION() void EnableCombo() { bEnable = true; }
	UFUNCTION() void DisableCombo() { bEnable = false; }

public:
	UCDoAction_Magic();
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;


private:
	ACMagicBall* MagicBall;
	bool bEnable;
	bool bExsit;
};

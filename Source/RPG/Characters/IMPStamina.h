#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IMPStamina.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIMPStamina : public UInterface
{
	GENERATED_BODY()
};

class RPG_API IIMPStamina
{
	GENERATED_BODY()
private:
	float InAmout;
public:
	virtual float GetMp() { return InAmout; };
	virtual float GetStamina() { return InAmout; };
	virtual void UseMp(float InMP) {};
	virtual void UseStamina(float INStamina) {};
};

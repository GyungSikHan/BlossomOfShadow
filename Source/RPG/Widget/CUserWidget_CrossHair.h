#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

UCLASS()
class RPG_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Color")
	void CrossHair_White();
	UFUNCTION(BlueprintImplementableEvent, Category = "Color")
	void CrossHair_Red();
};

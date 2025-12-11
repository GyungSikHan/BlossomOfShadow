#pragma once

#include "CoreMinimal.h"
#include "RPG/Characters/IMPStamina.h"
#include "RPG/Weapon/CWeaponStructures.h"
#include "CSkills.generated.h"

UCLASS()
class RPG_API UCSkills : public UObject
{
	GENERATED_BODY()
public:
	bool GetInAction() { return bInAction; }
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FDoActionData> ActionDatas;

public:
	UCSkills();

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttaachment, class UCDoAction* InDoAction);

public:
	virtual void Skill_Pressed();
	virtual void Skill_Released();

public:
	UFUNCTION(BlueprintNativeEvent)
	void Begin_Skill();
	virtual void Begin_Skill_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void End_Skill();
	virtual void End_Skill_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}

protected:
	bool bInAction;

	ACharacter* Owner;
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;

	IIMPStamina* Character;

	int32 Index;
};

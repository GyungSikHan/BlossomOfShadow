#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.h"
#include "Animation/AnimMontage.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "CMontagesComponent.generated.h"

USTRUCT()
struct FMontagesData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		EStateType Type = EStateType::Max;
	UPROPERTY(EditAnywhere)
		UAnimMontage* Montage{};
	UPROPERTY(EditAnywhere)
		float PlayRate = 1.0f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "DataTable")
		UDataTable* DataTable{};

public:
	UCMontagesComponent();

protected:
	virtual void BeginPlay() override;

public:	
	void PlayBackStepMode();
	void PlayRollMode(int32 InIndex);
	void PlayHitDefenceMode();
	void PlayHitLandMode();
	void PlayKoandGetup();
	void PlayDeadMode();

private:
	void PlayAnimMontage(EStateType InType);

private:
	ACharacter* OwnerCharacter;
	FMontagesData* Datas[(int32)EStateType::Max];
};

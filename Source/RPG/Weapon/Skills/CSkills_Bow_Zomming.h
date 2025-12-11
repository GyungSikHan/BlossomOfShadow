#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CSkills.h"
#include "CSkills_Bow_Zomming.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag{};

	UPROPERTY(EditAnywhere)
	FVector CameraLocation = FVector::ZeroVector;
};

UCLASS()
class RPG_API UCSkills_Bow_Zomming : public UCSkills
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
	class UCurveVector* Curve;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	FAimData AimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	float AimingSpeed = 200;

public:
	UCSkills_Bow_Zomming();

public:
	virtual void Skill_Pressed();
	virtual void Skill_Released();

public:
	UFUNCTION(BlueprintCallable)
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;

public:
	virtual void Tick_Implementation(float InDeltaTime) override;

private:
	UFUNCTION()
	void OnAiming(FVector Output);

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	FTimeline Timeline;

private:
	FAimData OriginData;

private:
	float* Bend;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;
	bool bAniming;
private:
	TSubclassOf<class UUserWidget> CrossHairClass;
	class UCUserWidget_CrossHair* CrossHair;
};

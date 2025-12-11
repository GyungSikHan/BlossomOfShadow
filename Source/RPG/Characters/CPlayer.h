// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CCharacter.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "IMPStamina.h"
#include "InputActionValue.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Components/CParkourComponent.h"
#include "RPG/Components/CStatusComponent.h"
#include "RPG/Components/CWeaponComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class RPG_API ACPlayer : public ACCharacter
{
	GENERATED_BODY()
public:
	FORCEINLINE	APlayerController* GetPlayerController() { return Controller; }
	FORCEINLINE class UCUserWidget_Menu* GetMeun() { return WBMeun; }
	FORCEINLINE	class UCameraComponent* GetFollwCamera() { return Camera; }
	FORCEINLINE	class UCUserWidget_PlayerInterface* GetUI() { return LabelWidget; }

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	class UCUserWidget_PlayerInterface* LabelWidget;

	//Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Sprint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Jumping;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Avoid;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Sword;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Hammer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Rod;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Bow;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Warp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Zooming;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Targetting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* ChangeTargetting;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Skill1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Skill2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		UInputAction* Menu;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Pause;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ControllUI;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

private:
	UPROPERTY(VisibleAnywhere)
	class UCFeetComponent* Feet;
	UPROPERTY(VisibleAnywhere)
	class UCZoomComponent* Zoom;
	UPROPERTY(VisibleAnywhere)
	class UCParkourComponent* Parkour;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* ArrowGroup;
	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ParkourArrows[(int32)EParkourArrowType::Max];

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "UI")
	TSubclassOf<class UCUserWidget_GameplayWidget> MenuWidgetClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ControllUIClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UI")
	TSubclassOf<UCUserWidget_Menu> WBMeunClass;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UCUserWidget_Result> ResultWidgetClass;


private:
	UPROPERTY(VisibleDefaultsOnly)
	class UCUserWidget_GameplayWidget* MenuWidget;
	UPROPERTY()
	class UCUserWidget_Result* ResultWidget;
public:
	UPROPERTY(BlueprintReadOnly)
	UCUserWidget_Menu* WBMeun;
	UPROPERTY(BlueprintReadOnly)
	UUserWidget* UIControll;


public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(TeamID); }

private:
	UFUNCTION(BlueprintCallable)
	void TestDead();

public:
	UFUNCTION(BlueprintCallable)
	void ShowResultUI(bool Result);

protected:
	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

protected:
	virtual void End_Hitted() override;
	virtual void End_Dead() override;

public:
	void SaveHP();
	void Restore();
	void MenuHide();
	void ChangeInputMode(bool bGameMode = true);

private:
	void OnAvoid();
	void BackStep();
	void Roll();
	void SetZooming(const FInputActionValue& Value);
	
	void OnGamePause();

	void OnSkillButton(ESkillsType InSkillType);
	void Combo(ESkillsType InSkills = ESkillsType::Max);

	void ChangeTarget();
	void RecoveryMPStamina();
	void MakeSound();

	void VisibleMenu();

	void VisibleControllMenu();
	void HidenUI();


public:
	virtual void End_BackStep() override;
	virtual void End_Roll() override;

public:
	virtual float GetMp() { return Status->GetMP(); };
	virtual float GetStamina() { return Status->GetStamina(); };
	virtual void UseMp(float InMP) override;
	virtual void UseStamina(float INStamina) override;
	virtual void Landed(const FHitResult& Hit) override;
protected:
	virtual void Hitted() override;
	virtual void Dead() override;
private:
	bool bRotation;
	bool bNoise;
	bool bRoll;

	float Axis;

	FTimerHandle Timer;

	APlayerController* Controller;
	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};





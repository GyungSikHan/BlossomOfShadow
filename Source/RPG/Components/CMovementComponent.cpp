#include "CMovementComponent.h"
#include "RPG/Utilities/CLog.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementComponent::UCMovementComponent()
{
}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnJump()
{
	OwnerCharacter->Jump();
}

void UCMovementComponent::OnEndJump()
{
	OwnerCharacter->StopJumping();
}

void UCMovementComponent::EnableControlRotation()
{
	bControlRotation = true;
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	bControlRotation = false;
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::OnMove(const FInputActionValue& Value)
{
	if (bCanMove == false)
		return;
	Values = Value;
	FVector2D MovementVector = Value.Get<FVector2D>();

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector ForwardDirection = FQuat(rotator).GetForwardVector();
	FVector Rightdirection = FQuat(rotator).GetRightVector();
	
	OwnerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	OwnerCharacter->AddMovementInput(Rightdirection, MovementVector.X);
}

//void UCMovementComponent::OnHorizontalLook(float InAxis)
void UCMovementComponent::OnLook(const FInputActionValue& Value)
{
	if (bFixedCamera == true)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	OwnerCharacter->AddControllerYawInput(MovementVector.X);
	OwnerCharacter->AddControllerPitchInput(-MovementVector.Y);

	//OwnerCharacter->AddControllerYawInput(InAxis.X * GetWorld()->GetDeltaSeconds() * HorizontalLook);
	//OwnerCharacter->AddControllerPitchInput(InAxis.Y * GetWorld()->GetDeltaSeconds() * VerticalLook);
}
#include "CZoomComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCZoomComponent::UCZoomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	SpringArm = Cast<USpringArmComponent>(GetOwner()->GetComponentByClass(USpringArmComponent::StaticClass()));
	if(SpringArm == nullptr)
		return;

	CurrentValue = SpringArm->TargetArmLength;
}

void UCZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(SpringArm == nullptr)
		return;
	if(UKismetMathLibrary::NearlyEqual_FloatFloat(SpringArm->TargetArmLength, CurrentValue) == true)
		return;
	SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, CurrentValue, DeltaTime, InterpSpeed);
}

void UCZoomComponent::SetZoomValue(float InValue)
{
	CurrentValue += (Speed * InValue);
	CurrentValue = FMath::Clamp(CurrentValue, Range.X, Range.Y);
}


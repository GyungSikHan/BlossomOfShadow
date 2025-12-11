#include "CBF_NearlyAngle.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

ACharacter* UCBF_NearlyAngle::GetNearlyFrontAngle(ACharacter* InCharacter, TArray<ACharacter*> InArray)
{
	float angle{};
	ACharacter* candidate{};

	for (ACharacter* array : InArray)
	{
		FVector location = array->GetActorLocation() - InCharacter->GetActorLocation();
		FVector forwardVector = UKismetMathLibrary::GetForwardVector(InCharacter->GetControlRotation());
		float dot = UKismetMathLibrary::Dot_VectorVector(location.GetSafeNormal2D(), forwardVector);

		if(dot >= angle)
		{
			angle = dot;
			candidate = array;
		}
	}

	return candidate;
}

#include "CPatrolPath.h"
#include "Components/SplineComponent.h"
#include "Components/TextRenderComponent.h"

ACPatrolPath::ACPatrolPath()
{
	//bRunConstructionScriptOnDrag = false;

	Root =  this->CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Spline =  this->CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(Root);
	Text = this->CreateDefaultSubobject<UTextRenderComponent>("Text");
	Text->SetupAttachment(Root);

	Spline->SetRelativeLocation(FVector(0, 0, 30));
	Spline->bHiddenInGame = false;

	Text->SetRelativeLocation(FVector(0, 0, 120));
	Text->SetRelativeRotation(FRotator(0, 180, 0));
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	Text->TextRenderColor = FColor::Red;
	Text->bHiddenInGame = true;
}

void ACPatrolPath::BeginPlay()
{
	Super::BeginPlay();

}

void ACPatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACPatrolPath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if(Text != nullptr)
		Text->SetText(FText::FromString(GetActorLabel()));
#endif

	Spline->SetClosedLoop(bLoop);
}

FVector ACPatrolPath::GetMoveTo()
{
	return Spline->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);
}

void ACPatrolPath::UpdateIndex()
{
	int32 count = Spline->GetNumberOfSplinePoints();

	if (bReverse)
	{
		if (Index > 0)
		{
			Index--;
			return;
		}

		if (Spline->IsClosedLoop())
		{
			Index = count - 1;
			return;;
		}
		Index = 1;
		bReverse = false;

		return;
	}

	if (Index < count - 1)
	{
		Index++;
		return;
	}

	if (Spline->IsClosedLoop())
	{
		Index = 0;
		return;
	}

	Index = count - 2;
	bReverse = true;
}


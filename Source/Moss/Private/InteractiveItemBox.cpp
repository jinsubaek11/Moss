#include "InteractiveItemBox.h"
#include "Helper.h"


AInteractiveItemBox::AInteractiveItemBox()
{
	PrimaryActorTick.bCanEverTick = true;

	type = EItemType::BOX;

	ConstructorHelpers::FObjectFinder<UMaterialInterface> ogMat(TEXT("/Script/Engine.Material'/Game/VR/Material/M_InteractiveBox.M_InteractiveBox'"));
	if (ogMat.Succeeded())
	{
		originalMat = ogMat.Object;
		meshComp->SetMaterial(0, ogMat.Object);
	}
}

void AInteractiveItemBox::BeginPlay()
{
	Super::BeginPlay();

	dynamicMat = UMaterialInstanceDynamic::Create(originalMat, this);
	for (int i = 0; i < meshComp->GetMaterials().Num(); i++)
	{
		meshComp->SetMaterial(i, dynamicMat);
	}
}

void AInteractiveItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isReadyToInteract && !isHighlight)
	{		
		BeforeInteract();
	}
}

void AInteractiveItemBox::BeforeInteract()
{
	isHighlight = true;
	SetHighlight();
}

void AInteractiveItemBox::Interact(FVector start, FVector end)
{
	FVector dir = (end - start).GetSafeNormal();
	float dist = FVector::Dist(start, end);

	startPos = GetActorLocation();
	endPos = startPos + dir * dist;

	InteractMove();
}

void AInteractiveItemBox::AfterInteract()
{
	isHighlight = false;
	SetHighlight();
}

void AInteractiveItemBox::InteractMove()
{
	if (moveTime >= moveCoolTime)
	{
		AfterInteract();

		moveTime = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(moveTimer);
		
		return;
	}

	moveTime += GetWorld()->GetDeltaSeconds();

	FVector pos = FMath::Lerp<FVector>(startPos, endPos, moveTime / moveCoolTime);
	SetActorLocation(pos);

	GetWorld()->GetTimerManager().SetTimer(moveTimer, this, &AInteractiveItemBox::InteractMove, 0.02);
}

void AInteractiveItemBox::SetHighlight()
{
	if (highlightTime >= highlightCoolTime)
	{
		highlightTime = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(highlightTimer);

		return;
	}

	highlightTime += GetWorld()->GetDeltaSeconds();

	float brightness;

	if (isHighlight)
	{
		brightness = FMath::Lerp<float>(defaultBrightness, modifiedBrightness, highlightTime / highlightCoolTime);
	}
	else
	{
		brightness = FMath::Lerp<float>(modifiedBrightness, defaultBrightness, highlightTime / highlightCoolTime);
	}

	//dynamicMat->SetVector
	dynamicMat->SetScalarParameterValue(TEXT("Brightness"), brightness);
	GetWorld()->GetTimerManager().SetTimer(highlightTimer, this, &AInteractiveItemBox::SetHighlight, 0.02);
}


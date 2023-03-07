#include "InteractiveItemBox.h"
#include "Helper.h"


AInteractiveItemBox::AInteractiveItemBox()
{
	type = EItemType::BOX;
}

void AInteractiveItemBox::BeginPlay()
{
	Super::BeginPlay();

	startOrigin = GetActorLocation();
}

void AInteractiveItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (isActive)
	//{
	//	InteractMove();
	//}
}

void AInteractiveItemBox::Interact(FVector2D mouseStart, FVector2D mouseEnd)
{
	//isActive = true;

	//float deltaX = mouseEnd.X - mouseStart.X;
	//float deltaY = mouseEnd.Y - mouseStart.Y;

	//if (abs(deltaX) > abs(deltaY))
	//{
	//	if (deltaX > 0)
	//	{
	//		moveDirection = FVector(0, 1, 0);
	//	}
	//	else
	//	{
	//		moveDirection = FVector(0, -1, 0);
	//	}

	//	moveDist = abs(deltaX);
	//}
	//else
	//{
	//	if (deltaY > 0)
	//	{
	//		moveDirection = FVector(-1, 0, 0);
	//	}
	//	else
	//	{
	//		moveDirection = FVector(1, 0, 0);
	//	}

	//	moveDist = abs(deltaY);
	//}

	//UE_LOG(LogTemp, Warning, TEXT("Interact"));
}

void AInteractiveItemBox::InteractMove()
{
	//if (moveTime >= moveCoolTime)
	//{
	//	startOrigin = GetActorLocation();
	//	isActive = false;
	//	moveTime = 0.f;
	//}

	//moveTime += GetWorld()->GetDeltaSeconds();

	//FVector start = startOrigin;
	//FVector end = start + moveDirection * moveDist;
	//FVector curPos = FMath::Lerp<FVector>(start, end, moveTime / moveCoolTime);
	//SetActorLocation(curPos);

	//UE_LOG(LogTemp, Warning, TEXT("InteractMove"));
}



#include "InteractiveItemBridge.h"
#include "InteractiveItemStone.h"


AInteractiveItemBridge::AInteractiveItemBridge()
{
	type = EItemType::BRIDGE;
}

void AInteractiveItemBridge::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 5; i++)
	{
		AInteractiveItemStone* stone = GetWorld()->SpawnActor<AInteractiveItemStone>(stoneFactory, GetActorLocation(), FRotator(0, 90, 0));
		if (i > 0)
		{
			stone->SetHidden(true);
		}
		
		stones.Emplace(stone);
	}
}

void AInteractiveItemBridge::BeforeInteract()
{

}

void AInteractiveItemBridge::Interact(FVector start, FVector end)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]() -> void {
		if (currentStoneIndex >= stones.Num())
		{
			GetWorldTimerManager().ClearTimer(bridgeTimer);
			return;
		}

		if (currentStoneIndex-1 >= 0 && stones[currentStoneIndex-1])
		{
			FVector nextPos = stones[currentStoneIndex - 1]->GetNextPosition();
			stones[currentStoneIndex]->SetActorLocation(nextPos);
			stones[currentStoneIndex]->SetOrigin(nextPos);
			stones[currentStoneIndex]->SetHidden(false);
		}

		if (currentStoneIndex < stones.Num() - 1)
		{
			stones[currentStoneIndex++]->SetActiveSmoothStone();
		}
	});

	GetWorldTimerManager().SetTimer(bridgeTimer, timerDelegate, 0.7f, true);
}

void AInteractiveItemBridge::AfterInteract()
{

}

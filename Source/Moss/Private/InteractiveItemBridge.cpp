#include "InteractiveItemBridge.h"
#include "InteractiveItemStone.h"


AInteractiveItemBridge::AInteractiveItemBridge()
{
	type = EItemType::BRIDGE;

	ConstructorHelpers::FObjectFinder<UStaticMesh> stoneMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (stoneMesh.Succeeded())
	{
		meshComp->SetStaticMesh(stoneMesh.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> tempBridgeMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/VR/Material/M_InteractiveBridgeInst.M_InteractiveBridgeInst'"));
	if (tempBridgeMat.Succeeded())
	{
		originalMat = tempBridgeMat.Object;
		meshComp->SetMaterial(0, originalMat);
	}
}

void AInteractiveItemBridge::BeginPlay()
{
	Super::BeginPlay();

	dynamicMat = UMaterialInstanceDynamic::Create(originalMat, this);
	for (int i = 0; i < meshComp->GetMaterials().Num(); i++)
	{
		meshComp->SetMaterial(i, dynamicMat);
	}

	origin = GetActorLocation();
	endPos = origin + GetActorForwardVector() * 800;

	dynamicMat->SetScalarParameterValue(TEXT("Smooth"), 200);
}

void AInteractiveItemBridge::BeforeInteract()
{

}

void AInteractiveItemBridge::Interact(FVector start, FVector end)
{
	AInteractiveItemStone* stone = GetWorld()->SpawnActor<AInteractiveItemStone>(stoneFactory, GetActorLocation(), FRotator(0, 90, 0));
	stones.Emplace(stone);

	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]() -> void {
		if (bridgeTime > bridgeMaxTime)
		{
			GetWorldTimerManager().ClearTimer(bridgeTimer);
			return;
		}
		
		bridgeTime += GetWorld()->GetDeltaSeconds();

		FVector curPos = FMath::Lerp<FVector>(origin, endPos, bridgeTime / bridgeMaxTime);

		if (bridgePoint <= (bridgeTime / bridgeMaxTime))
		{	
			if (!isMade)
			{
				AInteractiveItemStone* stone = GetWorld()->SpawnActor<AInteractiveItemStone>(stoneFactory, GetActorLocation(), FRotator(0, 90, 0));
				stones.Emplace(stone);
				isMade = true;
				bridgePoint += bridgeWeight;
			}
			else
			{
				isMade = false;
			}
		}

		SetActorLocation(curPos);

		//if (currentStoneIndex >= stones.Num())
		//{
		//	GetWorldTimerManager().ClearTimer(bridgeTimer);
		//	return;
		//}

		//if (currentStoneIndex-1 >= 0 && stones[currentStoneIndex-1])
		//{
		//	FVector nextPos = stones[currentStoneIndex - 1]->GetNextPosition();
		//	stones[currentStoneIndex]->SetActorLocation(nextPos);
		//	stones[currentStoneIndex]->SetOrigin(nextPos);
		//	stones[currentStoneIndex]->SetHidden(false);
		//}

		//if (currentStoneIndex < stones.Num() - 1)
		//{
		//	stones[currentStoneIndex++]->SetActiveSmoothStone();
		//}
	});

	GetWorldTimerManager().SetTimer(bridgeTimer, timerDelegate, 0.02f, true);
}

void AInteractiveItemBridge::AfterInteract()
{

}

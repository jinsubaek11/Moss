#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "InteractiveItemBridge.generated.h"

UCLASS()
class MOSS_API AInteractiveItemBridge : public AInteractiveItem
{
	GENERATED_BODY()

public:
	AInteractiveItemBridge();

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeforeInteract() override;
	virtual void Interact(FVector start = FVector(0), FVector end = FVector(0)) override;
	virtual void AfterInteract() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AInteractiveItemStone> stoneFactory;
	UPROPERTY()
	TArray<class AInteractiveItemStone*> stones;

	FTimerHandle bridgeTimer;
	float bridgeTime;
	float bridgeMaxTime = 3.f;
	float bridgePoint = 0.25f;
	float bridgeWeight = 0.25f;
	bool isMade = false;

	FVector origin;
	FVector endPos;

	int currentStoneIndex;
};

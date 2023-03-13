#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "InteractiveItemBox.generated.h"


UCLASS()
class MOSS_API AInteractiveItemBox : public AInteractiveItem
{
	GENERATED_BODY()
	
public:
	AInteractiveItemBox();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual void BeforeInteract();
	virtual void Interact(FVector start = FVector(0), FVector end = FVector(0)) override;
	virtual void AfterInteract();

private:
	void InteractMove();
	void SetHighlight();

private:
	FVector startPos;
	FVector endPos;

	FTimerHandle highlightTimer;
	float highlightTime;
	float highlightCoolTime = 0.5f;
	
	bool isHighlight = false;
	float defaultBrightness = 0.f;
	float modifiedBrightness = 1.f;

	FTimerHandle moveTimer;
	float moveTime;
	float moveCoolTime = 1.f;
};

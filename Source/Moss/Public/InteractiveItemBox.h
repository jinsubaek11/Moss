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
	virtual void Interact(
		FVector2D mouseStart = FVector2D(0), 
		FVector2D mouseEnd = FVector2D(0)) override;

private:
	void InteractMove();

private:
	FVector startOrigin;
	FVector moveDirection;
	float moveDist;
	float moveTime;
	float moveCoolTime = 1.f;
};

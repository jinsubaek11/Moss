#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "InteractiveItemStone.generated.h"


UCLASS()
class MOSS_API AInteractiveItemStone : public AInteractiveItem
{
	GENERATED_BODY()
	
public:
	AInteractiveItemStone();

protected:
	virtual void BeginPlay() override;

public:
	//void SetActiveSmoothStone();
	//void SetOrigin(FVector loc);
	//FVector GetNextPosition() const;

private:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* hardMeshComp;
	UPROPERTY()
	class UMaterialInterface* hardStoneMat;

	FTimerHandle stoneMoveTimer;
	float moveTime = 0.f;
	float moveCoolTime = 0.7f;

	float smoothTime = 0.f;
	float smoothCoolTime = 1.f;

	FVector origin;
};

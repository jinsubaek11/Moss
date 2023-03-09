#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.generated.h"

UENUM()
enum class EItemType : uint8
{
	BOX,
};

UCLASS()
class MOSS_API AInteractiveItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractiveItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Interact(
		FVector2D mouseStart = FVector2D(0),
		FVector2D mouseEnd = FVector2D(0)) {};	
	void SetIsActive(bool value);
	EItemType GetType() const;

protected:
	UPROPERTY(EditDefaultsOnly, category="item")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, category = "item")
	class UStaticMeshComponent* meshComp;

	bool isActive = false;
	EItemType type;

};

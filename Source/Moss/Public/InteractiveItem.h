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
	virtual void BeforeInteract() {};
	virtual void Interact(FVector start = FVector(0), FVector end = FVector(0)) {};	
	virtual void AfterInteract() {};
	
	void SetIsReadyToInteract(bool value);
	EItemType GetType() const;

protected:
	UPROPERTY(EditDefaultsOnly, category="item")
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, category = "item")
	class UStaticMeshComponent* meshComp;
	UPROPERTY()
	class UMaterialInterface* originalMat;
	UPROPERTY()
	class UMaterialInstanceDynamic* dynamicMat;
	
	bool isReadyToInteract = false;
	EItemType type;
};

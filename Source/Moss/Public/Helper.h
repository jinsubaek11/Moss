#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.h"
#include "Helper.generated.h"

UENUM()
enum class EMouseType : uint8
{
	START,
	END,
};

UCLASS()
class MOSS_API AHelper : public AActor
{
	GENERATED_BODY()
	
public:	
	AHelper();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetActivate(bool value);
	void SetIsForwardMove(bool value);
	void SetIsInteractToItem(bool value);
	void SetIsReadyToInteract(bool value);
	bool GetIsActivate() const;
	void SetMousePos(FVector2D pos, EMouseType type);

private:
	void Move();
	void ForwardMove();
	void MoveLerp();
	void ReadyToMove();
	void BackToMove();
	void InteractToItem();
	void FollowMainCharacter();
	void DrawDistanceLine();
	TArray<FVector> GetPlayerViewTracePoint(float scale) const;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* sphereComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* coreMeshComp;
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* shieldMeshComp;

	UPROPERTY()
	class AMainCharacter* mainCharacter;
	UPROPERTY()
	class AFirstLevelPlayer* player;

	bool isActivate = false;
	bool isMoveToReady = true;
	bool isMove = false;
	bool isBackToMove = false;
	bool isForwardMove = false;
	bool isCollide = false;
	bool isInteractToItem = false;
	bool isReadyToInteract = false;
	bool isOverlapping = false;

	float normalRange = 500.f;
	float collisionRange = 1300.f;

	float moveTime = 0.f;
	float moveCoolTime = 0.3f;
	float overlappingTime = 0.f;

	FVector2D mouseStart;
	FVector2D mouseEnd;
	FVector mouseDir;

	AInteractiveItem* currentItem;

	FVector stopPos;
};

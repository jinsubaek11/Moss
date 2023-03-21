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
	void SetisInteractToTarget(bool value);
	void SetIsReadyToInteract(bool value);
	bool GetIsActivate() const;

private:
	void Move();
	void ForwardMove();
	void MoveLerp();
	void ReadyToMove();
	void BackToMove();
	void InteractToItem();
	void InteractToEnemy();
	void FollowMainCharacter();
	void ClearLine();
	void SetShieldScaleSmall(bool isSmall);
	void DrawDistanceLine(FVector& start, FVector& end);
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
	class UNiagaraComponent* lineComp;
	UPROPERTY()
	class UMaterialInterface* originalShieldMaterial;
	UPROPERTY()
	class UMaterialInstanceDynamic* dynamicShieldMaterial;

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
	bool isInteractToTarget = false;
	bool isReadyToInteract = false;
	bool isOverlapping = false;

	float normalRange = 500.f;
	float collisionRange = 3000.f;

	float moveTime = 0.f;
	float moveCoolTime = 0.3f;
	float overlappingTime = 0.f;

	class AActor* interactTarget;
	//class AInteractiveItem* currentItem;
	//class AEnemy* currentEnemy;

	FVector interactStartPos;
	FVector interactEndPos;

	FVector stopPos;
	float maxDist = 400.f;

	UPROPERTY()
	TArray<FVector> Lines;
	int32 LineSmooth = 40;

	FTimerHandle scaleTimer;
	float defaultScale = 0.6f;
	float scaleTime = 0.f;
	float scaleCoolTime = 0.2f;
	bool isScaling = false;

	bool isSmallMat = false;
	float defaultFresnelExp = 5.5f;
	float modifiedFresnelExp = 3.f;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "Potal.generated.h"

UCLASS()
class MOSS_API APotal : public AInteractiveItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void InPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OutPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	void PlayGateAnimation();

public:
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* doorComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* gatewayOuterComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* gatewayInnerComp;
	UPROPERTY()
	class AMainCharacter* mainCharacter;
	UPROPERTY()
	class UMainCharacterAnim* mainCharacterAnim;
	
	bool isPlayEndingAnimation = false;
	bool isOpenPortal = false;
	
	FTimerHandle gateAnimationTimer;
	bool isPlayGateAnimation = false;
	int gateAnimationCount = 0;
	int maxGateAnimationCount = 60;


};

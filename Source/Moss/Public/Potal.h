// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Potal.generated.h"

UCLASS()
class MOSS_API APotal : public AActor
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


	UPROPERTY(EditAnywhere)
	class UBoxComponent* doorway;
	
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* doorComp;

	UFUNCTION()
	void InPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OutPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY()
	class AMainCharacter* mainCharacter;

	UPROPERTY()
	class UMainCharacterAnim* mainCharacterAnim;
};

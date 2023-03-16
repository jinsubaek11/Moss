// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class MOSS_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 충돌체, 외관, 이동컴포넌트
	UPROPERTY(EditAnywhere)
		class USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
		class UProjectileMovementComponent* movementComp;

	UFUNCTION()
		void OnDie();
	
	// 총알 파편 효과 공장
	UPROPERTY(EditAnywhere, Category=BulletEffect)
	class UParticleSystem* bulletEffectFactory;
};
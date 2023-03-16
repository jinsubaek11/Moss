// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class MOSS_API AEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemy();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // 적 AI 관리 컴포넌트 클래스
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSMComponent)
        class UEnemyFSM* fsm;
    // 총 스켈레탈메시
    //UPROPERTY(EditAnywhere, Category=GunMesh)
    //    class USkeletalMeshComponent* gunMeshComp;

    UPROPERTY(EditAnywhere, Category = BulletFactory)
        TSubclassOf<class ABullet> bulletFactory;

    // 총알발사 처리 함수
    void RootFire();

    void SetInteract(bool value);
    void Interact(FVector start, FVector end);
    bool isInteract;

    void SetHighlight();
    void BeforeInteract();
    void AfterInteract();
    bool isHighlight = false;
    float defaultBrightness = 0.f;
    float modifiedBrightness = 1.f;
    float highlightTime = 0.f;
    float highlightCoolTime = 0.5f;
    FTimerHandle highlightTimer;
    UPROPERTY()
    class UMaterialInterface* originalWolfMaterial;
    UPROPERTY()
    class UMaterialInstanceDynamic* dynamicWolfMaterial;

    // 총알 파편 효과 공장
	UPROPERTY(EditAnywhere, Category=BulletEffect)
	class UParticleSystem* bulletEffectFactory;

};
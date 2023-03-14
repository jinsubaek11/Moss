// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Components/StaticMeshComponent.h>


// Sets default values
ABullet::ABullet()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;


    // 충돌체, 외관, 이동컴포넌트
    sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
    SetRootComponent(sphereComp);
    // 충돌체 크기설정
    sphereComp->SetSphereRadius(13);
    // 충돌프로파일 설정
    sphereComp->SetCollisionProfileName(TEXT("BlockAll"));
    // 부모 컴포넌트 지정
    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
    meshComp->SetupAttachment(RootComponent);
    // 외관 크기 설정
    meshComp->SetRelativeScale3D(FVector(0.25f));
    // 충돌 비활성화
    meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //meshComp->SetCollisionProfileName(TEXT("NoCollision"));

       // 발사체 컴포넌트
    movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
    // movement 컴포넌트가 갱신시킬 컴포넌트 지정
    movementComp->SetUpdatedComponent(sphereComp);
    // 초기속도
    movementComp->InitialSpeed = 5000;
    // 최대속도
    movementComp->MaxSpeed = 1500;
    // 반동 여부
    movementComp->bShouldBounce = true;
    // 반동 값
    movementComp->Bounciness = 0.5f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
    Super::BeginPlay();

    // InitialLifeSpan = 2;
    // SetLifeSpan(2);
    FTimerHandle dieTimerHandle;
    //GetWorldTimerManager().SetTimer(dieTimerHandle, this, &ABulletActor::OnDie, 0.1f);

    GetWorldTimerManager().SetTimer(dieTimerHandle, FTimerDelegate::CreateLambda(
        [this]()->void {
            this->Destroy();
        }
    ), 2, false);

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ABullet::OnDie()
{
    Destroy();
}

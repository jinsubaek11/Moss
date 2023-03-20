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


    // �浹ü, �ܰ�, �̵�������Ʈ
    sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
    SetRootComponent(sphereComp);
    // �浹ü ũ�⼳��
    sphereComp->SetSphereRadius(13);
    // �浹�������� ����
    sphereComp->SetCollisionProfileName(TEXT("BlockAll"));
    // �θ� ������Ʈ ����
    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
    meshComp->SetupAttachment(RootComponent);
    // �ܰ� ũ�� ����
    meshComp->SetRelativeScale3D(FVector(0.25f));
    // �浹 ��Ȱ��ȭ
    meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    //meshComp->SetCollisionProfileName(TEXT("NoCollision"));

       // �߻�ü ������Ʈ
    // movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
    // // movement ������Ʈ�� ���Ž�ų ������Ʈ ����
    // movementComp->SetUpdatedComponent(sphereComp);
    // // �ʱ�ӵ�
    // movementComp->InitialSpeed = 5000;
    // // �ִ�ӵ�
    // movementComp->MaxSpeed = 200;
    // // �ݵ� ����
    // movementComp->bShouldBounce = true;
    // // �ݵ� ��
    // movementComp->Bounciness = 1.0f;
        
    // //SetLifeSpan(2);
    // InitialLifeSpan = 2;

    //movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
    //// movement ������Ʈ�� ���Ž�ų ������Ʈ ����
    //movementComp->SetUpdatedComponent(sphereComp);
    //// �ʱ�ӵ�
    //movementComp->InitialSpeed = 5000;
    //// �ִ�ӵ�
    //movementComp->MaxSpeed = 1500;
    //// �ݵ� ����
    //movementComp->bShouldBounce = true;
    //// �ݵ� ��
    //movementComp->Bounciness = 0.5f;
    //movementComp->    
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
    Super::BeginPlay();

    //FTimerHandle dieTimerHandle;
    //GetWorldTimerManager().SetTimer(dieTimerHandle, this, &ABullet::OnDie, 0.1f);

    //GetWorldTimerManager().SetTimer(dieTimerHandle, FTimerDelegate::CreateLambda(
    //    [this]()->void {
    //        this->Destroy();
    //    }
    //), 2, false);


}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SetActorLocation(GetActorLocation() + GetActorForwardVector() * 900 * DeltaTime);
}

void ABullet::OnDie()
{
    Destroy();
}

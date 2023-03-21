// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "Bullet.h"
#include "components/capsulecomponent.h"



// Sets default values
AEnemy::AEnemy()
{
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("EnemyPreset"));

    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    // 1.스켈레탈메시 데이터 로드
    ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/AnimalVarietyPack/Wolf/Meshes/SK_Wolf.SK_Wolf'"));
    //1.데이터 로드 성공하면
    if (tempMesh.Succeeded())
    {
        //1-2. 데이터할당
        GetMesh()->SetSkeletalMesh(tempMesh.Object);
        //1-3. 메시 위치 및 회전 설정
        GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
        // 1-4. 메시 크기 수정
        GetMesh()->SetRelativeScale3D(FVector(0.84f));
    }
    // EnemyFSM 컴포넌트 추가
    fsm = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

    // 애니메이션 블루프린트 할당하기
    ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/VR/Blueprints/ABP_EnemyWolf.ABP_EnemyWolf_C'"));
    if (tempClass.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(tempClass.Class);
    }

    ConstructorHelpers::FObjectFinder<UMaterialInterface> wolfMat(TEXT("/Script/Engine.Material'/Game/VR/Material/M_InteractiveWolf.M_InteractiveWolf'"));
    if (wolfMat.Succeeded())
    {
        originalWolfMaterial = wolfMat.Object;
        GetMesh()->SetMaterial(0, wolfMat.Object);
    }

    //// 1. 총 스켈레탈메시 컴포넌트 등록
    //gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
    //// 1-1. 부모 컴포넌트를 Mesh 컴포넌트로 설정
    //gunMeshComp->SetupAttachment(GetMesh());
    //// 1-2. 스켈레탈메시 데이터 로드
    //ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
    //// 2. 읽어왔을때 성공했다면
    //if (TempGunMesh.Succeeded())
    //{
    //    // 3. Mesh에 적용하고싶다. 
    //    GetMesh()->SetSkeletalMesh(TempGunMesh.Object);
    //    // 4. Transform 을 수정하고싶다.
    //    GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
    //}



    // 월드에 배치되거나 스폰될 때 자동으로
    // AIController부터 Possess될 수 있도록 설정
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    dynamicWolfMaterial = UMaterialInstanceDynamic::Create(originalWolfMaterial, this);
    GetMesh()->SetMaterial(0, dynamicWolfMaterial);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (isInteract && !isHighlight)
    {
        BeforeInteract();
    }
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::RootFire()
{
    FRotator rot = GetActorRotation();

    if (isInteract)
    {
        rot.Pitch = 0.;
        AfterInteract();
    }

    GetWorld()->SpawnActor<ABullet>(bulletFactory, GetActorLocation(), rot);
}

void AEnemy::SetInteract(bool value)
{
    isInteract = value;
}

void AEnemy::BeforeInteract()
{
    isHighlight = true;
    SetHighlight();
}

void AEnemy::Interact(FVector start, FVector end)
{
    FRotator rot = (end - start).Rotation();
    SetActorRotation(rot);
}

void AEnemy::AfterInteract()
{
    isHighlight = false;
    SetHighlight();
}

void AEnemy::SetHighlight()
{
    if (highlightTime >= highlightCoolTime)
    {
        highlightTime = 0.f;
        GetWorld()->GetTimerManager().ClearTimer(highlightTimer);

        return;
    }

    highlightTime += GetWorld()->GetDeltaSeconds();

    float brightness;

    if (isHighlight)
    {
        brightness = FMath::Lerp<float>(defaultBrightness, modifiedBrightness, highlightTime / highlightCoolTime);
    }
    else
    {
        brightness = FMath::Lerp<float>(modifiedBrightness, defaultBrightness, highlightTime / highlightCoolTime);
    }

    dynamicWolfMaterial->SetVectorParameterValue(TEXT("Brightness"), FVector4(brightness, brightness, brightness, brightness));
    GetWorld()->GetTimerManager().SetTimer(highlightTimer, this, &AEnemy::SetHighlight, 0.02);
}
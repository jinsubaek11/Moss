// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include "EnemyAnim.h"
#include <Kismet/GameplayStatics.h>
#include "MainCharacter.h"
#include <Components/CapsuleComponent.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
    Super::BeginPlay();

    // 월드에서 MainCharacter 타킷 찾아오기
    auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass());
    // ATPSPlayer 타입으로 캐스팅
    target = Cast<AMainCharacter>(actor);
    // 소유 객체 가져오기
    me = Cast<AEnemy>(GetOwner());

    // UEnemyAnim* 할당
    anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
}

void UEnemyFSM::IdleState()
{
    // 1. 시간이 흘렀으니까
    currentTime += GetWorld()->DeltaRealTimeSeconds;
    // 1. 만약 경과 시간이 대기 시간을 초과했다면
    if (currentTime > idleDelayTime)
    {
        //3. 이동 상태로 전환하고 싶다.
        mState = EEnemyState::Move;
        // 경과 시간 초기화
        currentTime = 0;
    }
    // 목표 : 타깃이 공격 범위를 벗어나면 상태를 이동으로 전환하고 싶다.
    // 1. 타깃과의 기리가 필요하다.
    float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

    // 2. 타깃과의 거리가 공격 범위를 벗어났으니까
    if (distance > attackRange)
    {
        // 3. 상태를 이동으로 전환하고 싶다.
        mState = EEnemyState::Move;

        // 애니메이션 상태 동기화
        anim->animState = mState;
    }
}




void UEnemyFSM::MoveState()
{
    if (me == nullptr || target == nullptr)
    {
        return;
    }

    //UE_LOG(LogTemp, Warning, TEXT("kkkkk"))

    // 1. 타깃 목적지가 필요하다.
    FVector destination = target->GetActorLocation();
    // 2. 방향이 필요하다.
    FVector dir = destination - me->GetActorLocation();
    // 3. 방향으로 이동하고 싶다.
    me->AddMovementInput(dir.GetSafeNormal());

    // 타깃과 가까워지면 공격 상태로 전환하고 싶다.
    // 1. 만약 거리가 공격 범위 안에 들어오면
    if (dir.Size() < attackRange)
    {
        //2. 공격 상태로 전환하고 싶다.
        mState = EEnemyState::Attack;

        // 애니메이션 상태 동기화
        anim->animState = mState;
        // 공격 애니메이션 재생 활성화
        anim->bAttackPlay = true;
        // 공격 상태 전환 시 대기 시간이 바로 끝나도록 처리
        currentTime = attackDelayTime;
    }
}

void UEnemyFSM::AttackState()
{
    //목표: 일정시간에 한 번씩 공격하고 싶다.
    //1.시간이 흘러야한다.
    currentTime += GetWorld()->DeltaRealTimeSeconds;
    //2. 공격 시간이 됐으니까
    if (currentTime > attackDelayTime)
    {
        // 3.공격하고 싶다.
        me->RootFire();
        //UE_LOG(LogTemp, Warning, TEXT("Attack!!"));
        // 경과 시간 초기화
        currentTime = 0;
        anim->bAttackPlay = true;
    }
    // 목표: 타킷이 공격 범위를 벗어나면 상태를 이동으로 전환하고 싶다.
    //1. 타깃과의 거리가 필요하다.
    float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
    //2. 타깃과의 거리가 공격 범위를 벗어났으니까
    if (distance > attackRange)
    {
        // 3.상태를 이동으로 전환하고 싶다.
        mState = EEnemyState::Move;
        // 애니메이션 상태 동기화
        anim->animState = mState;
    }
}

void UEnemyFSM::DamageState()
{
    //1. 시간이 흘렀으니까
    currentTime += GetWorld()->DeltaTimeSeconds;
    //2. 만약 경과 시간이 대기 시간을 초과했다면
    if (currentTime > damageDelayTime)
    {
        //3. 대기 상태로 전환하고 싶다.
        mState = EEnemyState::Idle;
        //4. 경과 시간 초기화
        currentTime = 0;
        // 애니메이션 상태 동기화
        anim->animState = mState;
    }
}

void UEnemyFSM::DieState()
{
    // 계속 아래로 내려가고 싶다.
    // 등속운동 공식 P=P0+vt
    FVector P0 = me->GetActorLocation();
    FVector vt = FVector::DownVector * GetWorld()->DeltaTimeSeconds;
    FVector P = P0 + vt;
    me->SetActorLocation(P);

    // 1. 만약 2미터 이상 내려왔다면
    if (P.Z < -200.0f)
    {
        //2. 제거시킨다
        me->Destroy();
    }
}

void UEnemyFSM::OnDamageProcess()
{
    UE_LOG(LogTemp, Warning, TEXT("Die"))
        me->Destroy();

    // 체력감소
    hp--;
    // 만약 체력이 남아있다면
    if (hp > 0)
    {
        // 상태를 피격으로 전환
        mState = EEnemyState::Damage;
    }
    // 그렇지 않다면
    else
    {
        // 상태를 죽음으로 전환
        mState = EEnemyState::Die;
        // 캡슐 충돌체 비활성화
        me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
    // 애니메이션 상태 동기화
    anim->animState = mState;
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (mState)
    {

    case EEnemyState::Idle:
        IdleState();
        break;
    case EEnemyState::Move:
        MoveState();
        break;
    case EEnemyState::Attack:
        AttackState();
        break;
    case EEnemyState::Damage:
        DamageState();
        break;
    case EEnemyState::Die:
        DieState();
        break;
    }

    //UE_LOG(LogTemp, Warning, TEXT("%d"), mState)

}

// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "MainCharacter.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "MainCharacter.h"

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

	// ���忡�� MainCharacter ŸŶ ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass());
	// ATPSPlayer Ÿ������ ĳ����
	target = Cast<AMainCharacter>(actor);
	// ���� ��ü ��������
	me = Cast<AEnemy>(GetOwner());
}

void UEnemyFSM::IdleState()
{
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	// 1. ���� ��� �ð��� ��� �ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		//3. �̵� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Move;
		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
	}
}

void UEnemyFSM::MoveState()
{
	if (me == nullptr)
	{
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("kkkkk"))

		// 1. Ÿ�� �������� �ʿ��ϴ�.
		FVector destination = target->GetActorLocation();
	// 2. ������ �ʿ��ϴ�.
	FVector dir = destination - me->GetActorLocation();
	// 3. �������� �̵��ϰ� �ʹ�.
	me->AddMovementInput(dir.GetSafeNormal());

	// Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	// 1. ���� �Ÿ��� ���� ���� �ȿ� ������
	if (dir.Size() < attackdRange)
	{
		//2. ���� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	//��ǥ: �����ð��� �� ���� �����ϰ� �ʹ�.
	//1.�ð��� �귯���Ѵ�.
	currentTime += GetWorld()->DeltaRealTimeSeconds;
	//2. ���� �ð��� �����ϱ�
	if (currentTime > attackDelayTime)
	{
		// 3.�����ϰ� �ʹ�.
		//UE_LOG(LogTemp, Warning, TEXT("Attack!!"));
		// ��� �ð� �ʱ�ȭ
		currentTime = 0;
	}
	// ��ǥ: ŸŶ�� ���� ������ ����� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	//1. Ÿ����� �Ÿ��� �ʿ��ϴ�.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	//2. Ÿ����� �Ÿ��� ���� ������ ������ϱ�
	if (distance > attackdRange)
	{
		// 3.���¸� �̵����� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Move;
	}
}

void UEnemyFSM::DamageState()
{
}

void UEnemyFSM::DieState()
{
}

void UEnemyFSM::OnDamageProcess()
{
	me->Destroy();
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

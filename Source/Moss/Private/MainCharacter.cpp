#include "MainCharacter.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacterAnim.h"
//#include "EnemyFSM.h"


AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/VR/Assets/MainCharacter/Ch29_nonPBR_1__UE.Ch29_nonPBR_1__UE'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	}

	//무기넣고싶음
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(GetMesh());
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));;
	meshComp->SetupAttachment(GetMesh(),TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMeshComponent> staticMesh(TEXT("/Script/Engine.StaticMesh'/Game/VR/Assets/Weapon/rose_quartzs_sword.rose_quartzs_sword'"));
	if (staticMesh.Succeeded())
	{
		meshComp->SetRelativeLocation(FVector(-42, 7, 1));
		meshComp->SetRelativeRotation(FRotator(0, 90, 0));

	}
	boxComp->SetCollisionProfileName(TEXT("Sword"));
	boxComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	boxComp->SetRelativeLocation(FVector(-42, 7, 1));
	boxComp->SetRelativeRotation(FRotator(0, 90, 0));
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	hp = initialHp;

}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	
}

void AMainCharacter::InputJump()
{
	Jump();
}

void AMainCharacter::InputRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 500;
}

void AMainCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 300;

}

void AMainCharacter::OnHitEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Ouch"));
	hp--;
	if (hp <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Death"));
	}
}

void AMainCharacter::InputAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("attack"));
	auto anim = Cast<UMainCharacterAnim>(GetMesh()->GetAnimInstance());
	anim->PlayAttackAnim();
	//부딪힌 대상 적인지 판단
	/*
	auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
	if (enemy)
	{
		auto enemyFSM = Cast<UEnemyFSM>(enemy);
		enemyFSM->OnDamageProcess();
	}
	*/
}



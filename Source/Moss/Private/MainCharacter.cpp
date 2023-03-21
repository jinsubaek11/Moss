#include "MainCharacter.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacterAnim.h"
#include "EnemyFSM.h"
#include "Magic.h"
#include "Components/CapsuleComponent.h"
#include <Animation/AnimMontage.h>
#include "Kismet/GameplayStatics.h"
#include <Components/ArrowComponent.h>

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("CharacterPreset"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	}

	//무기넣고싶음
	//boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	//boxComp->SetupAttachment(GetMesh());
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	meshComp->SetRelativeLocation(FVector(-42, 7, 1));
	meshComp->SetRelativeRotation(FRotator(0, 90, 0));
	meshComp->SetCollisionProfileName(TEXT("WeaponPreset"));
	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("arrowComp"));
	arrowComp->SetupAttachment(RootComponent);
	//boxComp->SetCollisionProfileName(TEXT("Sword"));
	//boxComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	//boxComp->SetRelativeLocation(FVector(-42, 7, 1));
	//boxComp->SetRelativeRotation(FRotator(0, 90, 0));
	//boxComp->SetCollisionProfileName(TEXT("WeaponPreset"));

	//포탈애니메이션
	ConstructorHelpers::FObjectFinder<UAnimSequence>anim(TEXT("AnimSequence'/Game/VR/Animation/MainCharacter/fine_UE.fine_UE'"));
	if (anim.Succeeded())
	{
		Anim = anim.Object;
	}
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	hp = initialHp;

	GetCharacterMovement()->JumpZVelocity = 600;
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
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void AMainCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 500;

}

void AMainCharacter::OnHitEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Ouch"));
	hp--;
	if (hp <= 0)
	{
		isDead;
		UE_LOG(LogTemp, Warning, TEXT("Deathsign"));

		auto anim = Cast<UMainCharacterAnim>(GetMesh()->GetAnimInstance());
		if (anim) {
			bool isMontagePlaying = anim->IsAnyMontagePlaying();
			if (isMontagePlaying == false)
			{
			UE_LOG(LogTemp,Warning,TEXT("PlayDeathanim"));
				anim->PlayDeathAnim();
				/*
				FTimerHandle deathTimer;
				FTimerDelegate timeDelegate;
				timeDelegate.BindLambda([this]() -> void 
				{
					UGameplayStatics::OpenLevel(this,TEXT("Three"));
				});

				GetWorld()->GetTimerManager().SetTimer(deathTimer, timeDelegate, 3, false);
				*/
			}
		}
		
	}
}

void AMainCharacter::InputAttack()
{
	auto anim = Cast<UMainCharacterAnim>(GetMesh()->GetAnimInstance());
	if (anim) {
		bool isMontagePlaying = anim->IsAnyMontagePlaying();
		if (isMontagePlaying == false)
		{
			anim->PlayAttackAnim();

		}
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("attack"));
	
	//LineTrace의 시작 위치
	FVector startPos = meshComp->GetComponentLocation();
	//LineTrace의 종료 위치
	FVector endPos = meshComp-> GetComponentLocation ()+ meshComp->GetForwardVector() *100;
	//LineTrace의 충돌 정보를 담을 변수
	FHitResult hitInfo;
	//충돌옵션설정 변수
	FCollisionQueryParams params;
	//자기자신은 충돌에서 제외
	params.AddIgnoredActor(this);
	//channel 필터를 이용한 LineTrace충돌 검출 (충돌정보, 시작위치, 종료위치, 검출채널, 충돌옵션)
	bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos,ECC_Visibility,params);
	//Linetrace가 부딪혔을때
	if (bHit)
	{
		//충돌처리 
		
			FTransform trans(hitInfo.ImpactPoint);
			}


	//부딪힌 대상 적인지 판단
	auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
	if (enemy)
	{
		auto enemyFSM = Cast<UEnemyFSM>(enemy);
		enemyFSM->OnDamageProcess();
	}

}

//포탈애니메이션
void AMainCharacter::PlayAnim()
{
	auto anim = Cast<UMainCharacterAnim>(GetMesh()->GetAnimInstance());
	if (anim) {
		bool isMontagePlaying = anim->IsAnyMontagePlaying();
		if (isMontagePlaying == false)
		{
			anim->PlayFinishAnim();
		}
	}
}

void AMainCharacter::InputMagic()
{
	auto anim = Cast<UMainCharacterAnim>(GetMesh()->GetAnimInstance());
	if (anim) {
		bool isMontagePlaying = anim->IsAnyMontagePlaying();
		if (isMontagePlaying == false)
		{
			anim->PlayMagicAnim();
			if (anim->isMagicEnd)
			{
				GetWorld()->SpawnActor<AMagic>(magicFactory, arrowComp->GetComponentLocation(), arrowComp->GetComponentRotation());
			}

		}
	}
}

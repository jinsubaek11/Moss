#include "MainCharacter.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>


AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	}

	//무기넣고싶음
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));;
	meshComp->SetupAttachment(RootComponent);
	boxComp->SetCollisionProfileName(TEXT("Sword"));
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
	
}



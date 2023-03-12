// Fill out your copyright notice in the Description page of Project Settings.


#include "Magic.h"
#include <Components/SphereComponent.h>
#include "EnemyFSM.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AMagic::AMagic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	magicBoxComp=CreateDefaultSubobject<USphereComponent>(TEXT("magicBoxComp"));
	SetRootComponent(magicBoxComp);
	magicBoxComp->SetCollisionProfileName(TEXT("Weapon"));

	magicComp=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("magicComp"));
	magicComp->SetupAttachment(magicBoxComp);

	

}

// Called when the game starts or when spawned
void AMagic::BeginPlay()
{
	Super::BeginPlay();

	enemyFSM = Cast<UEnemyFSM>(UGameplayStatics::GetActorOfClass(GetWorld(), UEnemyFSM::StaticClass()));

	magicBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AMagic::OverlapMagic);
}

// Called every frame
void AMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector().RotateAngleAxis(0, FVector(1, 0, 0)).GetSafeNormal() * moveSpeed * DeltaTime);
	

	currentTime += DeltaTime;
	if (currentTime >= 30)
	{
		UE_LOG(LogTemp,Warning,TEXT("Ddd"));
		Destroy();
	}

	//GetActorRightVector()
	//GetActorUpVector()
}

void AMagic::OverlapMagic(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	enemyFSM->OnDamageProcess();
	Destroy();
}


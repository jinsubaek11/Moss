// Fill out your copyright notice in the Description page of Project Settings.


#include "KillZone.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <Components/BoxComponent.h>

// Sets default values
AKillZone::AKillZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	killBox = CreateDefaultSubobject<UBoxComponent>(TEXT("killBox"));
	
	killBox->SetCollisionProfileName(TEXT("PortalPrest"));
	SetRootComponent(RootComponent);

}

// Called when the game starts or when spawned
void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
	 killBox->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::InKillZone);
	 mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));

}

// Called every frame
void AKillZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillZone::InKillZone(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//mainCharacter->OnHitEvent();

	UGameplayStatics::OpenLevel(this, TEXT("Three"));
	
}



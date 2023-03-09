// Fill out your copyright notice in the Description page of Project Settings.


#include "Potal.h"
#include <Components/BoxComponent.h>

// Sets default values
APotal::APotal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp= CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetCollisionProfileName(TEXT("PotalPreset"));
	SetRootComponent(boxComp);
	boxComp-> SetBoxExtent(FVector(100,10,100));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp -> SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void APotal::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APotal::InPotal);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &APotal::OutPotal);

}

// Called every frame
void APotal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APotal::InPotal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Fine"));
}

void APotal::OutPotal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Out"));

}


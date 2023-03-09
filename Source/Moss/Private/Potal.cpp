// Fill out your copyright notice in the Description page of Project Settings.


#include "Potal.h"
#include <Components/BoxComponent.h>
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacterAnim.h"


// Sets default values
APotal::APotal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	doorway = CreateDefaultSubobject<UBoxComponent>(TEXT("doorway"));
	doorway->SetCollisionProfileName(TEXT("PortalPreset"));
	SetRootComponent(doorway);
	doorway-> SetBoxExtent(FVector(100,10,100));

	doorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorComp"));
	doorComp-> SetupAttachment(doorway);

	

}

// Called when the game starts or when spawned
void APotal::BeginPlay()
{
	Super::BeginPlay();
	
	doorway->OnComponentBeginOverlap.AddDynamic(this, &APotal::InPortal);
	doorway->OnComponentEndOverlap.AddDynamic(this, &APotal::OutPortal);

	mainCharacter =Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
	//mainCharacterAnim = Cast<UMainCharacterAnim>(GetMesh()->GetAnimInstance());

}

// Called every frame
void APotal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APotal::InPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Fine"));
	
	mainCharacter->PlayAnim();

}


void APotal::OutPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Out"));


}


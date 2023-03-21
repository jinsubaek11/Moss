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

	boxComp->SetCollisionProfileName(TEXT("PortalPreset"));
	boxComp-> SetBoxExtent(FVector(75,65,100));

	//doorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("doorComp"));
	//doorComp-> SetupAttachment(RootComponent);

	gatewayOuterComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gatewayOuterComp"));
	gatewayOuterComp->SetupAttachment(RootComponent);

	gatewayInnerComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gatewayInnerComp"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> innerMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (innerMesh.Succeeded())
	{
		gatewayInnerComp->SetStaticMesh(innerMesh.Object);
		gatewayInnerComp->SetupAttachment(RootComponent);
	}
	
	ConstructorHelpers::FObjectFinder<UMaterialInterface> ogMat(TEXT("/Script/Engine.Material'/Game/VR/Material/M_Portal.M_Portal'"));
	if (ogMat.Succeeded())
	{
		originalMat = ogMat.Object;
		gatewayInnerComp->SetMaterial(0, ogMat.Object);
	}
}

// Called when the game starts or when spawned
void APotal::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APotal::InPortal);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &APotal::OutPortal);


	mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));

	dynamicMat = UMaterialInstanceDynamic::Create(originalMat, this);
	for (int i = 0; i < gatewayInnerComp->GetMaterials().Num(); i++)
	{
		gatewayInnerComp->SetMaterial(i, dynamicMat);
	}
}

// Called every frame
void APotal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APotal::InPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isOpenPortal) return;

	mainCharacter->PlayAnim();
	isPlayEndingAnimation = true;
	isOpenPortal = true;

	//PlayGateAnimation();
}


void APotal::OutPortal(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("Out"));



}

void APotal::PlayGateAnimation()
{
	isPlayGateAnimation = true;
	dynamicMat->SetScalarParameterValue(TEXT("IsActive"), 1.);

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]()-> void {
		if (gateAnimationCount++ > maxGateAnimationCount)
		{
			gateAnimationCount = 0;
			isPlayGateAnimation = false;
			GetWorld()->GetTimerManager().ClearTimer(gateAnimationTimer);
			return;
		}

		float r = FMath::Lerp<float>(1.5, 0., gateAnimationCount/(float)maxGateAnimationCount);
		dynamicMat->SetScalarParameterValue(TEXT("Radius"), r);
	});

	GetWorld()->GetTimerManager().SetTimer(gateAnimationTimer, timerDelegate, .01, true);
}




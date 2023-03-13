#include "InteractiveItem.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


AInteractiveItem::AInteractiveItem()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	boxComp->SetCollisionProfileName(TEXT("ItemPreset"));
	SetRootComponent(boxComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	meshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> ogMat(TEXT("/Script/Engine.Material'/Game/VR/Material/M_InteractiveBox.M_InteractiveBox'"));
	if (ogMat.Succeeded())
	{
		originalMat = ogMat.Object;
		meshComp->SetMaterial(0, ogMat.Object);
	}
}

void AInteractiveItem::BeginPlay()
{
	Super::BeginPlay();
	
	dynamicMat = UMaterialInstanceDynamic::Create(originalMat, this);
	for (int i = 0; i < meshComp->GetMaterials().Num(); i++)
	{
		meshComp->SetMaterial(i, dynamicMat);
	}
}

void AInteractiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveItem::SetIsReadyToInteract(bool value)
{
	isReadyToInteract = value;
}

EItemType AInteractiveItem::GetType() const
{
	return type;
}



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
}

void AInteractiveItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveItem::SetIsActive(bool value)
{
	isActive = value;
}

EItemType AInteractiveItem::GetType() const
{
	return type;
}



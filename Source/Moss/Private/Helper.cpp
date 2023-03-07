#include "Helper.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "FirstLevelPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "InteractiveItem.h"


AHelper::AHelper()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphereComp->SetCollisionProfileName(TEXT("HelperPreset"));
	SetRootComponent(sphereComp);

	coreMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Core Mesh Component"));
	coreMeshComp->SetupAttachment(RootComponent);

	shieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh Component"));
	shieldMeshComp->SetupAttachment(RootComponent);
}

void AHelper::BeginPlay()
{
	Super::BeginPlay();
	
	mainCharacter = Cast<AMainCharacter>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass())
	);
	player = Cast<AFirstLevelPlayer>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AFirstLevelPlayer::StaticClass())
	);

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHelper::OnBeginOverlap);
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &AHelper::OnEndOverlap);
}

void AHelper::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AInteractiveItem* item = Cast<AInteractiveItem>(OtherActor);

	if (item)
	{
		currentItem = item;
		isOverlapping = true;
	}
}

void AHelper::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	isOverlapping = false;
	overlappingTime = 0.f;
}

void AHelper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerCameraManager* cm = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	FRotator newRotation = UKismetMathLibrary::MakeRotFromXZ(
		cm->GetCameraLocation() - GetActorLocation(), GetActorUpVector()
	);

	SetActorRotation(newRotation);

	if (isOverlapping)
	{
		overlappingTime += DeltaTime;

		if (overlappingTime >= 1.f)
		{
			isReadyToInteract = true;

		}
	}
	
	if (isReadyToInteract)
	{
		UE_LOG(LogTemp, Warning, TEXT("isReadyToInteract"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("isNotReadyToInteract"));
	}

	if (isInteractToItem && isReadyToInteract)
	{
		//UE_LOG(LogTemp, Warning, TEXT("startInteract"));
		isInteractToItem = false;
		isReadyToInteract = false;
		InteractToItem();
	}

	if (isActivate)
	{
		if (isMoveToReady)
		{
			ReadyToMove();
			return;
		}

		if (isForwardMove)
		{
			ForwardMove();
			return;
		}

		if (isMove)
		{
			Move();
			return;
		}
	}

	if (isBackToMove)
	{
		BackToMove();
		return;
	}

	if (mainCharacter)
	{
		FollowMainCharacter();
	}

}

void AHelper::SetActivate(bool value)
{
	if (value)
	{
		isMoveToReady = true;
		isBackToMove = true;
	}
	else
	{
		isMoveToReady = false;
	}

	isActivate = value;
	moveTime = 0.f;
}

void AHelper::Move()
{
	TArray<FVector> collisionTracePoints = GetPlayerViewTracePoint(collisionRange);
	TArray<FVector> tracePoints = GetPlayerViewTracePoint(normalRange);

	SetActorLocation(tracePoints[1]);
}

void AHelper::ForwardMove()
{
	if (moveTime >= moveCoolTime)
	{
		moveTime = 0.f;
		return;
	}

	TArray<FVector> tracePoints = GetPlayerViewTracePoint(collisionRange);
	FHitResult hitResult;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool isHit = GetWorld()->LineTraceSingleByChannel(
		hitResult, tracePoints[0], tracePoints[1], ECollisionChannel::ECC_Visibility, params);

	if (isHit)
	{
		moveTime += GetWorld()->GetDeltaSeconds();
		FVector lerpPos = FMath::Lerp<FVector>(GetActorLocation(), hitResult.Location, moveTime / moveCoolTime);
		SetActorLocation(lerpPos);
	}
}

void AHelper::SetIsInteractToItem(bool value)
{
	isInteractToItem = value;
}

void AHelper::MoveLerp()
{

}

void AHelper::ReadyToMove()
{
	if (moveTime >= moveCoolTime)
	{
		isMove = true;
		isMoveToReady = false;
		moveTime = 0.f;
		return;
	}

	FVector curPos = GetActorLocation();

	moveTime += GetWorld()->GetDeltaSeconds();
	FVector lerpPos = FMath::Lerp<FVector>(curPos, GetPlayerViewTracePoint(normalRange)[1], moveTime / moveCoolTime);
	SetActorLocation(lerpPos);
}

void AHelper::BackToMove()
{
	if (moveTime >= moveCoolTime)
	{
		isBackToMove = false;
		moveTime = 0.f;
		return;
	}

	FVector curPos = GetActorLocation();
	FVector end =
		mainCharacter->GetActorLocation() + mainCharacter->GetActorRightVector() * 60
		- mainCharacter->GetActorForwardVector() * 30 + FVector(0, 0, 100);

	moveTime += GetWorld()->GetDeltaSeconds();
	FVector lerpPos = FMath::Lerp<FVector>(curPos, end, moveTime / moveCoolTime);
	SetActorLocation(lerpPos);
}

void AHelper::InteractToItem()
{
	switch (currentItem->GetType())
	{
	case EItemType::BOX:
		currentItem->Interact(mouseStart, mouseEnd);
		break;
	}
}

void AHelper::FollowMainCharacter()
{
	FVector position =
		mainCharacter->GetActorLocation() + mainCharacter->GetActorRightVector() * 60
		- mainCharacter->GetActorForwardVector() * 30 + FVector(0, 0, 100);
	SetActorLocation(position);
}

TArray<FVector> AHelper::GetPlayerViewTracePoint(float scale) const
{
	if (!player) return TArray<FVector>{ FVector::ZeroVector };

	FVector start;
	FRotator rotation;
	player->GetController()->GetPlayerViewPoint(start, rotation);

	return TArray<FVector>{ start, start + rotation.Vector() * scale };
}

bool AHelper::GetIsActivate() const
{
	return isActivate;
}

void AHelper::SetIsForwardMove(bool value)
{
	isForwardMove = value;
}

void AHelper::SetMousePos(FVector2D pos, EMouseType type)
{
	if (type == EMouseType::START)
	{
		mouseStart = pos;
		return;
	}

	mouseEnd = pos;
}

void AHelper::SetIsReadyToInteract(bool value)
{
	isReadyToInteract = value;
}

void AHelper::DrawMovingDistLine()
{

}

#include "Helper.h"
#include "Components/SphereComponent.h"
#include "MainCharacter.h"
#include "FirstLevelPlayer.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "InteractiveItem.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include <HeadMountedDisplayFunctionLibrary.h>
#include <MotionControllerComponent.h>


AHelper::AHelper()
{
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphereComp->SetCollisionProfileName(TEXT("HelperPreset"));
	SetRootComponent(sphereComp);

	coreMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Core Mesh Component"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> coreMat(TEXT("/Script/Engine.Material'/Game/VR/Material/M_HelperCore.M_HelperCore'"));
	if (coreMat.Succeeded())
	{
		coreMeshComp->SetMaterial(0, coreMat.Object);
	}
	coreMeshComp->SetupAttachment(RootComponent);

	shieldMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shield Mesh Component"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface> shieldMat(TEXT("/Script/Engine.Material'/Game/VR/Material/M_Shield.M_Shield'"));
	if (shieldMat.Succeeded())
	{
		originalShieldMaterial = shieldMat.Object;
		shieldMeshComp->SetMaterial(0, shieldMat.Object);
	}
	shieldMeshComp->SetupAttachment(RootComponent);

	lineComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LineComp"));
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> lineNs(TEXT("/Script/Niagara.NiagaraSystem'/Game/VRTemplate/VFX/NS_TeleportTrace.NS_TeleportTrace'"));
	if (lineNs.Succeeded())
	{
		lineComp->SetAsset(lineNs.Object);
	}
	
	lineComp->SetupAttachment(RootComponent);
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

	dynamicShieldMaterial = UMaterialInstanceDynamic::Create(originalShieldMaterial, this);
	for (int i = 0; i < shieldMeshComp->GetMaterials().Num(); i++)
	{
		shieldMeshComp->SetMaterial(i, dynamicShieldMaterial);
	}
}

void AHelper::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	interactTarget = OtherActor;
	isOverlapping = true;
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

		if (overlappingTime >= 1.f && !isReadyToInteract)
		{
			isReadyToInteract = true;
			stopPos = GetActorLocation();
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("%d, %d"), isInteractToTarget, isReadyToInteract);
	if (isInteractToTarget && isReadyToInteract)
	{
		AInteractiveItem* item = Cast<AInteractiveItem>(interactTarget);
		if (item)
		{
			InteractToItem();
			item->SetIsReadyToInteract(false);	
		}
		else
		{
			AEnemy* enemy = Cast<AEnemy>(interactTarget);
			if (!enemy) return;

			enemy->RootFire();
			enemy->isInteract = false;
		}

		dynamicShieldMaterial->SetScalarParameterValue(TEXT("IsSmall"), 0);
		dynamicShieldMaterial->SetScalarParameterValue(TEXT("FresnelExp"), defaultFresnelExp);
		isReadyToInteract = false;
		isScaling = false;
		isInteractToTarget = false;

	}
	
	if (isInteractToTarget)
	{
		isInteractToTarget = false;
	}

	if (isReadyToInteract)
	{
		AInteractiveItem* item = Cast<AInteractiveItem>(interactTarget);
		if (item)
		{
			item->SetIsReadyToInteract(true);
		}
		else
		{
			AEnemy* enemy = Cast<AEnemy>(interactTarget);
			if (!enemy) return;

			enemy->Interact(interactStartPos, interactEndPos);
			enemy->isInteract = true;
		}

		dynamicShieldMaterial->SetScalarParameterValue(TEXT("IsSmall"), 1);
		dynamicShieldMaterial->SetScalarParameterValue(TEXT("FresnelExp"), modifiedFresnelExp);
		SetShieldScaleSmall(true);

		TArray<FVector> tracePoints = GetPlayerViewTracePoint(collisionRange);
		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		params.AddIgnoredActor(mainCharacter);

		bool isHit = GetWorld()->LineTraceSingleByChannel(
			hitResult, tracePoints[0], tracePoints[1], ECollisionChannel::ECC_Visibility, params);

		if (isHit)
		{
			FVector corePos = hitResult.Location;
			corePos.Z = stopPos.Z;

			float dist = FVector::Dist(corePos, stopPos);
			if (dist > maxDist)
			{
				corePos = stopPos + (corePos - stopPos).GetSafeNormal() * maxDist;
			}

			coreMeshComp->SetWorldLocation(corePos);
			SetActorLocation(stopPos);

			overlappingTime = 0.f;

			DrawDistanceLine(stopPos, corePos);

			interactStartPos = stopPos;
			interactEndPos = corePos;

			if (lineComp)
			{
				//coreMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				lineComp->SetVisibility(true);
				UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(lineComp, FName(TEXT("User.PointArray")), Lines);
			}
		}

		return;
	}
	else
	{
		//coreMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		coreMeshComp->SetWorldLocation(GetActorLocation());
		SetShieldScaleSmall(false);
		lineComp->SetVisibility(false);
		ClearLine();
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

void AHelper::SetisInteractToTarget(bool value)
{
	isInteractToTarget = value;
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
	AInteractiveItem* item = Cast<AInteractiveItem>(interactTarget);
	if (!item) return;

	switch (item->GetType())
	{
	case EItemType::BOX:
		item->Interact(interactStartPos, interactEndPos);
		break;
	}
}

void AHelper::InteractToEnemy()
{
	AEnemy* enemy = Cast<AEnemy>(interactTarget);
	if (!enemy) return;

	enemy->Interact(interactStartPos, interactEndPos);
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
	FVector end;
	FRotator rotation;

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		start = player->RightHand->GetComponentLocation();
		end = start + player->RightHand->GetForwardVector() * scale;
		return TArray<FVector>{ start, end };
	}
	else
	{
		player->GetController()->GetPlayerViewPoint(start, rotation);
	}


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

void AHelper::SetIsReadyToInteract(bool value)
{
	isReadyToInteract = value;	
}

void AHelper::ClearLine()
{
	Lines.RemoveAt(0, Lines.Num());
}

void AHelper::DrawDistanceLine(FVector& start, FVector& end)
{
	ClearLine();

	for (int i = 0; i < LineSmooth; i++)
	{
		FVector pos = FMath::Lerp<FVector>(start, end, i / (float)LineSmooth);
		Lines.Add(pos);
	}
}

void AHelper::SetShieldScaleSmall(bool isSmall)
{
	if (!isSmall)
	{
		shieldMeshComp->SetRelativeScale3D(FVector(defaultScale));
		return;
	}

	if (isScaling) return;

	isScaling = true;

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]()->void {
		if (scaleTime >= scaleCoolTime)
		{
			scaleTime = 0.f;
			GetWorld()->GetTimerManager().ClearTimer(scaleTimer);

			return;
		}
		scaleTime += GetWorld()->GetDeltaSeconds();

		float curScale = FMath::Lerp<float>(defaultScale, 0.5, scaleTime / scaleCoolTime);

		shieldMeshComp->SetRelativeScale3D(FVector(curScale));
	});

	GetWorld()->GetTimerManager().SetTimer(scaleTimer, timerDelegate, 0.02, true);
}
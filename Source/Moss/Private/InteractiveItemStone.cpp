#include "InteractiveItemStone.h"
#include "Components/BoxComponent.h"


AInteractiveItemStone::AInteractiveItemStone()
{
	//type = EItemType::BRIDGE;

	boxComp->SetBoxExtent(FVector(52, 115, 13));

	//hardMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HardMesh Component"));
	//hardMeshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> stoneMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (stoneMesh.Succeeded())
	{
		meshComp->SetStaticMesh(stoneMesh.Object);
		meshComp->SetRelativeLocation(FVector(0.1, -1.6, -150));
		meshComp->SetRelativeRotation(FRotator(0, -55, 0));
		meshComp->SetRelativeScale3D(FVector(3.1));

		//hardMeshComp->SetStaticMesh(stoneMesh.Object);
		//hardMeshComp->SetRelativeLocation(FVector(0.1, -1.6, -137));
		//hardMeshComp->SetRelativeRotation(FRotator(0, -55, 0));
		//hardMeshComp->SetRelativeScale3D(FVector(2.5));
	}

	//ConstructorHelpers::FObjectFinder<UMaterialInterface> tempStoneMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/VR/Material/M_InteractiveBridgeInst.M_InteractiveBridgeInst'"));
	//if (tempStoneMat.Succeeded())
	//{
	//	originalMat = tempStoneMat.Object;
	//	meshComp->SetMaterial(0, tempStoneMat.Object);
	//}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> tempHardStoneMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Stylized_Egypt/Materials/stones_materials/MI_stones_01_Inst.MI_stones_01_Inst'"));
	if (tempHardStoneMat.Succeeded())
	{
		//hardStoneMat = tempHardStoneMat.Object;
		meshComp->SetMaterial(0, tempHardStoneMat.Object);
	}
}

void AInteractiveItemStone::BeginPlay()
{
	Super::BeginPlay();

	//dynamicMat = UMaterialInstanceDynamic::Create(originalMat, this);
	//for (int i = 0; i < meshComp->GetMaterials().Num(); i++)
	//{
	//	meshComp->SetMaterial(i, dynamicMat);
	//}

	//dynamicMat->SetScalarParameterValue(TEXT("Smooth"), 0.);

	//meshComp->SetVisibility(false);

	//origin = GetActorLocation();

	//FTimerHandle timer;
	//GetWorldTimerManager().SetTimer(timer, this, &AInteractiveItemStone::SetActiveSmoothStone, 2.f);
}

//void AInteractiveItemStone::SetActiveSmoothStone()
//{
//	meshComp->SetVisibility(true);
//
//	FVector start = origin;
//
//	FTimerDelegate timerDelegate;
//	timerDelegate.BindLambda([&]() -> void {
//		if (moveTime >= moveCoolTime)
//		{
//			meshComp->SetVisibility(false);
//			GetWorldTimerManager().ClearTimer(stoneMoveTimer);
//			return;
//		}
//
//		moveTime += GetWorld()->GetDeltaSeconds();
//
//		//if (moveTime > 0.2f)
//		//{
//			//float smoothValue = FMath::Lerp<float>(60, 200, smoothTime / smoothCoolTime);
//			//dynamicMat->SetScalarParameterValue(TEXT("Smooth"), smoothValue);
//			dynamicMat->SetScalarParameterValue(TEXT("Smooth"), 300);
//		//}
//
//		FVector endPos = origin + GetActorForwardVector() * 350;
//		FVector curPos = FMath::Lerp<FVector>(origin, endPos, moveTime / moveCoolTime);
//		curPos.Z = hardMeshComp->GetComponentLocation().Z - 30;
//		
//		meshComp->SetWorldLocation(curPos);
//	});
//	
//	GetWorldTimerManager().SetTimer(stoneMoveTimer, timerDelegate, 0.02, true);
//}
//
//void AInteractiveItemStone::SetOrigin(FVector loc)
//{
//	origin = loc;
//}
//
//FVector AInteractiveItemStone::GetNextPosition() const
//{
//	FVector nextPos = meshComp->GetComponentLocation();
//	nextPos.Z = GetActorLocation().Z;
//	return nextPos;
//}

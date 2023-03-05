#include "FirstLevelPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <MotionControllerComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/SkeletalMeshComponent.h>


AFirstLevelPlayer::AFirstLevelPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	rightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right"));
	rightHand->SetupAttachment(RootComponent);
	rightHandMesh =CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("rightHandMesh"));
	rightHandMesh->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT(""));
	if (TempMesh.Succeeded())
	{
		rightHandMesh->SetSkeletalMesh(TempMesh.Object);
	}
}

void AFirstLevelPlayer::BeginPlay()
{
	Super::BeginPlay();

	auto pc = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc)
	{
		auto localPlayer = pc->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(localPlayer);
		if (subSystem)
		{
			subSystem->AddMappingContext(IMC_Input, 0);
		}
	}

	mainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMainCharacter::StaticClass()));
	if (mainCharacter)
	{
		mainCharacter->GetCharacterMovement()->bOrientRotationToMovement = true; 
		mainCharacter->GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	}
}

void AFirstLevelPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFirstLevelPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* inputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (inputSystem)
	{
		inputSystem->BindAction(IA_Mouse, ETriggerEvent::Triggered, this, &AFirstLevelPlayer::Turn);
		inputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AFirstLevelPlayer::Move);
		PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFirstLevelPlayer::Jumping);
	}
}

void AFirstLevelPlayer::Turn(const FInputActionValue& Values)
{
	FVector2D Axis = Values.Get<FVector2D>();

	AddControllerYawInput(Axis.X * 0.5f);
	AddControllerPitchInput(-Axis.Y * 0.5f);
}

void AFirstLevelPlayer::Move(const FInputActionValue& Values)
{
	FVector2D axis = Values.Get<FVector2D>();

	mainCharacter->AddMovementInput(FVector(1, 0, 0), axis.X);
	mainCharacter->AddMovementInput(FVector(0, 1, 0), axis.Y);
}

void AFirstLevelPlayer::Jumping()
{
	mainCharacter->InputJump();
}

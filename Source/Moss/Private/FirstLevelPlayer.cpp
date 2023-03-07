#include "FirstLevelPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helper.h"


AFirstLevelPlayer::AFirstLevelPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AFirstLevelPlayer::BeginPlay()
{
	Super::BeginPlay();

	playerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if (playerController)
	{
		auto localPlayer = playerController->GetLocalPlayer();
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

	helper = Cast<AHelper>(UGameplayStatics::GetActorOfClass(GetWorld(), AHelper::StaticClass()));
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
		inputSystem->BindAction(IA_HelperMouse, ETriggerEvent::Triggered, this, &AFirstLevelPlayer::SetHelperActivate);
		inputSystem->BindAction(IA_HelperMove, ETriggerEvent::Triggered, this, &AFirstLevelPlayer::SetHelperMove);
		inputSystem->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AFirstLevelPlayer::Move);
		
	
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

void AFirstLevelPlayer::SetHelperActivate(const FInputActionValue& Values)
{
	if (!helper) return;

	bool isActivate = Values.Get<bool>();
	helper->SetActivate(isActivate);
}

void AFirstLevelPlayer::SetHelperMove(const FInputActionValue& Values)
{
	if (!helper) return;

	bool isForwardMove = Values.Get<bool>();
	double x, y;
	playerController->GetMousePosition(x, y);

	if (isForwardMove)
	{
		helper->SetMousePos(FVector2D(x, y), EMouseType::START);
	}
	else
	{
		helper->SetMousePos(FVector2D(x, y), EMouseType::END);
		helper->SetIsInteractToItem(true);

		FTimerHandle timer;
		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda(
			[this]() -> void {
				helper->SetIsReadyToInteract(false);
			}
		);

		GetWorld()->GetTimerManager().SetTimer(timer, timerDelegate, false, 1.f);
	}

	helper->SetIsForwardMove(isForwardMove);
}


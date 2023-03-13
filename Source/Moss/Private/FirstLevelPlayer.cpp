#include "FirstLevelPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Helper.h"
#include "Potal.h"



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

	potal = Cast<APotal>(UGameplayStatics::GetActorOfClass(GetWorld(), APotal::StaticClass()));
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
		PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFirstLevelPlayer::Jumping);
		PlayerInputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AFirstLevelPlayer::Attack);
		PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &AFirstLevelPlayer::Running);
		PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &AFirstLevelPlayer::StopRunning);
		PlayerInputComponent->BindAction(TEXT("magic"), IE_Pressed, this, &AFirstLevelPlayer::Magical);

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

	//UE_LOG(LogTemp, Warning, TEXT("%d"), potal->isPlayEndingAnimation);
	if (potal->isPlayEndingAnimation)
	{
		mainCharacter->AddMovementInput(FVector(0, 0, 0), axis.X);
		mainCharacter->AddMovementInput(FVector(0, 0, 0), axis.Y);
	}
	else
	{
		mainCharacter->AddMovementInput(FVector(1, 0, 0), axis.X);
		mainCharacter->AddMovementInput(FVector(0, 1, 0), axis.Y);

	}

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
	//double x, y;
	//playerController->GetMousePosition(x, y);

	if (isForwardMove)
	{
		//helper->SetMousePos(FVector2D(x, y), EMouseType::START);
	}
	else
	{
		//helper->SetMousePos(FVector2D(x, y), EMouseType::END);
		helper->SetIsInteractToItem(true);

		FTimerHandle timer;
		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda(
			[this]() -> void {
				UE_LOG(LogTemp, Warning, TEXT("SetIsReadyToInteract False"));
				helper->SetIsReadyToInteract(false);
			}
		);

		GetWorld()->GetTimerManager().SetTimer(timer, timerDelegate, 0.3, false);
	}

	helper->SetIsForwardMove(isForwardMove);
}
void AFirstLevelPlayer::Jumping()
{
	if (potal->isPlayEndingAnimation)
	{
		return;
	}
	else
	{
		mainCharacter->InputJump();

	}
}

void AFirstLevelPlayer::Attack()
{
	mainCharacter->InputAttack();

}

void AFirstLevelPlayer::Running()
{
	mainCharacter->InputRun();

}

void AFirstLevelPlayer::StopRunning()
{
	mainCharacter->Walk();

}

void AFirstLevelPlayer::Magical()
{
	mainCharacter->InputMagic();
}

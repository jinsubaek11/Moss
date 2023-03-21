#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Helper.h"
#include "FirstLevelPlayer.generated.h"

UCLASS()
class MOSS_API AFirstLevelPlayer : public APawn
{
	GENERATED_BODY()

public:
	AFirstLevelPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Turn(const FInputActionValue& Values);
	void Move(const FInputActionValue& Values);
	void SetHelperActivate(const FInputActionValue& Values);
	void SetHelperMove(const FInputActionValue& Values);
	

public:
	UPROPERTY()
	class AMainCharacter* mainCharacter;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Input;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Mouse;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_HelperMouse;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_HelperMove;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Quit;


	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
	class USceneComponent* rootComp;
	UPROPERTY(VisibleAnywhere, Category = "VRCamera")
	class UCameraComponent* VRCamera;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* LeftHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class UMotionControllerComponent* RightHand;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class USkeletalMeshComponent* LeftHandMesh;
	UPROPERTY(VisibleAnywhere, Category = "MotionController")
	class USkeletalMeshComponent* RightHandMesh;
	
	FRotator prevRot;
	FRotator curRot;

	UPROPERTY()
	class AHelper* helper;
	UPROPERTY()
	class APlayerController* playerController;

	UPROPERTY()
	class APotal* potal;

	void Jumping();
	void Attack();
	void Running();
	void StopRunning();
	void Magical();
	void Exit();

	//UPROPERTY(EditAnywhere)
	//class UWidgetComponent* endingUI;

	
};

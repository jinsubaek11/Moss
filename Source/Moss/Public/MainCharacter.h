#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MainCharacter.generated.h"

UCLASS()
class MOSS_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMainCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//점프
	void InputJump();

	//무기를 넣고싶다
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	//현재체력
	UPROPERTY(EditDefaultsOnly)
	int32 hp;
	//처음체력
	UPROPERTY(EditDefaultsOnly)
	int32 initialHp = 10;
	
	//공격당했을때
	void OnHitEvent();

	//공격
	void InputAttack();
	

};

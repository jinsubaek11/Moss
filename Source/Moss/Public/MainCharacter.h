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

	//����
	void InputJump();

	//���⸦ �ְ�ʹ�
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	//����ü��
	UPROPERTY(EditDefaultsOnly)
	int32 hp;
	//ó��ü��
	UPROPERTY(EditDefaultsOnly)
	int32 initialHp = 10;
	
	//���ݴ�������
	void OnHitEvent();

	//����
	void InputAttack();
	

};

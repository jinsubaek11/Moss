// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnim.h"
#include "Maincharacter.h"
#include <GameFramework/CharacterMovementComponent.h>


void UMainCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	//�÷��̾��� �̵��ӵ��� ������ speed�� �Ҵ�
	auto ownerPawn = TryGetPawnOwner();
	//2. �÷��̾�� ĳ����
	auto player = Cast<AMainCharacter>(ownerPawn);
	if (player)
	{
		//3. �̵��ӵ� �ʿ�
		FVector velocity = player->GetVelocity();
		//4. �÷��̾��� ���� ���Ͱ� �ʿ�
		FVector forwardVector = player->GetActorForwardVector();
		//5. speed�� �� �Ҵ��ϱ�
		speed = FVector::DotProduct(forwardVector, velocity);
		//6. �¿� �ӵ� �Ҵ��ϱ�
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		//�÷��̾ ���߿� �ִ��� 
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}

}

void UMainCharacterAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}

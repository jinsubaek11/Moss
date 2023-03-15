// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnim.h"
#include "Maincharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Potal.h"


void UMainCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	//플레이어의 이동속도를 가져와 speed에 할당
	auto ownerPawn = TryGetPawnOwner();
	//2. 플레이어로 캐스팅
	auto player = Cast<AMainCharacter>(ownerPawn);
	if (player)
	{
		//3. 이동속도 필요
		FVector velocity = player->GetVelocity();
		//4. 플레이어의 전방 벡터가 필요
		FVector forwardVector = player->GetActorForwardVector();
		//5. speed에 값 할당하기
		speed = FVector::DotProduct(forwardVector, velocity);
		//6. 좌우 속도 할당하기
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);

		//플레이어가 공중에 있는지 
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();
	}
}

void UMainCharacterAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}

void UMainCharacterAnim::PlayFinishAnim()
{
	Montage_Play(finishAnimMontage);
}

void UMainCharacterAnim::PlayMagicAnim()
{
	Montage_Play(magicAnimMontage);
}

void UMainCharacterAnim::PlayDeathAnim()
{
	Montage_Play(DeathMontage);
}

void UMainCharacterAnim::AnimNotify_FinishEnd()
{
	APotal* portal = Cast<APotal>(UGameplayStatics::GetActorOfClass(GetWorld(), APotal::StaticClass()));
	if (portal)
	{
		portal->PlayGateAnimation();
		portal->isPlayEndingAnimation = false;
		//FTimerHandle timer;
		//FTimerDelegate timerDelegate;
		//timerDelegate.BindLambda([&]() -> void {
		//	portal->isPlayEndingAnimation = false;
		//});

		//GetWorld()->GetTimerManager().SetTimer(timer, timerDelegate, 0.5, false);
	}
}

void UMainCharacterAnim::AnimNotify_MagicEnd()
{
	isMagicEnd=true;
}

void UMainCharacterAnim::AnimNotify_DeathEnd()
{
	isDeathEnd = true;
	UGameplayStatics::OpenLevel(this, TEXT("Three"));

}

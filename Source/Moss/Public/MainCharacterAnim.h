// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"git 
#include "MainCharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class MOSS_API UMainCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	float speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
	float direction = 0;

	//매프레임 갱신되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	//공중여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerAnim)
	bool isInAir = false;
	
	//재생할 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category=PlayerAnim)
	class UAnimMontage* attackAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* finishAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
	class UAnimMontage* magicAnimMontage;

	//공격 애니메이션 재생함수
	void PlayAttackAnim();
	void PlayFinishAnim();
	void PlayMagicAnim();

	UFUNCTION()
	void AnimNotify_FinishEnd();
	UFUNCTION()
	void AnimNotify_MagicEnd();

	bool isMagicEnd=true;
};

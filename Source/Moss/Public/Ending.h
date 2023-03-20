// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Ending.generated.h"

/**
 * 
 */
UCLASS()
class MOSS_API UEnding : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION()
	void QuitVRGame();
	
};

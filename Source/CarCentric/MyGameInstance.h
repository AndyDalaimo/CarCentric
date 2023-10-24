// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CARCENTRIC_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UMyGameInstance(const FObjectInitializer& ObjectInitializer);

	// Check if References are valid
	virtual void Init();

private:

	FTimerHandle GameTimer;
	FTimerDelegate TimerDelegate;
	float timerRate = 1.f;
	int32 seconds = 0;

	UFUNCTION()
	void TimerFunction();
};

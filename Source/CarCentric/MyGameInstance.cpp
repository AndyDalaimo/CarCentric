
// Fill out your copyright notice in the Description page of Project Settings.

#include "TimerManager.h"
#include "MyGameInstance.h"


UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	// UE_LOG(LogTemp, Warning, TEXT("MY GAME INSTANCE"));
	

}


void UMyGameInstance::Init()
{
	TimerDelegate.BindUFunction(this, "TimerFunction");
	GetWorld()->GetTimerManager().SetTimer(GameTimer, TimerDelegate, timerRate, true);
}

// Function Called from timer 
void UMyGameInstance::TimerFunction()
{
	UE_LOG(LogTemp, Warning, TEXT("Timer function is called"));
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "TimerManager.h"
#include "Misc/Timecode.h"
#include "MyGameInstance.h"


UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	// UE_LOG(LogTemp, Warning, TEXT("MY GAME INSTANCE"));
	

}

// Initialize Timer
void UMyGameInstance::Init()
{
	TimerDelegate.BindUFunction(this, "TimerFunction");
	GetWorld()->GetTimerManager().SetTimer(GameTimer, TimerDelegate, timerRate, true);
}

// Function Called from timer 
void UMyGameInstance::TimerFunction()
{
	seconds++;
	totalTime--;

	float elapsed = GetWorld()->GetTimerManager().GetTimerElapsed(GameTimer);
	FTimecode timer = FTimecode(0, 0, seconds, 0, true);
	UE_LOG(LogTemp, Warning, TEXT("Timer function is called"));
	FString timeString = timer.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Timer elapsed %s"), *timeString);
	UE_LOG(LogTemp, Warning, TEXT("Time Left: %d"), totalTime);
	if (totalTime <= 0)
	{
		GetTimerManager().ClearTimer(GameTimer);
	}
	if (!GameTimer.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Cleared"));

	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"

#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Misc/Timecode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HUDFinder(TEXT("/Script/Engine.UserWidget'/Game/UI/WBP_HUD'"));

	if (!HUDFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Nope"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Yep"));
		HUDUIWidgetClass = HUDFinder.Class;
	}
}

// Initialize Timer
void UMyGameInstance::Init()
{
	// UE_LOG(LogTemp, Warning, TEXT("Widget Class Found: %s"), *HUDUIWidgetClass->GetName());

	TimerDelegate.BindUFunction(this, "TimerFunction");
	GetWorld()->GetTimerManager().SetTimer(GameTimer, TimerDelegate, timerRate, true);
	if (HUDUIWidgetClass) ShowHUDUIWidget();

	// ShowHUDUIWidget();
}

// Function Called from timer 
void UMyGameInstance::TimerFunction()
{
	seconds++;
	totalTime--;

	float elapsed = GetWorld()->GetTimerManager().GetTimerElapsed(GameTimer);
	FTimecode timer = FTimecode(0, 0, seconds, 0, true);
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


void UMyGameInstance::ShowHUDUIWidget()
{
	UUserWidget* HUDUI = CreateWidget<UUserWidget>(this, *HUDUIWidgetClass);
	HUDUI->AddToViewport(999999);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, HUDUI ? HUDUI->GetName() : "Not valid");
}

void UMyGameInstance::ExitHUDUIWidget()
{

}
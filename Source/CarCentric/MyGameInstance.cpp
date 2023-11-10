
// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"

#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Misc/Timecode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


UMyGameInstance::UMyGameInstance(const FObjectInitializer& ObjectInitializer)
{

	static ConstructorHelpers::FClassFinder<UUserWidget> HUDFinder(TEXT("/Script/Engine.UserWidget'/Game/UI/WBP_HUD'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverFinder(TEXT("/Game/UI/WBP_GameOver"));

	if (!HUDFinder.Succeeded() && !GameOverFinder.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("UI NOT FOUND"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UI FOUND"));
		HUDUIWidgetClass = HUDFinder.Class;
		GameOverWidgetClass = GameOverFinder.Class;
	}
}

UMyGameInstance::~UMyGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Instance Destroyed"));
}

// Initialize Timer
// Set default values for totalTime as well as validating widgets in game
void UMyGameInstance::Init()
{

	UE_LOG(LogTemp, Warning, TEXT("Widget Class Found: %s"), *HUDUIWidgetClass->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Widget Class Found: %s"), *GameOverWidgetClass->GetName());

	// FOR TESTING SET TOTALTIME TO 99999 ELSE 10
	// totalTime = 99999;
	totalTime = 5;

	TimerDelegate.BindUFunction(this, "TimerFunction");
	GetWorld()->GetTimerManager().SetTimer(GameTimer, TimerDelegate, timerRate, true);
	if (HUDUIWidgetClass) ShowHUDUIWidget();

}

// Function Called from timer 
// Format time into usable string for HUD UI
// Exit and invalidate timer when reaching Zero. Exit UI and call Game Over
void UMyGameInstance::TimerFunction()
{
	seconds++;
	totalTime--;

	float elapsed = GetWorld()->GetTimerManager().GetTimerElapsed(GameTimer);
	FTimecode timer = FTimecode(0, 0, seconds, 0, true);
	FString timeString = timer.ToString();

	// TESTING -- Shows game timer on output log (Is displayed on UI)
	// UE_LOG(LogTemp, Warning, TEXT("Timer elapsed %s"), *timeString);
	// UE_LOG(LogTemp, Warning, TEXT("Time Left: %d"), totalTime);

	if (totalTime <= 0)
	{
		GetTimerManager().ClearTimer(GameTimer);
	}
	if (!GameTimer.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Timer Cleared"));

		// Game over. Pause timer and remove input from player (Except Mouse/UI Input)
		ExitHUDUIWidget();
	}
}

// Function Called from BP_TimePowerup. Before destroying actor, totalTime 
// will increase and add time to in game timer 
void UMyGameInstance::TimePowerupCollect()
{
	totalTime += 5;
}

// Add Timer to Player screen. 
void UMyGameInstance::ShowHUDUIWidget()
{
	UUserWidget* HUDUI = CreateWidget<UUserWidget>(this, *HUDUIWidgetClass);
	HUDUI->AddToViewport();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, HUDUI ? HUDUI->GetName() : "Not valid");
	
}


// Game Over. Player Only has mouse input and timer has run out. This does
// remove all Widgets
void UMyGameInstance::ExitHUDUIWidget()
{
	// Remove Game Timer UI (Game Over)
	UWidgetLayoutLibrary::RemoveAllWidgets(GetWorld());

	// Reference to Player Controller
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	// Set Up Input Parameters
	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	// Set Input Mode
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	// Show GameOver Screen
	ShowGameOverUIWidget();
}

// Game Over screen. Prompt player to restart level or Quit game
void UMyGameInstance::ShowGameOverUIWidget()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Game Over UI");

	// Create widget and add to screen
	UUserWidget* GameOverUI = CreateWidget<UUserWidget>(this, *GameOverWidgetClass);
	GameOverUI->AddToViewport();

}

// Called in From GameOver UI. Restart game time and give player control of Player
void UMyGameInstance::RestartGame()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();

	// Set up input parameters for player controller in Game
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	

	// Set Input Mode
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;

	// Restart Game Timer
	Init();
}




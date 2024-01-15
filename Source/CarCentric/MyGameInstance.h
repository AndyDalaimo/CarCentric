// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
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
	~UMyGameInstance() override;
	
	// Check if References are valid
	virtual void Init();

	// Create Widget and Show Game Timer
	UFUNCTION(BlueprintCallable)
	void ShowHUDUIWidget();
	
	// Remove All widgets and remove move input from player
	UFUNCTION(BlueprintCallable)
	void ExitHUDUIWidget();

	// Create Widget and Show Game Over Screen
	UFUNCTION(BlueprintCallable)
	void ShowGameOverUIWidget();

	// Restart Game
	UFUNCTION(BlueprintCallable)
	void RestartGame();

	// Player Collected time powerup in game world
	UFUNCTION(BlueprintCallable)
	void TimePowerupCollect(int32 increase);

	UFUNCTION()
	void SetTimerRate(float rate);

	// In Game Timer shown on HUD
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 totalTime;

	UPROPERTY()
	int32 seconds = 0;

	UPROPERTY()
	float timerRate = 1.f;

private:

	// Properties for in game timer
	FTimerHandle GameTimer;
	FTimerDelegate TimerDelegate;

	// HUD with Game Timer
	TSubclassOf<UUserWidget> HUDUIWidgetClass;

	// Game Over UI
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	// In Game timer started on Start Game 
	UFUNCTION()
	void TimerFunction();




};

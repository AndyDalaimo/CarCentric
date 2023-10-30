// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CarCentricCharacter.h"
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

	UFUNCTION(BlueprintCallable)
	void ShowHUDUIWidget();
	
	UFUNCTION(BlueprintCallable)
	void ExitHUDUIWidget();

	UFUNCTION(BlueprintNativeEvent)
	void RemoveGameTimer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 totalTime = 10;

private:


	FTimerHandle GameTimer;
	FTimerDelegate TimerDelegate;
	float timerRate = 1.f;
	int32 seconds = 0;

	// In Game timer started on Start Game 
	UFUNCTION()
	void TimerFunction();

	// HUD with Game Timer
	TSubclassOf<UUserWidget> HUDUIWidgetClass;

};

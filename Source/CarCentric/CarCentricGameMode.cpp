// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarCentricGameMode.h"
#include "CarCentricCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACarCentricGameMode::ACarCentricGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

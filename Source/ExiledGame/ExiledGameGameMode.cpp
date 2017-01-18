// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ExiledGame.h"
#include "ExiledGameGameMode.h"
#include "ExiledGamePlayerController.h"
#include "ExiledGameCharacter.h"

AExiledGameGameMode::AExiledGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AExiledGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
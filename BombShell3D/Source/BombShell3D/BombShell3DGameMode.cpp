// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BombShell3DGameMode.h"
#include "BombShell3DCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABombShell3DGameMode::ABombShell3DGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

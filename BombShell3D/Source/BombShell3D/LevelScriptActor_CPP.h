// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScriptActor_CPP.generated.h"

UCLASS()
class BOMBSHELL3D_API ALevelScriptActor_CPP : public ALevelScriptActor
{
	GENERATED_BODY()
	
private:
	static const int ROWS = 21;
	static const int COLUMNS = 11;
	static const int HEIGHT = 7;
	int BaseX, BaseY, BaseZ;
	FVector Coos[ROWS][COLUMNS][HEIGHT];

public:
	UFUNCTION(BlueprintCallable)
		void GenerateArena();

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	UClass* DestructibleCube_BP;
	
};

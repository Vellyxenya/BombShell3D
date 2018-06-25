// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SelectionCube_CPP.h"
#include "ModeSelectionCube_CPP.generated.h"

UENUM()
enum Options {
	AgainstFriend,
	Matchmaking
};

UCLASS()
class BOMBSHELL3D_API AModeSelectionCube_CPP : public ASelectionCube_CPP
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AModeSelectionCube_CPP();

	int32 CurrentOptionIndex = 0;
	TArray<FString> PlayOptions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotateAntiClockwise() override;
	void RotateClockwise() override;

	UCubeArrow_CPP* RightArrow;
	UCubeArrow_CPP* LeftArrow;

private:
	FRotator Rotator;
	FVector PlayerPosition;
	
	void LoopIndex();
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeSelectionCube_CPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

AModeSelectionCube_CPP::AModeSelectionCube_CPP() {
	NumberOfOptions = 3; //Matchmaking, Against Friend, 3rd Option
	PrimaryActorTick.bCanEverTick = true;
	/*RightArrow = NewObject<UCubeArrow_CPP>(this, UCubeArrow_CPP::StaticClass(), TEXT("RightArrowp"));
	LeftArrow = NewObject<UCubeArrow_CPP>(this, UCubeArrow_CPP::StaticClass(), TEXT("LeftArrowp"));*/
	PlayOptions.Add(TEXT("Matchmaking"));
	PlayOptions.Add(TEXT("Against Friend"));
	PlayOptions.Add(TEXT("Other"));
	NumberOfOptions = PlayOptions.Num();
}

void AModeSelectionCube_CPP::BeginPlay() {
	Super::BeginPlay();
}

void AModeSelectionCube_CPP::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	Rotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPosition);
	SetActorRotation(Rotator);
}

void AModeSelectionCube_CPP::RotateAntiClockwise() {
	Super::RotateAntiClockwise();
	//UE_LOG(LogTemp, Warning, TEXT("Child anti"))
	CurrentOptionIndex--;
	LoopIndex();
	UE_LOG(LogTemp, Warning, TEXT("Current Index : %i"), CurrentOptionIndex);
}

void AModeSelectionCube_CPP::RotateClockwise() {
	Super::RotateClockwise();
	//UE_LOG(LogTemp, Warning, TEXT("Child anti"))
	CurrentOptionIndex++;
	LoopIndex();
	UE_LOG(LogTemp, Warning, TEXT("Current Index : %i"), CurrentOptionIndex);
}

void AModeSelectionCube_CPP::LoopIndex() {
	if (CurrentOptionIndex >= NumberOfOptions) {
		CurrentOptionIndex = 0;
	} else if (CurrentOptionIndex < 0) {
		CurrentOptionIndex = NumberOfOptions - 1;
	}
}
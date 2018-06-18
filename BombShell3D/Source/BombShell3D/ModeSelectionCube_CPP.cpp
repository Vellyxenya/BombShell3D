// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeSelectionCube_CPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

AModeSelectionCube_CPP::AModeSelectionCube_CPP() {
	UE_LOG(LogTemp, Warning, TEXT("arrowp"))
	PrimaryActorTick.bCanEverTick = true;
	/*RightArrow = NewObject<UCubeArrow_CPP>(this, UCubeArrow_CPP::StaticClass(), TEXT("RightArrowp"));
	LeftArrow = NewObject<UCubeArrow_CPP>(this, UCubeArrow_CPP::StaticClass(), TEXT("LeftArrowp"));*/
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



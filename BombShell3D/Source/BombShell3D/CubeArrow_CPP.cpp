// Fill out your copyright notice in the Description page of Project Settings.

#include "CubeArrow_CPP.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UCubeArrow_CPP::UCubeArrow_CPP()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	OnBeginCursorOver.AddDynamic(this, &UCubeArrow_CPP::OnHovered);
}

void UCubeArrow_CPP::OnHovered(UPrimitiveComponent* Component) {
	UE_LOG(LogTemp, Warning, TEXT("Hovered on!"));
}
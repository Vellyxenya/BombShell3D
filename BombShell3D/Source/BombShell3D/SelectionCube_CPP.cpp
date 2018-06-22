// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectionCube_CPP.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"

// Sets default values
ASelectionCube_CPP::ASelectionCube_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ASelectionCube_CPP::BeginPlay() {
	Super::BeginPlay();
	
}

void ASelectionCube_CPP::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ASelectionCube_CPP::RotateAntiClockwise() {
	UE_LOG(LogTemp, Warning, TEXT("RotateAntiClockwise!"));
	TArray<UStaticMeshComponent*> Components;
	GetComponents(Components);
	if (Components.Num() > 0)
	{
		for (UStaticMeshComponent* FoundComp : Components) {
			//FoundComp = Components[0];
			//UE_LOG(LogTemp, Warning, TEXT("found the following : %s"), *FoundComp->GetName());
			if (FoundComp->GetName() == FString("Cube")) {
				//UE_LOG(LogTemp, Warning, TEXT("found the following : %s"), *FoundComp->GetName());
				//FoundComp->SetRelativeRotation(FRotator(45,45, 68));
				OnRotateAntiClockwise.Broadcast();
				//UE_LOG(LogTemp, Warning, TEXT("Broadcaasting anticlockwise"));
			}
		}
		//UStaticMeshComponent* FoundComp = Components[0];
		//FoundComp->GetName();
		//UE_LOG(LogTemp, Warning, TEXT("found the following : %s"), *FoundComp->GetName());
	}
	//GetComponentsByTag(TSubclassOf<UActorComponent> UStaticMeshComponent, FName("Core"));
}

void ASelectionCube_CPP::RotateClockwise() {
	UE_LOG(LogTemp, Warning, TEXT("RotateClockwise!"));
	OnRotateClockwise.Broadcast();
}
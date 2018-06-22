// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeArrow_CPP.h"

#include "SelectionCube_CPP.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotateAntiClockwise);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotateClockwise);

UCLASS()
class BOMBSHELL3D_API ASelectionCube_CPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionCube_CPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RotateAntiClockwise();
	void RotateClockwise();

	UPROPERTY(BlueprintAssignable)
		FOnRotateAntiClockwise OnRotateAntiClockwise;

	UPROPERTY(BlueprintAssignable)
		FOnRotateClockwise OnRotateClockwise;

	UCubeArrow_CPP* RightArrow;
	UCubeArrow_CPP* LeftArrow;
};

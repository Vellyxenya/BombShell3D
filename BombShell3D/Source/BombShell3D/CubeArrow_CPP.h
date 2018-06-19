// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "CubeArrow_CPP.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FComponentBeginCursorOverSignature);

UCLASS(meta=(BlueprintSpawnableComponent))
class BOMBSHELL3D_API UCubeArrow_CPP : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCubeArrow_CPP();

	UPROPERTY(EditAnywhere)
		FString ThisName;

	UFUNCTION()
		void OnHovered(UPrimitiveComponent* Component);
};

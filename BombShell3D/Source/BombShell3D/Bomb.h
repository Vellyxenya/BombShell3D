// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExplode);

UCLASS()
class BOMBSHELL3D_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	void Lock();

	UFUNCTION(BlueprintCallable)
		void CreateExplosions();

	FVector Location;

	UFUNCTION()
		void OnOverlap(AActor* ThisActor, AActor* OtherActor);

	UPROPERTY(BlueprintAssignable)
		FOnExplode OnExplode;

	UPROPERTY(EditDefaultsOnly, Category = ExplosionSetup)
		UClass* ExplosionRay_BP;

	UPROPERTY(EditDefaultsOnly, Category = RotationTest)
		float Pitch;

	UPROPERTY(EditDefaultsOnly, Category = RotationTest)
		float Roll;

	UPROPERTY(EditDefaultsOnly, Category = RotationTest)
		float Yaw;
};


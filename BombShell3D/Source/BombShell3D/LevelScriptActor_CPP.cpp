// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelScriptActor_CPP.h"
#include "Engine/World.h"
#include "DestructibleCube_CPP.h"

void ALevelScriptActor_CPP::GenerateArena() {
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			for (int k = 0; k < HEIGHT; k++) {
				Coos[i][j][k] = FVector(BaseX+200*i, BaseX + 200 * j, BaseX + 200 * k);
				if (FMath::RandRange(0, 9) >= 4) {
					World->SpawnActor<ADestructibleCube_CPP>(DestructibleCube_BP)->SetActorLocation(Coos[i][j][k]);
				}
			}
		}
	}
}
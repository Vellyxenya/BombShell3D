// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "BombShellGameInstance.generated.h"

UCLASS()
class BOMBSHELL3D_API UBombShellGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
public:
	UBombShellGameInstance(const FObjectInitializer & ObjectInitializer);
	
	virtual void Init();

	UFUNCTION(BlueprintCallable)
		void LoadMainMenu();

	UFUNCTION()
		void LoadSelectionMenu() override;

	UFUNCTION(BlueprintCallable)
		void DisplayUI();

	UFUNCTION(BlueprintCallable)
		bool AskPlayerController();
	
private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	class UMainMenu_CPP* MainMenu;

	TSubclassOf<class UUserWidget> UIClass;
	class UUI_CPP* UI;
};

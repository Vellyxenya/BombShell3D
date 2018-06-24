// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "BombShellGameInstance.generated.h"

UENUM()
enum GameStatus
{
	MainMenu		UMETA(DisplayName = "MainMenu"),
	SelectionMenu	UMETA(DisplayName = "SelectionMenu"),
	Playing			UMETA(DisplayName = "Playing"),
};

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
		void DisplaySelectionUI();

	UFUNCTION(BlueprintCallable)
		bool AskPlayerController();

	void SetGameStatus(enum GameStatus Status);
	enum GameStatus GetGameStatus();

private:
	enum GameStatus CurrentStatus;

	TSubclassOf<class UUserWidget> MainMenuClass;
	class UMainMenu_CPP* MainMenu;

	TSubclassOf<class UUserWidget> UIClass;
	TSubclassOf<class UUserWidget> Selection_UI_Class;

	class UUI_CPP* UI;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h" //We can't forward declare this class cause it's a pointer
#include "OnlineSessionInterface.h"
#include "OnlineFriendsInterface.h"
#include "MenuInterface.h"
#include "BombShellGameInstance.generated.h"

class UTexture2D;

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
	UBombShellGameInstance();
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

	UFUNCTION()
		void ReadListComplete(int32 number, bool Success, const FString& var1, const FString& var2);

	void SetGameStatus(enum GameStatus Status);
	enum GameStatus GetGameStatus();

private:
	enum GameStatus CurrentStatus;

	TSubclassOf<class UUserWidget> MainMenuClass;
	class UMainMenu_CPP* MainMenu;

	TSubclassOf<class UUserWidget> UIClass;
	TSubclassOf<class UUserWidget> Selection_UI_Class;

	class UUI_CPP* UI;

	IOnlineSessionPtr SessionInterface;
	IOnlineFriendsPtr FriendsInterface;

	FOnReadFriendsListComplete FriendListReadCompleteDelegate;
	APlayerController* PlayerController;

	IOnlineSubsystem* OnlineSubsystem;
	void SetupOnlineSystem(class APlayerController *PlayerController);

	//UTexture2D* GetSteamAvatar(const FBPUniqueNetId UniqueNetId);
};
// Fill out your copyright notice in the Description page of Project Settings.

#include "BombShellGameInstance.h"
#include "MainMenu_CPP.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI_CPP.h"
#include "PlayerController_CPP.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "UnrealNames.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/Texture2D.h"

//const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SESSION_NAME = EName::NAME_GameSession; //This fixes the engine bug in which public connections doesn't get updated
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

class FOnlineUserPresence;

UBombShellGameInstance::UBombShellGameInstance(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menus/MainMenu_BP"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MainMenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> UIBPClass(TEXT("/Game/Display/UI/BombingGun_UI"));
	if (!ensure(UIBPClass.Class != nullptr)) return;
	UIClass = UIBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> Selection_UI(TEXT("/Game/Display/UI/Selection_UI"));
	if (!ensure(Selection_UI.Class != nullptr)) return;
	Selection_UI_Class = Selection_UI.Class;

	FriendListReadCompleteDelegate = FOnReadFriendsListComplete::CreateUObject(this, &UBombShellGameInstance::ReadListComplete);
}

void UBombShellGameInstance::SetGameStatus(enum GameStatus Status) {
	UE_LOG(LogTemp, Warning, TEXT("Setting game status"))
	CurrentStatus = Status;
}

GameStatus UBombShellGameInstance::GetGameStatus() {
	return CurrentStatus;
}

void UBombShellGameInstance::Init() {
	Super::Init();
	OnlineSubsystem = IOnlineSubsystem::Get(FName("Steam"));
	
	//UE_LOG(LogTemp, Warning, TEXT("MainMenuClassName : %s"), *MainMenuClass->GetName());
	/*IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(FName("Steam"));
	if (OnlineSubsystem != nullptr) {
		FriendsInterface = OnlineSubsystem->GetFriendsInterface();
		int32 PlayerId = 0;
		if (FriendsInterface.IsValid()) {
			MyDelegate.BindUFunction(this, "ReadListComplete");
			//FriendsInterface->ReadFriendsList(PlayerId, EFriendsLists::ToString(EFriendsLists::Default), MyDelegate);

			UE_LOG(LogTemp, Warning, TEXT("in if is valid"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("in if is not valid"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is a nullptr, try launching a standalone game or from PowerShell"));
	}*/

	//TArray< TSharedRef<FOnlineFriend> > FriendList;
	//FriendListReadCompleteDelegate(FOnReadFriendsListComplete::CreateUObject(this, &ThisClass::OnReadFriendsListCompleted))
	/*if (OnlineSubsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem : %s"), *OnlineSubsystem->GetSubsystemName().ToString());
		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			//SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzleGameInstance::OnCreateSessionComplete);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is a nullptr"));
	}*/
	
}

void UBombShellGameInstance::ReadListComplete(int32 LocalUserNumber, bool Success, const FString& ListName, const FString& Error) {
	if (Success) {
		UE_LOG(LogTemp, Warning, TEXT("Read friendlist complete"));

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Read friendlist failed"));
	}
}

bool UBombShellGameInstance::AskPlayerController() {
	/*APlayerController* PlayerController = GetFirstLocalPlayerController();
	//if (!ensure(PlayerController != nullptr)) return false;
	//return Cast<APlayerController_CPP>(PlayerController)->bCanPutBomb;
	if (PlayerController != nullptr) {
		return Cast<APlayerController_CPP>(PlayerController)->bCanPutBomb;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No player controller found!"));
		return false;
	}*/
	return false;
}

void UBombShellGameInstance::DisplayUI() {

	if (!ensure(UIClass != nullptr)) return;
	UI = CreateWidget<UUI_CPP>(this, UIClass);
	if (!ensure(UI != nullptr)) return;
	UI->AddToViewport(2);
	UE_LOG(LogTemp, Warning, TEXT("DISPLAY UI"));
	
}

void UBombShellGameInstance::DisplaySelectionUI() {
	if (!ensure(Selection_UI_Class != nullptr)) return;
	UI = CreateWidget<UUI_CPP>(this, Selection_UI_Class);
	if (!ensure(UI != nullptr)) return;
	UI->AddToViewport(2);
	UE_LOG(LogTemp, Warning, TEXT("DISPLAYING SELECTION UI"));
}

void UBombShellGameInstance::LoadMainMenu() {
	if (!ensure(MainMenuClass != nullptr)) return;
	MainMenu = CreateWidget<UMainMenu_CPP>(this, MainMenuClass);
	if (!ensure(MainMenu != nullptr)) return;

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UBombShellGameInstance::LoadSelectionMenu() {
	PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
	
	//FInputModeGameOnly Inputmode;
	FInputModeGameOnly Inputmode;
	PlayerController->SetInputMode(Inputmode);
	SetGameStatus(GameStatus::SelectionMenu);
	SetupOnlineSystem(PlayerController);
	PlayerController->ClientTravel("/Game/Maps/SelectionMenu", ETravelType::TRAVEL_Absolute);
}

void UBombShellGameInstance::SetupOnlineSystem(APlayerController *PlayerController) {
	if (OnlineSubsystem != nullptr) {
		FriendsInterface = OnlineSubsystem->GetFriendsInterface();
		if (FriendsInterface.IsValid()) {
			UE_LOG(LogTemp, Warning, TEXT("friendinterface valid"));

			//PlayerController = Cast<APlayerController_CPP>(GetWorld()->GetFirstPlayerController());
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController != nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Playercontroller not nullptr"));
				ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
				if (LocalPlayer) {
					FriendsInterface->ReadFriendsList(LocalPlayer->GetControllerId(), EFriendsLists::ToString((EFriendsLists::Default)), FriendListReadCompleteDelegate);
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Playercontroller is nullptr"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("friendinterface not valid"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("OnlineSubsystem is a nullptr, try launching a standalone game or from PowerShell"));
	}
}
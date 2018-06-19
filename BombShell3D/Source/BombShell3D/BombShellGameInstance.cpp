// Fill out your copyright notice in the Description page of Project Settings.

#include "BombShellGameInstance.h"
#include "MainMenu_CPP.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI_CPP.h"
#include "PlayerController_CPP.h"

UBombShellGameInstance::UBombShellGameInstance(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menus/MainMenu_BP"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MainMenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> UIBPClass(TEXT("/Game/Display/UI/BombingGun_UI"));
	if (!ensure(UIBPClass.Class != nullptr)) return;
	UIClass = UIBPClass.Class;
}

void UBombShellGameInstance::SetGameStatus(enum GameStatus Status) {
	CurrentStatus = Status;
}

GameStatus UBombShellGameInstance::GetGameStatus() {
	return CurrentStatus;
}

void UBombShellGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("MainMenuClassName : %s"), *MainMenuClass->GetName());
}

bool UBombShellGameInstance::AskPlayerController() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	/*if (!ensure(PlayerController != nullptr)) return false;
	return Cast<APlayerController_CPP>(PlayerController)->bCanPutBomb;*/
	if (PlayerController != nullptr) {
		return Cast<APlayerController_CPP>(PlayerController)->bCanPutBomb;
	} else {
		UE_LOG(LogTemp, Warning, TEXT("No player controller found!"));
		return false;
	}
}

void UBombShellGameInstance::DisplayUI() {
	if (!ensure(UIClass != nullptr)) return;
	UI = CreateWidget<UUI_CPP>(this, UIClass);
	if (!ensure(UI != nullptr)) return;
	UI->AddToViewport(2);
	UE_LOG(LogTemp, Warning, TEXT("DISPLAY UI"));
}

void UBombShellGameInstance::LoadMainMenu() {
	if (!ensure(MainMenuClass != nullptr)) return;
	MainMenu = CreateWidget<UMainMenu_CPP>(this, MainMenuClass);
	if (!ensure(MainMenu != nullptr)) return;

	MainMenu->Setup();
	MainMenu->SetMenuInterface(this);
}

void UBombShellGameInstance::LoadSelectionMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;
	
	//FInputModeGameOnly Inputmode;
	FInputModeGameOnly Inputmode;
	PlayerController->SetInputMode(Inputmode);
	PlayerController->ClientTravel("/Game/Maps/SelectionMenu", ETravelType::TRAVEL_Absolute);
	SetGameStatus(GameStatus::SelectionMenu);
}


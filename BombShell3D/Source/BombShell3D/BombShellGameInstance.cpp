// Fill out your copyright notice in the Description page of Project Settings.

#include "BombShellGameInstance.h"
#include "MainMenu_CPP.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

UBombShellGameInstance::UBombShellGameInstance(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menus/MainMenu_BP"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MainMenuClass = MenuBPClass.Class;
}

void UBombShellGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("MainMenuClassName : %s"), *MainMenuClass->GetName());
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
	PlayerController->ClientTravel("/Game/Maps/SelectionMenu", ETravelType::TRAVEL_Absolute);
}


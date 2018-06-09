// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu_CPP.h"
#include "Components/Button.h"

bool UMainMenu_CPP::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(MM_PlayButton != nullptr)) return false;
	MM_PlayButton->OnClicked.AddDynamic(this, &UMainMenu_CPP::LoadSelectionMenu);
	if (!ensure(MM_QuitButton != nullptr)) return false;
	MM_QuitButton->OnClicked.AddDynamic(this, &UMainMenu_CPP::QuitToDesktop);

	return true;
}

void UMainMenu_CPP::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}

void UMainMenu_CPP::Setup() {
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

//Opposite of Setup
void UMainMenu_CPP::Teardown() {
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(true);

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
}

void UMainMenu_CPP::QuitToDesktop() {
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FString ConsoleQuitCommand = FString("Quit");
	PlayerController->ConsoleCommand(ConsoleQuitCommand, true);
}

void UMainMenu_CPP::LoadSelectionMenu() {
	if (MenuInterface != nullptr) {
		MenuInterface->LoadSelectionMenu();
	}
}
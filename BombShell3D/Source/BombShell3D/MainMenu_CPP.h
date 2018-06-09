// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu_CPP.generated.h"


UCLASS()
class BOMBSHELL3D_API UMainMenu_CPP : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetMenuInterface(IMenuInterface* MenuInterface);
	void Setup();
	void Teardown();
	
protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* MM_PlayButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MM_TutorialButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MM_SettingsButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MM_CreditsButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* MM_QuitButton;

	UFUNCTION()
		void QuitToDesktop();

	UFUNCTION()
		void LoadSelectionMenu();

	IMenuInterface* MenuInterface;
};

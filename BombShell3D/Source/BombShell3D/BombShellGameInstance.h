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

USTRUCT(BlueprintType)
struct FCustomBlueprintSessionResult
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Blueprint Session Result")
		FString SessionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Blueprint Session Result")
		bool bIsLan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Blueprint Session Result")
		int32 CurrentNumberOfPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Blueprint Session Result")
		int32 MaxNumberOfPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Blueprint Session Result")
		bool bIsPasswordProtected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Blueprint Session Result")
		FString SessionPassword;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Blueprint Session Result")
		int32 Ping;
};


//stolen from Advanced Session Plugin cuz I can't find any other way to put FUniqueNetId in a struct
USTRUCT(BlueprintType)
struct FBPUniqueNetId
{
	GENERATED_USTRUCT_BODY()

private:
	bool bUseDirectPointer;

public:
	TSharedPtr<const FUniqueNetId> UniqueNetId;
	const FUniqueNetId * UniqueNetIdPtr;

	void SetUniqueNetId(const TSharedPtr<const FUniqueNetId> &ID)
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
		UniqueNetId = ID;
	}

	void SetUniqueNetId(const FUniqueNetId *ID)
	{
		bUseDirectPointer = true;
		UniqueNetIdPtr = ID;
	}

	bool IsValid() const
	{
		if (bUseDirectPointer && UniqueNetIdPtr != nullptr)
		{
			return true;
		}
		else if (UniqueNetId.IsValid())
		{
			return true;
		}
		else
			return false;

	}

	const FUniqueNetId* GetUniqueNetId() const
	{
		if (bUseDirectPointer && UniqueNetIdPtr != nullptr)
		{
			// No longer converting to non const as all functions now pass const UniqueNetIds
			return /*const_cast<FUniqueNetId*>*/(UniqueNetIdPtr);
		}
		else if (UniqueNetId.IsValid())
		{
			return UniqueNetId.Get();
		}
		else
			return nullptr;
	}

	FBPUniqueNetId()
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FSteamFriendInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steam Friend Info")
		UTexture2D* PlayerAvatar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steam Friend Info")
		FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Steam Friend Info")
		FBPUniqueNetId PlayerUniqueNetID;
};

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

	/**
	* Delegate fired when the friend list request has been processed
	* @param	LocalUserNum		The local user id (UniqueNetId) of the requesting player
	* @param	bWasSuccessful		true if the async action completed without error, false if there was an error
	* @param	ListName			the friend list name
	* @param	ErrorString			if there is any errors
	*/
	void OnReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString& ListName, const FString& ErrorString);

public:
	/**
	* gets the steam avatar of a player based on his UniqueNetId
	* @Param		UniqueNetId		the UniqueNetId of the player you want to get his avatar
	*/
	UTexture2D* GetSteamAvatar(const FBPUniqueNetId UniqueNetId);

	/**
	* called to get the list of steam friends a player has
	* @Param		PlayerController		the player controller of the player asking for the friend list
	* @Param		FriendsList				list of friends' info in  bluepritn wrapper structure
	*/
	UFUNCTION(BlueprintCallable, Category = "Network|Friends")
		void GetSteamFriendsList(APlayerController *PlayerController);

	/**
	* Called from the delegate when getting the friend list request in completed
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Network|Friends")
		void OnGetSteamFriendRequestCompleteUMG(const TArray<FSteamFriendInfo>& BPFriendsLists);
};
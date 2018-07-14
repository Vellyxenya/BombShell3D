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

//we include the steam api here to be able to get the steam avatar
//refresh your visual studio files from editor after adding this to avoid weird redline errors
#if PLATFORM_WINDOWS || PLATFORM_MAC || PLATFORM_LINUX

#pragma push_macro("ARRAY_COUNT")
#undef ARRAY_COUNT

#include <steam/steam_api.h>

#pragma pop_macro("ARRAY_COUNT")

#endif

//const static FName SESSION_NAME = TEXT("My Session Game");
const static FName SESSION_NAME = EName::NAME_GameSession; //This fixes the engine bug in which public connections doesn't get updated
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

class FOnlineUserPresence;

UBombShellGameInstance::UBombShellGameInstance(const FObjectInitializer & ObjectInitializer) {
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UBombShellGameInstance::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UBombShellGameInstance::OnStartOnlineGameComplete);

	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menus/MainMenu_BP"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MainMenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> UIBPClass(TEXT("/Game/Display/UI/BombingGun_UI"));
	if (!ensure(UIBPClass.Class != nullptr)) return;
	UIClass = UIBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> Selection_UI(TEXT("/Game/Display/UI/Selection_UI"));
	if (!ensure(Selection_UI.Class != nullptr)) return;
	Selection_UI_Class = Selection_UI.Class;

	FriendListReadCompleteDelegate = FOnReadFriendsListComplete::CreateUObject(this, &UBombShellGameInstance::OnReadFriendsListCompleted);
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
	UE_LOG(LogTemp, Warning, TEXT("Loading Main Menu"))
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
			//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController != nullptr) {
				UE_LOG(LogTemp, Warning, TEXT("Playercontroller not nullptr"));
				ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);
				if (LocalPlayer) {
					//This next line seems to be initiating the steam friends query mechanism, and it works, even if not intended ;)
					//FriendsInterface->ReadFriendsList(LocalPlayer->GetControllerId(), EFriendsLists::ToString((EFriendsLists::Default)), FriendListReadCompleteDelegate);
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

void UBombShellGameInstance::StartOnlineGame(FString ServerName, int32 MaxNumPlayers, bool bIsLAN, bool bIsPresence, bool bIsPasswordProtected, FString SessionPassword) {
	// Creating a local player where we can get the UserID from
	ULocalPlayer* const Player = GetFirstGamePlayer();

	// Call our custom HostSession function. GameSessionName is a GameInstance variable
	HostSession(Player->GetPreferredUniqueNetId(), SESSION_NAME /*GameSessionName*/, ServerName, bIsLAN, bIsPresence, MaxNumPlayers, bIsPasswordProtected, SessionPassword);
}

bool UBombShellGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, FString ServerName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers, bool bIsPasswordProtected, FString SessionPassword) {
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		// Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*Fill in all the Session Settings that we want to use.
			There are more with SessionSettings.Set(...);
			For example the Map or the GameMode/Type.*/

			SessionSettings = MakeShareable(new FOnlineSessionSettings());
			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			MaxPlayersInSession = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;
			//setting a value in the FOnlineSessionSetting's settings array
			SessionSettings->Set(SETTING_MAPNAME, LobbyMapName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

			//Making a temporary FOnlineSessionSetting variable to hold the data we want to add to the FOnlineSessionSetting's settings array
			FOnlineSessionSetting ExtraSessionSetting;
			ExtraSessionSetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;

			//setting the temporary data to the ServerName we got from UMG
			ExtraSessionSetting.Data = ServerName;

			//adding the Server Name value in the FOnlineSessionSetting 's settings array using the key defined in header
			//the key can be any FNAME but we define it to avoid mistakes
			SessionSettings->Settings.Add(SETTING_SERVER_NAME, ExtraSessionSetting);

			//setting the temporary data to the bIsPasswordProtected we got from UMG
			ExtraSessionSetting.Data = bIsPasswordProtected;
			//adding the bIsPasswordProtected value in the FOnlineSessionSetting 's settings array using the key defined in header
			SessionSettings->Settings.Add(SETTING_SERVER_IS_PROTECTED, ExtraSessionSetting);


			//setting the temporary data to the Password we got from UMG
			ExtraSessionSetting.Data = SessionPassword;
			//adding the Password value in the FOnlineSessionSetting 's settings array using the key defined in header
			SessionSettings->Settings.Add(SETTING_SERVER_PROTECT_PASSWORD, ExtraSessionSetting);



			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			// Our delegate should get called when this is complete (doesn't need to be successful!)
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No Online Subsytem found!"));
	}
	return false;
}

void UBombShellGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid()) {
			// Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful) {
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				// The StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}

	}
}

void UBombShellGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful) {
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}
	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LobbyMapName, true, "listen");
		UE_LOG(LogTemp, Warning, TEXT("LISTENING!!!"))
	}
}

UTexture2D* UBombShellGameInstance::GetSteamAvatar(const FBPUniqueNetId UniqueNetId) {
	if (UniqueNetId.IsValid()) {
		uint32 Width = 0;
		uint32 Height = 0;

		//get the player iID
		uint64 id = *((uint64*)UniqueNetId.UniqueNetId->GetBytes());

		int Picture = 0;

		// get the Avatar ID using the player ID
		Picture = SteamFriends()->GetMediumFriendAvatar(id);

		//if the Avatar ID is not valid retrun null
		if (Picture == -1)
			return nullptr;

		//get the image size from steam
		SteamUtils()->GetImageSize(Picture, &Width, &Height);

		// if the image size is valid (most of this is from the Advanced Seesion Plugin as well, mordentral, THANK YOU!
		if (Width > 0 && Height > 0) {
			//Creating the buffer "oAvatarRGBA" and then filling it with the RGBA Stream from the Steam Avatar
			uint8 *oAvatarRGBA = new uint8[Width * Height * 4];


			//Filling the buffer with the RGBA Stream from the Steam Avatar and creating a UTextur2D to parse the RGBA Steam in
			SteamUtils()->GetImageRGBA(Picture, (uint8*)oAvatarRGBA, 4 * Height * Width * sizeof(char));

			UTexture2D* Avatar = UTexture2D::CreateTransient(Width, Height, PF_R8G8B8A8);

			// Switched to a Memcpy instead of byte by byte transer
			uint8* MipData = (uint8*)Avatar->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(MipData, (void*)oAvatarRGBA, Height * Width * 4);
			Avatar->PlatformData->Mips[0].BulkData.Unlock();

			// Original implementation was missing this!!
			// the hell man......
			// deallocating the memory used to get the avatar data
			delete[] oAvatarRGBA;

			//Setting some Parameters for the Texture and finally returning it
			Avatar->PlatformData->NumSlices = 1;
			Avatar->NeverStream = true;
			//Avatar->CompressionSettings = TC_EditorIcon;

			Avatar->UpdateResource();

			return Avatar;
		}
	}
	return nullptr;
}

void UBombShellGameInstance::SendSessionInviteToFriend(APlayerController* InvitingPlayer, const FBPUniqueNetId & Friend) {
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub) {
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(InvitingPlayer->GetLocalPlayer());
		if (LocalPlayer) {
			// Get SessionInterface from the OnlineSubsystem
			IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
			if (Sessions.IsValid() && LocalPlayer->GetPreferredUniqueNetId().IsValid() && Friend.IsValid()) {
				Sessions->SendSessionInviteToFriend(*LocalPlayer->GetPreferredUniqueNetId(), /*GameSessionName*/SESSION_NAME, *Friend.GetUniqueNetId());
				UE_LOG(LogTemp, Warning, TEXT("Invite successfully sent"))
			}
		}
	}
}

void UBombShellGameInstance::GetSteamFriendsList(APlayerController *PlayerController)
{
	UE_LOG(LogTemp, Warning, TEXT("In Function : GetSteamFriendsList"))
	//check if the player controller is valid
	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player controller Valid, In getsteamfriendslist"))
		//get the steam online subsystem
		IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get(FName("Steam"));

		//check if the online subsystem is valid
		if (OnlineSub)
		{
			UE_LOG(LogTemp, Warning, TEXT("Online Subsys Valid, In getsteamfriendslist"))
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("Found Steam Online"));
			//get Friends interface
			IOnlineFriendsPtr FriendsInterface = OnlineSub->GetFriendsInterface();

			//if the Friends Interface is valid
			if (FriendsInterface.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("friend interface Valid, In getsteamfriendslist"))

				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Found friend interface"));
				// get the local player from the player controller
				ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->Player);

				//chaeck if the local player exists
				if (LocalPlayer)
				{
					//read the friend list from the online subsystem then call the delagate when completed
					FriendsInterface->ReadFriendsList(LocalPlayer->GetControllerId(), EFriendsLists::ToString((EFriendsLists::InGamePlayers)), FriendListReadCompleteDelegate);
				}
			}
		}
	}
}


// when reading friend list from the online subsystem is finished, get it and store it then call blueprint to show it on UMG
void UBombShellGameInstance::OnReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString & ListName, const FString & ErrorString)
{
	UE_LOG(LogTemp, Warning, TEXT("Onreadlfriendslistcompleted done"));

	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Onreadlfriendslistcompleted done, SUCCESSFULL"));
		//get the steam online subsystem
		IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get(FName("Steam"));

		//check if the online subsystem is valid
		if (OnlineSub)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, TEXT("Found Steam Online"));
			//get Friends interface
			IOnlineFriendsPtr FriendsInterface = OnlineSub->GetFriendsInterface();

			//if the Friends Interface is valid
			if (FriendsInterface.IsValid())
			{

				TArray< TSharedRef<FOnlineFriend> > FriendList;
				//get a list on all online players and store them in the FriendList
				FriendsInterface->GetFriendsList(LocalUserNum, ListName/*EFriendsLists::ToString((EFriendsLists::Default)),*/, FriendList);

				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, *FString::Printf(TEXT("Number of friends found is: %d"), FriendList.Num()));

				TArray<FSteamFriendInfo> BPFriendsList;
				//for each loop to convert the FOnlineFriend array into the cuteom BP struct
				for (TSharedRef<FOnlineFriend> Friend : FriendList)
				{
					//temp FSteamFriendInfo variable to add to the array
					FSteamFriendInfo TempSteamFriendInfo;
					//get the friend's User ID
					TempSteamFriendInfo.PlayerUniqueNetID.SetUniqueNetId(Friend->GetUserId());
					//get the friend's avatar as texture 2D and store it
					TempSteamFriendInfo.PlayerAvatar = GetSteamAvatar(TempSteamFriendInfo.PlayerUniqueNetID);
					//get the friend's display name
					TempSteamFriendInfo.PlayerName = Friend->GetDisplayName();
					//add the temp variable to the 
					BPFriendsList.Add(TempSteamFriendInfo);
					UE_LOG(LogTemp, Warning, TEXT("Iteration ..."));
				}
				UE_LOG(LogTemp, Warning, TEXT("SUCCESS 2"));

				//call blueprint to show the info on UMG
				OnGetSteamFriendRequestCompleteUMG(BPFriendsList);
			}
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("ShowErrorMessageUMG(FText::FromString(ErrorString));"));
	}
}
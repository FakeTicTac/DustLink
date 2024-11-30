// Copyright Dustbyte Software. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeConst

#include "DustLink/Public/MenuSystem/DustLinkMenu.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystemUtils.h"
#include "Components/Button.h"
#include  "DustLink/Public/Online/DustLinkSubsystem.h"


/**
 * @brief Sets up the menu and its components.
 *
 * This function initializes the menu widget, binds necessary events, and prepares it for display.
 * It also configures the default session settings, such as the number of public connections,
 * the type of match, and the path to the lobby level. This method should be called before adding
 * the widget to the viewport.
 *
 * @param NumberOfPublicConnections The default number of player slots available in the session (default is 4).
 * @param TypeOfMatch A string identifier for the session type (e.g., "Deathmatch", "Coop"). Default is "Error404".
 * @param LobbyPath The path to the lobby level where players will gather before starting the session. Default is "/Game/ThirdPerson/Maps/Lobby".
 */
void UDustLinkMenu::MenuSetup(const int32 NumberOfPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	const UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve world context."), *GetClass()->GetName());
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve Player controller."), *GetClass()->GetName());
		return;
	}

	FInputModeUIOnly InputModeUI;
	InputModeUI.SetWidgetToFocus(TakeWidget());
	InputModeUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	PlayerController->SetInputMode(InputModeUI);
	PlayerController->SetShowMouseCursor(true);

	const UGameInstance* GameInstance = GetGameInstance();
	
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve Game Instance."), *GetClass()->GetName());
		return;
	}

	DustLinkSubsystem = GameInstance->GetSubsystem<UDustLinkSubsystem>();

	NumPublicConnections = NumberOfPublicConnections;
	MatchType = TypeOfMatch;
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);

	if (DustLinkSubsystem)
	{
		DustLinkSubsystem->DustLinkOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		DustLinkSubsystem->DustLinkOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySession);
		DustLinkSubsystem->DustLinkOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSession);
		
		DustLinkSubsystem->DustLinkOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
		DustLinkSubsystem->DustLinkOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
	}
}

/**
 * @brief Tears down the menu and its components.
 *
 * This method removes the menu widget from the viewport and cleans up any associated resources or bindings.
 * It should be called when the menu is no longer needed (e.g., transitioning to gameplay).
 */
void UDustLinkMenu::MenuTearDown()
{
	RemoveFromParent();

	const UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve world context."), *GetClass()->GetName());
		return;
	}
	
	APlayerController* PlayerController = World->GetFirstPlayerController();
	
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve Player controller."), *GetClass()->GetName());
		return;
	}

	const FInputModeGameOnly InputModeGame;
	PlayerController->SetInputMode(InputModeGame);
	PlayerController->SetShowMouseCursor(false);
}

/**
 * @brief Initializes the widget when it is constructed.
 *
 * This method is called automatically when the widget is created.
 * It overrides the default `Initialize` function and is used to set up bindings,
 * configure components, and perform initial setup for the widget.
 *
 * @return Returns `true` if the widget was successfully initialized; otherwise, `false`.
 */
bool UDustLinkMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	
	if (HostButton) HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	if (JoinButton) JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	
	return true;
}

/**
 * @brief Called when the widget is about to be destroyed.
 *
 * This method is automatically invoked when the widget is removed from the viewport
 * and is being deallocated. It is overridden to perform cleanup tasks such as
 * unbinding delegates, releasing resources, or resetting subsystem references.
 */
void UDustLinkMenu::NativeDestruct()
{
	MenuTearDown();

	Super::NativeDestruct();
}

/**
 * @brief Callback for handling the completion of session creation.
 *
 * This method is triggered after attempting to create a session. It processes the result,
 * such as logging success or failure, and performs any additional actions based on the outcome.
 *
 * @param bWasSuccessful Indicates whether the session creation was successful.
 */
void UDustLinkMenu::OnCreateSession(const bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		HostButton->SetIsEnabled(true);
		return;
	}

	// Send player to the multiplayer map
	if (UWorld* World = GetWorld()) World->ServerTravel(PathToLobby);
}

/**
 * @brief Callback for handling the completion of session destruction.
 *
 * This method is triggered after attempting to destroy a session. It processes the result,
 * such as logging success or failure, and performs cleanup tasks if necessary.
 *
 * @param bWasSuccessful Indicates whether the session destruction was successful.
 */
void UDustLinkMenu::OnDestroySession(const bool bWasSuccessful)
{
	if (!bWasSuccessful) return;

	if (UWorld* World = GetWorld()) World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPerson");
}

/**
 * @brief Callback for handling the completion of starting a session.
 *
 * This method is triggered after attempting to start a session. It processes the result,
 * such as logging success or failure, and performs any additional actions based on the outcome.
 *
 * @param bWasSuccessful Indicates whether the session start was successful.
 */
void UDustLinkMenu::OnStartSession(const bool bWasSuccessful)
{
	if (!bWasSuccessful) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Orange, "Starting Game");
	}
}

/**
 * @brief Callback for handling the completion of a session search.
 *
 * This method is triggered after attempting to find sessions. It processes the search results,
 * logs the outcome, and displays or handles the found sessions as needed.
 *
 * @param SessionResults An array of `FOnlineSessionSearchResult` containing the found sessions.
 * @param bWasSuccessful Indicates whether the session search was successful.
 */
void UDustLinkMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, const bool bWasSuccessful)
{
	if (!DustLinkSubsystem) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve DustLink subsystem."), *GetClass()->GetName());
		return;
	}
	
	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);

		if (SettingsValue == MatchType)
		{
			DustLinkSubsystem->JoinSession(Result);
			break;
		}
	}

	if (!bWasSuccessful || SessionResults.Num() == 0)
	{
		JoinButton->SetIsEnabled(true);
	}
}

/**
 * @brief Callback for handling the completion of joining a session.
 *
 * This method is triggered after attempting to join a session. It processes the result,
 * such as logging success or failure, and performs any additional actions based on the outcome.
 *
 * @param Result The result of the join operation, represented as `EOnJoinSessionCompleteResult::Type`.
 */
void UDustLinkMenu::OnJoinSession(const EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve subsystem."), *GetClass()->GetName());
		return;
	}

	const IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();

	if (!SessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve session interface."), *GetClass()->GetName());
		return;
	}

	FString Address;
	SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

	APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve Player controller."), *GetClass()->GetName());
		return;
	}

	if (Result != EOnJoinSessionCompleteResult::Success) JoinButton->SetIsEnabled(true);

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

/**
 * @brief Callback function for the Host button.
 *
 * This function is executed when the `HostButton` is clicked.
 * It handles the logic for creating and starting a new session.
 */
void UDustLinkMenu::HostButtonClicked()
{
	HostButton->SetIsEnabled(false);
	
	if (!DustLinkSubsystem) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve DustLink subsystem."), *GetClass()->GetName());
		return;
	}

	DustLinkSubsystem->CreateSession(NumPublicConnections, MatchType);
}

/**
 * @brief Callback function for the Join button.
 *
 * This function is executed when the `JoinButton` is clicked.
 * It handles the logic for searching and joining an existing session.
 */
void UDustLinkMenu::JoinButtonClicked()
{
	JoinButton->SetIsEnabled(false);
	
	if (!DustLinkSubsystem) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve DustLink subsystem."), *GetClass()->GetName());
		return;
	}

	DustLinkSubsystem->FindSessions(20000);
}
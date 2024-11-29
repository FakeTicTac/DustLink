// Copyright Dustbyte Software. All Rights Reserved.

#include "DustLink/Public/Online/DustLinkSubsystem.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Online/OnlineSessionNames.h"


/**
 * @brief Default constructor for the DustLink subsystem.
 *
 * This constructor is responsible for initializing any required references
 * or variables before the subsystem is fully initialized by the game instance.
 */
UDustLinkSubsystem::UDustLinkSubsystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	InitializeOnlineSessionInterface();
}

/**
 * @brief Initializes the OnlineSessionInterface.
 *
 * This function retrieves the world context and attempts to initialize the OnlineSubsystem session interface.
 * Logs warnings if any steps fail.
 */
void UDustLinkSubsystem::InitializeOnlineSessionInterface()
{
	const UWorld* World = UObject::GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve world context."), *GetClass()->GetName());
		return;
	}

	const IOnlineSubsystem* Subsystem = Online::GetSubsystem(World);

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: OnlineSubsystem not found."), *GetClass()->GetName());
		return;
	}
	
	OnlineSessionInterface = Subsystem->GetSessionInterface();
}

/**
 * @brief Creates a new online session.
 * 
 * This method initializes a new session with the specified number of public connections and match type.
 * 
 * @param NumPublicConnections The number of available slots for players in the session.
 * @param MatchType A string identifier for the type of match (e.g., "Deathmatch", "Coop").
 */
void UDustLinkSubsystem::CreateSession(const int32 NumPublicConnections, const FString& MatchType)
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Session is no longer valid to process creation."), *GetClass()->GetName());
		return;
	}

	// Destroy existing session
	if (OnlineSessionInterface->GetNamedSession(NAME_GameSession)) OnlineSessionInterface->DestroySession(NAME_GameSession);

	// Store the registered Delegate, so that it can be later removed from delegate list
	CreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	CreateSessionSettings(NumPublicConnections, MatchType);

	if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); !OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Couldn't create session."), *GetClass()->GetName());
		OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		DustLinkOnCreateSessionComplete.Broadcast(false);
	}
}

/**
 * @brief Creates and initializes session settings.
 *
 * This method sets up the configuration for an online session, such as the number of public connections,
 * the match type, and other customizable parameters. The settings are stored in the `LastSessionSettings` field.
 *
 * @param NumPublicConnections The maximum number of players allowed in the session, excluding the host.
 * @param MatchType A string identifier for the session type (e.g., "Deathmatch", "Coop").
 */
void UDustLinkSubsystem::CreateSessionSettings(const int32 NumPublicConnections, const FString& MatchType)
{
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = Online::GetSubsystem(GetWorld())->GetSubsystemName() == "NULL";
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->Set(FName("MatchType"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}

/**
 * @brief Searches for available online sessions.
 *
 * This method queries the online subsystem for available sessions matching the specified criteria.
 * 
 * @param MaxSearchResults The maximum number of results to retrieve.
 */
void UDustLinkSubsystem::FindSessions(const int32 MaxSearchResults)
{
	if (!OnlineSessionInterface.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Session is no longer valid to process creation."), *GetClass()->GetName());
		return;
	}

	FindSessionsCompleteDelegateHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	
	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = Online::GetSubsystem(GetWorld())->GetSubsystemName() == "NULL";
	LastSessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

	if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); !OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		DustLinkOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
	}
}

/**
 * @brief Joins an existing session.
 *
 * Attempts to join a session specified by the search result.
 * 
 * @param SessionResult The result of a session search containing session details.
 */
void UDustLinkSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!OnlineSessionInterface.IsValid())
	{
		DustLinkOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		UE_LOG(LogTemp, Warning, TEXT("%s: Session is no longer valid to process creation."), *GetClass()->GetName());
		return;
	}

	JoinSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	if (const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); !OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		DustLinkOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}
}

/**
 * @brief Destroys the currently active session.
 *
 * Ends the active session and cleans up all associated resources.
 */
void UDustLinkSubsystem::DestroySession()
{
	//@TODO: Awaiting implementatio
}

/**
 * @brief Starts the current session.
 *
 * Signals the start of the session, allowing gameplay to commence.
 */
void UDustLinkSubsystem::StartSession()
{
	//@TODO: Awaiting implementation
}

/**
 * @brief Callback for when session creation is complete.
 *
 * This method is triggered by the `FOnCreateSessionCompleteDelegate` when the session creation process finishes.
 * 
 * @param SessionName The name of the session that was created.
 * @param bWasSuccessful Whether the session creation was successful.
 */
void UDustLinkSubsystem::OnCreateSessionComplete(FName SessionName, const bool bWasSuccessful)
{
	if (!OnlineSessionInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Session Interface is not defined."), *GetClass()->GetName());
		return;
	}
	
	OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	DustLinkOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

/**
 * @brief Callback for when session search is complete.
 *
 * This method is triggered by the `FOnFindSessionsCompleteDelegate` when the session search process finishes.
 * 
 * @param bWasSuccessful Whether the session search was successful.
 */
void UDustLinkSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (!OnlineSessionInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Session Interface is not defined."), *GetClass()->GetName());
		return;
	}
	
	if (LastSessionSearch->SearchResults.Num() <= 0) bWasSuccessful = false;

	OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	DustLinkOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

/**
 * @brief Callback for when joining a session is complete.
 *
 * This method is triggered by the `FOnJoinSessionCompleteDelegate` when joining a session finishes.
 * 
 * @param SessionName The name of the session that was joined.
 * @param Result The result of the join operation, indicating success or the type of failure.
 */
void UDustLinkSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Session Interface is not defined."), *GetClass()->GetName());
		return;
	}

	OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	DustLinkOnJoinSessionComplete.Broadcast(Result);
}

/**
 * @brief Callback for when session destruction is complete.
 *
 * This method is triggered by the `FOnDestroySessionCompleteDelegate` when the session destruction process finishes.
 * 
 * @param SessionName The name of the session that was destroyed.
 * @param bWasSuccessful Whether the session was successfully destroyed.
 */
void UDustLinkSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	//@TODO: Awaiting implementation
}

/**
 * @brief Callback for when session start is complete.
 *
 * This method is triggered by the `FOnStartSessionCompleteDelegate` when the session start process finishes.
 * 
 * @param SessionName The name of the session that was started.
 * @param bWasSuccessful Whether the session was successfully started.
 */
void UDustLinkSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//@TODO: Awaiting implementation
}
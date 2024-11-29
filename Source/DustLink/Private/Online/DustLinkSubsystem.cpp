// Copyright Dustbyte Software. All Rights Reserved.

#include "DustLink/Public/Online/DustLinkSubsystem.h"

#include "OnlineSubsystemUtils.h"


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
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to retrieve world context."), *GetClass()->GetName());
		return;
	}

	const IOnlineSubsystem* Subsystem = Online::GetSubsystem(World);

	if (!Subsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: OnlineSubsystem not found."), *GetClass()->GetName());
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
void UDustLinkSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	//@TODO: Awaiting implementation
}

/**
 * @brief Searches for available online sessions.
 *
 * This method queries the online subsystem for available sessions matching the specified criteria.
 * 
 * @param MaxSearchResults The maximum number of results to retrieve.
 * @param MatchType A string identifier for filtering sessions by match type.
 */
void UDustLinkSubsystem::FindSessions(int32 MaxSearchResults, FString MatchType)
{
	//@TODO: Awaiting implementation
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
	//@TODO: Awaiting implementation
}

/**
 * @brief Destroys the currently active session.
 *
 * Ends the active session and cleans up all associated resources.
 */
void UDustLinkSubsystem::DestroySession()
{
	//@TODO: Awaiting implementation
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
void UDustLinkSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//@TODO: Awaiting implementation
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
	//@TODO: Awaiting implementation
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
	//@TODO: Awaiting implementation
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
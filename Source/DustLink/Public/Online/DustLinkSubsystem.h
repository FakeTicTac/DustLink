// Copyright Dustbyte Software. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "DustLinkSubsystem.generated.h"

class IOnlineSession;


/**
 * @class UDustLinkSubsystem
 * @brief Subsystem for managing DustLink functionality within the game instance.
 *
 * This subsystem serves as the central manager for DustLink features, such as
 * online session handling, matchmaking, and communication with OnlineSubsystems.
 * It is initialized when the game instance starts and remains active for the lifetime of the instance.
 *
 * Usage:
 * - Use this subsystem to centralize functionality related to online features.
 * - Access it via the game instance with GetSubsystem<UDustLinkSubsystem>().
 */
UCLASS()
class DUSTLINK_API UDustLinkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor for the DustLink subsystem.
	 *
	 * This constructor is responsible for initializing any required references
	 * or variables before the subsystem is fully initialized by the game instance.
	 */
	UDustLinkSubsystem();
	
	/**
	 * @brief Creates a new online session.
	 * 
	 * This method initializes a new session with the specified number of public connections and match type.
	 * 
	 * @param NumPublicConnections The number of available slots for players in the session.
	 * @param MatchType A string identifier for the type of match (e.g., "Deathmatch", "Coop").
	 */
	void CreateSession(int32 NumPublicConnections, FString MatchType);

	/**
	 * @brief Searches for available online sessions.
	 *
	 * This method queries the online subsystem for available sessions matching the specified criteria.
	 * 
	 * @param MaxSearchResults The maximum number of results to retrieve.
	 * @param MatchType A string identifier for filtering sessions by match type.
	 */
	void FindSessions(int32 MaxSearchResults, FString MatchType);

	/**
	 * @brief Joins an existing session.
	 *
	 * Attempts to join a session specified by the search result.
	 * 
	 * @param SessionResult The result of a session search containing session details.
	 */
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);

	/**
	 * @brief Destroys the currently active session.
	 *
	 * Ends the active session and cleans up all associated resources.
	 */
	void DestroySession();

	/**
	 * @brief Starts the current session.
	 *
	 * Signals the start of the session, allowing gameplay to commence.
	 */
	void StartSession();
	
protected:
	/**
	 * @brief Initializes the OnlineSessionInterface.
	 *
	 * This function retrieves the world context and attempts to initialize the OnlineSubsystem session interface.
	 * Logs warnings if any steps fail.
	 */
	void InitializeOnlineSessionInterface();

	/**
	 * @brief Callback for when session creation is complete.
	 *
	 * This method is triggered by the `FOnCreateSessionCompleteDelegate` when the session creation process finishes.
	 * 
	 * @param SessionName The name of the session that was created.
	 * @param bWasSuccessful Whether the session creation was successful.
	 */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	 * @brief Callback for when session search is complete.
	 *
	 * This method is triggered by the `FOnFindSessionsCompleteDelegate` when the session search process finishes.
	 * 
	 * @param bWasSuccessful Whether the session search was successful.
	 */
	void OnFindSessionComplete(bool bWasSuccessful);

	/**
	 * @brief Callback for when joining a session is complete.
	 *
	 * This method is triggered by the `FOnJoinSessionCompleteDelegate` when joining a session finishes.
	 * 
	 * @param SessionName The name of the session that was joined.
	 * @param Result The result of the join operation, indicating success or the type of failure.
	 */
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	 * @brief Callback for when session destruction is complete.
	 *
	 * This method is triggered by the `FOnDestroySessionCompleteDelegate` when the session destruction process finishes.
	 * 
	 * @param SessionName The name of the session that was destroyed.
	 * @param bWasSuccessful Whether the session was successfully destroyed.
	 */
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	 * @brief Callback for when session start is complete.
	 *
	 * This method is triggered by the `FOnStartSessionCompleteDelegate` when the session start process finishes.
	 * 
	 * @param SessionName The name of the session that was started.
	 * @param bWasSuccessful Whether the session was successfully started.
	 */
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	
private:
	/**
	 * @brief Weak pointer to the OnlineSession interface.
	 *
	 * This pointer provides access to session management functionality from
	 * the OnlineSubsystem.
	 */
	TWeakPtr<IOnlineSession> OnlineSessionInterface;

	/**
	 * @brief Delegate triggered when a session creation process is complete.
	 *
	 * Bound to the OnlineSubsystem's `FOnCreateSessionCompleteDelegate` to notify success or failure.
	 */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;

	/**
	 * @brief Delegate triggered when a session search is complete.
	 *
	 * Bound to the OnlineSubsystem's `FOnFindSessionsCompleteDelegate` to process session search results.
	 */
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;

	/**
	 * @brief Delegate triggered when joining a session is complete.
	 *
	 * Bound to the OnlineSubsystem's `FOnJoinSessionCompleteDelegate` to handle success or failure.
	 */
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;

	/**
	 * @brief Delegate triggered when a session destruction process is complete.
	 *
	 * Bound to the OnlineSubsystem's `FOnDestroySessionCompleteDelegate` to clean up resources.
	 */
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;

	/**
	 * @brief Delegate triggered when starting a session is complete.
	 *
	 * Bound to the OnlineSubsystem's `FOnStartSessionCompleteDelegate` to confirm the session is ready for gameplay.
	 */
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;

	/**
	 * @brief Handle for the CreateSessionComplete delegate.
	 *
	 * Used to store and manage the binding of the `FOnCreateSessionCompleteDelegate` to the OnlineSubsystem.
	 * Allows unbinding when the delegate is no longer needed.
	 */
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	/**
	 * @brief Handle for the FindSessionsComplete delegate.
	 *
	 * Used to store and manage the binding of the `FOnFindSessionsCompleteDelegate` to the OnlineSubsystem.
	 * Allows unbinding when the delegate is no longer needed.
	 */
	FDelegateHandle FindSessionsCompleteDelegateHandle;

	/**
	 * @brief Handle for the JoinSessionComplete delegate.
	 *
	 * Used to store and manage the binding of the `FOnJoinSessionCompleteDelegate` to the OnlineSubsystem.
	 * Allows unbinding when the delegate is no longer needed.
	 */
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	/**
	 * @brief Handle for the DestroySessionComplete delegate.
	 *
	 * Used to store and manage the binding of the `FOnDestroySessionCompleteDelegate` to the OnlineSubsystem.
	 * Allows unbinding when the delegate is no longer needed.
	 */
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	/**
	 * @brief Handle for the StartSessionComplete delegate.
	 *
	 * Used to store and manage the binding of the `FOnStartSessionCompleteDelegate` to the OnlineSubsystem.
	 * Allows unbinding when the delegate is no longer needed.
	 */
	FDelegateHandle StartSessionCompleteDelegateHandle;
};
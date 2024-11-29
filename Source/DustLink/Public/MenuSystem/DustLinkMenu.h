// Copyright Dustbyte Software. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "DustLinkMenu.generated.h"

class UButton;


/**
 * @class UDustLinkMenu
 * @brief User interface menu for DustLink subsystem.
 *
 * This class represents the primary menu widget for the DustLink subsystem, providing functionality
 * for initializing and displaying menu-related features.
 */
UCLASS()
class DUSTLINK_API UDustLinkMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets up the menu and its components.
	 *
	 * This function initializes the menu widget, binds necessary events, and prepares it for display.
	 * It also configures the default session settings, such as the number of public connections
	 * and the type of match. This method should be called before adding the widget to the viewport.
	 *
	 * @param NumberOfPublicConnections The default number of player slots available in the session (default is 4).
	 * @param TypeOfMatch A string identifier for the session type (e.g., "Deathmatch", "Coop"). Default is "Error404".
	 */
	UFUNCTION(BlueprintCallable)
	void MenuSetup(const int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString("Error404"));

	/**
	 * @brief Tears down the menu and its components.
	 *
	 * This method removes the menu widget from the viewport and cleans up any associated resources or bindings.
	 * It should be called when the menu is no longer needed (e.g., transitioning to gameplay).
	 */
	UFUNCTION(BlueprintCallable)
	void MenuTearDown();
	
protected:

	/**
	 * @brief Initializes the widget when it is constructed.
	 *
	 * This method is called automatically when the widget is created.
	 * It overrides the default `Initialize` function and is used to set up bindings,
	 * configure components, and perform initial setup for the widget.
	 *
	 * @return Returns `true` if the widget was successfully initialized; otherwise, `false`.
	 */
	virtual bool Initialize() override;

	/**
	 * @brief Called when the widget is about to be destroyed.
	 *
	 * This method is automatically invoked when the widget is removed from the viewport
	 * and is being deallocated. It is overridden to perform cleanup tasks such as
	 * unbinding delegates, releasing resources, or resetting subsystem references.
	 */
	virtual void NativeDestruct() override;

	/**
	 * @brief Callback for handling the completion of session creation.
	 *
	 * This method is triggered after attempting to create a session. It processes the result,
	 * such as logging success or failure, and performs any additional actions based on the outcome.
	 *
	 * @param bWasSuccessful Indicates whether the session creation was successful.
	 */
	UFUNCTION()
	void OnCreateSession(const bool bWasSuccessful);

	/**
	 * @brief Callback for handling the completion of session destruction.
	 *
	 * This method is triggered after attempting to destroy a session. It processes the result,
	 * such as logging success or failure, and performs cleanup tasks if necessary.
	 *
	 * @param bWasSuccessful Indicates whether the session destruction was successful.
	 */
	UFUNCTION()
	void OnDestroySession(const bool bWasSuccessful);

	/**
	 * @brief Callback for handling the completion of starting a session.
	 *
	 * This method is triggered after attempting to start a session. It processes the result,
	 * such as logging success or failure, and performs any additional actions based on the outcome.
	 *
	 * @param bWasSuccessful Indicates whether the session start was successful.
	 */
	UFUNCTION()
	void OnStartSession(const bool bWasSuccessful);

	/**
	 * @brief Callback for handling the completion of a session search.
	 *
	 * This method is triggered after attempting to find sessions. It processes the search results,
	 * logs the outcome, and displays or handles the found sessions as needed.
	 *
	 * @param SessionResults An array of `FOnlineSessionSearchResult` containing the found sessions.
	 * @param bWasSuccessful Indicates whether the session search was successful.
	 */
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, const bool bWasSuccessful);
	
	/**
	 * @brief Callback for handling the completion of joining a session.
	 *
	 * This method is triggered after attempting to join a session. It processes the result,
	 * such as logging success or failure, and performs any additional actions based on the outcome.
	 *
	 * @param Result The result of the join operation, represented as `EOnJoinSessionCompleteResult::Type`.
	 */
	void OnJoinSession(const EOnJoinSessionCompleteResult::Type Result);
	
private:

	/**
	 * @brief Button widget for hosting a session.
	 *
	 * This button, when clicked, triggers the creation of a new session.
	 * The binding for its click event is set in the `MenuSetup` function.
	 */
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	/**
	 * @brief Button widget for joining a session.
	 *
	 * This button, when clicked, triggers the process of finding and joining an existing session.
	 * The binding for its click event is set in the `MenuSetup` function.
	 */
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	/**
	 * @brief Number of public connections available in the session.
	 *
	 * This property defines the maximum number of players (excluding the host) that can join the session.
	 * The default value is 4, but it can be adjusted when initializing the menu or creating a session.
	 */
	UPROPERTY()
	int32 NumPublicConnections {4};

	/**
	 * @brief Match type identifier for the session.
	 *
	 * This property specifies the type of match (e.g., "Deathmatch", "Coop").
	 * It is used for filtering sessions during search or categorizing session settings.
	 * The default value is "Error404", indicating that the type should be explicitly set.
	 */
	UPROPERTY()
	FString MatchType {TEXT("Error404")};

	/**
	 * @brief Reference to the DustLinkSubsystem.
	 *
	 * This subsystem provides core functionality for managing online sessions, including creating,
	 * finding, joining, and destroying sessions. It is initialized automatically when the game instance
	 * starts and remains active for its lifetime.
	 *
	 * Usage:
	 * - Access this subsystem to interact with DustLink's core features.
	 * - Ensure it is properly initialized before use.
	 */
	UPROPERTY()
	class UDustLinkSubsystem* DustLinkSubsystem;
	
	/**
	 * @brief Callback function for the Host button.
	 *
	 * This function is executed when the `HostButton` is clicked.
	 * It handles the logic for creating and starting a new session.
	 */
	UFUNCTION()
	void HostButtonClicked();

	/**
	 * @brief Callback function for the Join button.
	 *
	 * This function is executed when the `JoinButton` is clicked.
	 * It handles the logic for searching and joining an existing session.
	 */
	UFUNCTION()
	void JoinButtonClicked();
};
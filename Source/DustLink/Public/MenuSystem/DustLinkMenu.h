// Copyright Dustbyte Software. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

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
	 * This function is called to initialize the menu widget, bind necessary events,
	 * and prepare it for display. It should be called before adding the widget to the viewport.
	 *
	 * Usage:
	 * - Call this function in Blueprint or C++ to configure the menu before showing it to the player.
	 */
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

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
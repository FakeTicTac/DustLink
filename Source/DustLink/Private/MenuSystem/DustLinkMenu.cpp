// Copyright Dustbyte Software. All Rights Reserved.

// ReSharper disable CppMemberFunctionMayBeConst

#include "DustLink/Public/MenuSystem/DustLinkMenu.h"

#include "Components/Button.h"
#include  "DustLink/Public/Online/DustLinkSubsystem.h"


/**
 * @brief Sets up the menu and its components.
 *
 * This function is called to initialize the menu widget, bind necessary events,
 * and prepare it for display. It should be called before adding the widget to the viewport.
 *
 * Usage:
 * - Call this function in Blueprint or C++ to configure the menu before showing it to the player.
 */
void UDustLinkMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	const UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to retrieve world context."), *GetClass()->GetName());
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();

	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to retrieve Player controller."), *GetClass()->GetName());
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
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to retrieve Game Instance."), *GetClass()->GetName());
		return;
	}

	DustLinkSubsystem = GameInstance->GetSubsystem<UDustLinkSubsystem>();
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
 * @brief Callback function for the Host button.
 *
 * This function is executed when the `HostButton` is clicked.
 * It handles the logic for creating and starting a new session.
 */
void UDustLinkMenu::HostButtonClicked()
{
	if (!DustLinkSubsystem) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve DustLink subsystem."), *GetClass()->GetName());
		return;
	}
	
	DustLinkSubsystem->CreateSession(4, FString("Error404"));
}

/**
 * @brief Callback function for the Join button.
 *
 * This function is executed when the `JoinButton` is clicked.
 * It handles the logic for searching and joining an existing session.
 */
void UDustLinkMenu::JoinButtonClicked()
{
	if (!DustLinkSubsystem) 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: Failed to retrieve DustLink subsystem."), *GetClass()->GetName());
		return;
	}
}
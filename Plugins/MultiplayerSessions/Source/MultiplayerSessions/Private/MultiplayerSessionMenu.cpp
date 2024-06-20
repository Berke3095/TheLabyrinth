#include "MultiplayerSessionMenu.h"
#include "Components/Button.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UMultiplayerSessionMenu::NativeConstruct()
{
	Super::NativeConstruct();

	if (HostButton) { HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked); }
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::NativeConstruct - HostButton is null.")); }

	if (JoinButton) { JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked); }
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::NativeConstruct - JoinButton is null.")); }
}

void UMultiplayerSessionMenu::NativeDestruct()
{
	Super::NativeDestruct();

	MenuDestruct();
}

void UMultiplayerSessionMenu::OnCreateSession(bool bWasSuccessful1)
{
	if (bWasSuccessful1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.0f,
				FColor::Yellow,
				FString(TEXT("Session created successfully!"))
			);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.0f,
				FColor::Red,
				FString(TEXT("Failed to create session!"))
			);
		}

		if (HostButton) { HostButton->SetIsEnabled(true); }
		else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::OnCreateSession - HostButton is null.")); }
	}

	UWorld* World = GetWorld();
	if (World) 
	{
		FString TravelURL = FString::Printf(TEXT("%s?listen"), *PathToLobby);
		World->ServerTravel(TravelURL);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::OnCreateSession - World is null.")); }
}

void UMultiplayerSessionMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults1, bool bWasSuccesful1)
{
	if (MultiplayerSessionsSubsystem == nullptr) { return; }

	for (auto Result : SessionResults1)
	{
		FString SettingsValue{};
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			MultiplayerSessionsSubsystem->JoinSession(Result);
			return;
		}
	}
	if (!bWasSuccesful1 || SessionResults1.Num() == 0)
	{
		if (JoinButton) { JoinButton->SetIsEnabled(true); }
		else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::OnFindSessions - JoinButton is null.")); }
	}
}

void UMultiplayerSessionMenu::OnJoinSession(EOnJoinSessionCompleteResult::Type Result1)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem) 
	{ 
		IOnlineSessionPtr SessionInterface = SessionInterface = Subsystem->GetSessionInterface(); 
		if (SessionInterface.IsValid())
		{
			FString Address{};
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
			else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::OnJoinSession - PlayerController is null.")); }
		}
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::OnJoinSession - Subsystem is null.")); }

	if (Result1 != EOnJoinSessionCompleteResult::Success)
	{
		if (JoinButton) { JoinButton->SetIsEnabled(true); }
		else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::OnJoinSession - JoinButton is null.")); }
	}
}

void UMultiplayerSessionMenu::HostButtonClicked()
{
	if (HostButton)
	{
		HostButton->SetIsEnabled(false);

		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->CreateSession(NumOfPublicConnections, MatchType);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::HostButtonClicked - MultiplayerSessionsSubsystem is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::HostButtonClicked - HostButton is null.")); }
}

void UMultiplayerSessionMenu::JoinButtonClicked()
{
	if (JoinButton) 
	{ 
		JoinButton->SetIsEnabled(false); 
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->FindSessions(10000);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::JoinButtonClicked - MultiplayerSessionsSubsystem is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::JoinButtonClicked - JoinButton is null.")); }
}

void UMultiplayerSessionMenu::MenuDestruct()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData{};
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::MenuDestruct - PlayerController is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::MenuDestruct - World is null.")); }
}

void UMultiplayerSessionMenu::MenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData{};
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::MenuSetup - PlayerController is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::MenuSetup - World is null.")); }

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
			MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
			MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::MenuSetup - MultiplayerSessionsSubsystem is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionMenu::MenuSetup - GameInstance is null.")); }
}

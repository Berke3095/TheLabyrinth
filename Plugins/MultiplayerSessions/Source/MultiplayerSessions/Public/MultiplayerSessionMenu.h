#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionMenu.generated.h"

class UButton;
class UMultiplayerSessionsSubsystem;

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionMenu : public UUserWidget
{
	GENERATED_BODY()

private:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful1);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults1, bool bWasSuccesful1);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result1);

	UPROPERTY(meta = (BindWidget));
	UButton* HostButton{};
	UPROPERTY(meta = (BindWidget));
	UButton* JoinButton{};

	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();

	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem{};

	void MenuDestruct();

	int32 NumOfPublicConnections{2};
	FString MatchType{TEXT("1v1")};
	FString PathToLobby{ TEXT("/Game/Maps/Test_Lobby_Map") };
	
public:

	UFUNCTION(BlueprintCallable)
	void MenuSetup();
};

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccesful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccesful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UMultiplayerSessionsSubsystem();

	void CreateSession(int32 NumPublicConnections1, FString MatchType1);
	void FindSessions(int32 MaxSearchResults1);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult1);
	void DestroySession();

	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete{};
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete{};
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete{};
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete{};

protected:

	void OnCreateSessionComplete(FName SessionName1, bool bWasSuccessful1);
	void OnFindSessionsComplete(bool bWasSuccessful1);
	void OnJoinSessionComplete(FName SessionName1, EOnJoinSessionCompleteResult::Type Result1);
	void OnDestroySessionComplete(FName SessionName1, bool bWasSuccessful1);

private:

	IOnlineSessionPtr SessionInterface{};
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings{};
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch{};

	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate{};
	FDelegateHandle CreateSessionCompleteDelegateHandle{};
	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate{};
	FDelegateHandle FindSessionsCompleteDelegateHandle{};
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate{};
	FDelegateHandle JoinSessionCompleteDelegateHandle{};
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate{};
	FDelegateHandle DestroySessionCompleteDelegateHandle{};

	bool bCreateSessionOnDestroy{};
	int32 LastNumPublicConnections{};
	FString LastMatchType{};
};

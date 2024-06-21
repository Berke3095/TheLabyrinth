#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

class UMultiplayerSessionMenu;

UCLASS()
class THELABYRINTH_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
	UMultiplayerSessionMenu* MultiplayerSessionMenu{};
};

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

class UTextBlock;

UCLASS()
class THELABYRINTH_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	void SetText(UTextBlock* TextBlock1, FString Text1);
};

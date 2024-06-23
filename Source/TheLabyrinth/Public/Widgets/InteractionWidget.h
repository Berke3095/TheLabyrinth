#pragma once

#include "CoreMinimal.h"
#include "Widgets/MyUserWidget.h"
#include "InteractionWidget.generated.h"

class UTextBlock;

UCLASS()
class THELABYRINTH_API UInteractionWidget : public UMyUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionText{};

public:

	FORCEINLINE UTextBlock* GetInteractionText() const { return InteractionText ? InteractionText : nullptr; }
};

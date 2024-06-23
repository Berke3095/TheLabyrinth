#include "Widgets/InteractionWidget.h"

#include "MyCharacter.h"

void UInteractionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        AMyCharacter* MyCharacter = Cast<AMyCharacter>(PlayerController->GetCharacter());
        if (MyCharacter)
        {
            FString Text{};
            int32 TraceHitActor = MyCharacter->GetTraceHitActorCode();
            switch (TraceHitActor)
            {
            case 0:
                Text = "E - Pickup Rifle";
                break;
            case 1:
                Text = "E - Pickup Shotgun";
                break;
            default:
                break;
            }

            SetText(InteractionText, Text);
        }
        else { UE_LOG(LogTemp, Warning, TEXT("UInteractionWidget::NativeConstruct - MyCharacter is null.")); }
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UInteractionWidget::NativeConstruct - PlayerController is null.")); }
}

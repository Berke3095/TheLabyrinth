#include "Widgets/MyUserWidget.h"

#include "Components/TextBlock.h" 

void UMyUserWidget::SetText(UTextBlock* TextBlock1, FString Text1)
{
    if (TextBlock1)
    {
        TextBlock1->SetText(FText::FromString(Text1));
    }
    else { UE_LOG(LogTemp, Warning, TEXT("UMyUserWidget::SetText - %s is null."), *TextBlock1->GetName()); }
}

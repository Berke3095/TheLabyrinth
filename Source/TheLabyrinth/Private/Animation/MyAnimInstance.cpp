#include "Animation/MyAnimInstance.h"

#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h" 
#include "KismetAnimationLibrary.h" 

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if (MyCharacter)
	{
		CharacterMovement = MyCharacter->GetCharacterMovement();
		if (!CharacterMovement) { UE_LOG(LogTemp, Warning, TEXT("UMyAnimInstance::NativeInitializeAnimation - CharacterMovement is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMyAnimInstance::NativeInitializeAnimation - MyCharacter is null.")); }
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterMovement)
	{
		CharacterDirection = UKismetAnimationLibrary::CalculateDirection(CharacterMovement->Velocity, MyCharacter->GetActorRotation());

		Speed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);

		CharacterState = MyCharacter->GetCharacterState();

		FRotator AimRotation = MyCharacter->GetBaseAimRotation();
		CharacterPitchRotation = AimRotation.Pitch;

		if (CharacterPitchRotation > 90.0f && !MyCharacter->IsLocallyControlled())
		{
			FVector2D ToConvert(270.0f, 360.0f);
			FVector2D ConvertTo(-90.0f, 0.0f);
			CharacterPitchRotation = FMath::GetMappedRangeValueClamped(ToConvert, ConvertTo, CharacterPitchRotation);
		}

		UE_LOG(LogTemp, Warning, TEXT("CharacterPitchRotation is %f"), CharacterPitchRotation);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UMyAnimInstance::NativeUpdateAnimation - CharacterMovement is null.")); }
}

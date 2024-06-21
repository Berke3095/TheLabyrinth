#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

class AMyCharacter;
class UCharacterMovementComponent;

UCLASS()
class THELABYRINTH_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:

	UPROPERTY(BlueprintReadOnly)
	AMyCharacter* MyCharacter{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	UCharacterMovementComponent* CharacterMovement{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed{};

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float CharacterDirection{};
};

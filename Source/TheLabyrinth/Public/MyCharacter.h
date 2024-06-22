#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class APlayerController;

class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class THELABYRINTH_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMyCharacter();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void EyeTrace();

	/*
		REFERENCES
	*/
	APlayerController* PlayerController{};

	void GetReferences();

	/*
		COMPONENTS
	*/
	UCapsuleComponent* CapsuleComponent{};

	USkeletalMeshComponent* FPSMeshComponent{};
	UPROPERTY(EditDefaultsOnly);
	USkeletalMeshComponent* ReplicatedMeshComponent{};
	void SetMeshes();

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera{};
	void SetDefaults();

	UCharacterMovementComponent* CharMovement{};
	void SetCharacterMovement();

	/*
		INPUT
	*/
	void SetupEnhancedInput();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext{};

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction{};
	void Move(const FInputActionValue& InputValue1);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction{};
	void Look(const FInputActionValue& InputValue1);
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UInteractionWidget;

class UCameraComponent;
class UCombatComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "UnEquipped State"),
	ECS_Equipped UMETA(DisplayName = "Equipped State"),
	ECS_NONE UMETA(DisplayName = "NONE")
};

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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	/*
		FUNCTIONALITY
	*/
	void EyeTrace();
	AActor* InteractableActor{};
	void HandleInteractionWidget();

	UFUNCTION(Server, Reliable)
	void ServerEquip(AActor* WeaponToEquip1);

	UFUNCTION(Server, Reliable)
	void ServerDrop(AActor* SwapWeapon1);

	UPROPERTY(ReplicatedUsing = OnRep_CharacterState)
	ECharacterState CharacterState{ ECharacterState::ECS_UnEquipped };
	UFUNCTION()
	void OnRep_CharacterState();

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

	UPROPERTY(EditDefaultsOnly)
	UCombatComponent* CombatComponent{};

	/*
		INPUT
	*/
	void SetupEnhancedInput();

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputMappingContext* CharacterMappingContext{};

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* MoveAction{};
	void Move(const FInputActionValue& InputValue1);

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* LookAction{};
	void Look(const FInputActionValue& InputValue1);

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* InteractAction{};
	void Interact();

	/*
		WIDGETS
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass{};
	UInteractionWidget* InteractionWidget{};

public:

	FORCEINLINE USkeletalMeshComponent* GetReplicatedMesh() const { return ReplicatedMeshComponent ? ReplicatedMeshComponent : nullptr; }
	void SetCharacterState(ECharacterState CharacterState1) { CharacterState = CharacterState1; }
};

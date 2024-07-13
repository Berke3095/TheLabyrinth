#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterStates.h"
#include "MyCharacter.generated.h"

class UInteractionWidget;

class UCameraComponent;
class UCombatComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UMyAnimInstance;
class UAnimMontage;

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
	void Server_EquipWeapon(AMyWeapon* WeaponToEquip1);

	UFUNCTION(Server, Reliable)
	void Server_DropWeapon(AMyWeapon* SwapWeapon1);

	UPROPERTY(ReplicatedUsing = OnRep_CharacterState)
	ECharacterState CharacterState{ ECharacterState::ECS_UnEquipped };
	UFUNCTION()
	void OnRep_CharacterState();

	/*
		REFERENCES
	*/
	APlayerController* PlayerController{};
	UMyAnimInstance* ReplicatedAnimInstance{};

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

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* FireAction{};
	void FireWeapon();
	UFUNCTION(Server, Reliable)
	void Server_FireWeapon();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_FireWeapon();

	/*
		MONTAGES
	*/
	void PlayFireMontage(UAnimInstance* AnimInstance1, UAnimMontage* MontageToPlay1);

	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* ReplicatedFireAnimMontage{};
	

	/*
		WIDGETS
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass{};
	UInteractionWidget* InteractionWidget{};

public:

	FORCEINLINE USkeletalMeshComponent* GetReplicatedMesh() const { return ReplicatedMeshComponent ? ReplicatedMeshComponent : nullptr; }
	FORCEINLINE USkeletalMeshComponent* GetFPSMesh() const { return FPSMeshComponent ? FPSMeshComponent : nullptr; }

	void SetCharacterState(ECharacterState CharacterState1);
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	FORCEINLINE APlayerController* GetPlayerController() const { return PlayerController ? PlayerController : nullptr; }
};

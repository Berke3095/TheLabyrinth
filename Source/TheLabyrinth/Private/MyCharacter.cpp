#include "MyCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Camera/CameraComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterComponents/CombatComponent.h"

#include "Animation/MyAnimInstance.h" 
#include "Animation/AnimMontage.h"

#include "Widgets/InteractionWidget.h"

#include "Net/UnrealNetwork.h"

#include "Weapons/MyWeapon.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetDefaults();
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetReferences();
	SetupEnhancedInput();
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EyeTrace();
}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction) { EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupPlayerInputComponent - MoveAction is null.")); }

		if(LookAction) { EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupPlayerInputComponent - LookAction is null.")); }

		if (InteractAction) { EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMyCharacter::Interact); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupPlayerInputComponent - InteractAction is null.")); }

		if (FireAction) 
		{ EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AMyCharacter::FireWeapon); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupPlayerInputComponent - FireAction is null.")); }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupPlayerInputComponent - EnhancedInputComponent is null.")); }
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, CharacterState);
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (CombatComponent)
	{
		CombatComponent->MyCharacter = this;
	}
}

void AMyCharacter::EyeTrace()
{
	if (IsLocallyControlled() && PlayerController)
	{
		int32 ViewportSizeX{}, ViewportSizeY{};
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
		FVector2D ScreenLocation(ViewportSizeX / 2.0f, ViewportSizeY / 2.0f);

		FVector WorldLocation{}, WorldDirection{};
		bool bScreenToWorld = PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection);
		if (bScreenToWorld)
		{
			FVector End = WorldLocation + (WorldDirection * 140.0f);
			FHitResult HitResult{};

			FCollisionQueryParams CollisionParams{};
			CollisionParams.AddIgnoredActor(this);

			// DrawDebugLine(GetWorld(), WorldLocation, End, FColor::Red, false, -1.0f, 0.0f, 0.1f);

			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, End, ECC_GameTraceChannel1, CollisionParams);

			if (bHit)
			{
				DrawDebugSphere(
					GetWorld(),
					HitResult.ImpactPoint,
					12.0f,
					12,
					FColor::Red
				);

				AActor* HitActor = HitResult.GetActor();
				if (HitActor && HitActor->ActorHasTag("Interactable")) { InteractableActor = HitActor; HandleInteractionWidget(); }
			}
			else { if (InteractableActor) { InteractableActor = nullptr; } if (InteractionWidget) { InteractionWidget->RemoveFromViewport(); InteractionWidget = nullptr; } }
		}
	}
	else if(!PlayerController) { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::EyeTrace - PlayerController is null.")); }
}

void AMyCharacter::HandleInteractionWidget()
{
	if (!InteractionWidget && InteractableActor)
	{
		if (InteractableActor->ActorHasTag("Weapon"))
		{
			InteractionWidget = CreateWidget<UInteractionWidget>(PlayerController, InteractionWidgetClass);
			if (InteractionWidget)
			{
				switch (CharacterState)
				{
				case ECharacterState::ECS_UnEquipped:
					if (InteractableActor->ActorHasTag("Rifle")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Pickup Rifle"); }
					else if (InteractableActor->ActorHasTag("Shotgun")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Pickup Shotgun"); }
					InteractionWidget->AddToViewport();
					break;
				default:
					if (InteractableActor->ActorHasTag("Rifle")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Swap with Rifle"); }
					else if (InteractableActor->ActorHasTag("Shotgun")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Swap with Shotgun"); }
					InteractionWidget->AddToViewport();
					break;
				}
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::HandleInteractionWidget - InteractionWidget is null.")); }
		}
	}
	else
	{
		switch (CharacterState)
		{
		case ECharacterState::ECS_UnEquipped:
			if (InteractableActor->ActorHasTag("Rifle")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Pickup Rifle"); }
			else if (InteractableActor->ActorHasTag("Shotgun")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Pickup Shotgun"); }
			break;
		default:
			if (InteractableActor->ActorHasTag("Rifle")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Swap with Rifle"); }
			else if (InteractableActor->ActorHasTag("Shotgun")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Swap with Shotgun"); }
			break;
		}
	}
}

void AMyCharacter::Server_DropWeapon_Implementation(AMyWeapon* SwapWeapon1)
{
	CombatComponent->DropWeapon(SwapWeapon1);
}

void AMyCharacter::OnRep_CharacterState()
{
	switch (CharacterState)
	{
	case ECharacterState::ECS_UnEquipped:
		FPSMeshComponent->SetOwnerNoSee(true);
		break;
	default:
		FPSMeshComponent->SetOwnerNoSee(false);
		break;
	}
}

void AMyCharacter::GetReferences()
{
	UWorld* World = GetWorld();
	if (World)
	{
		PlayerController = World->GetFirstPlayerController();
	}

	ReplicatedAnimInstance = Cast<UMyAnimInstance>(ReplicatedMeshComponent->GetAnimInstance());
	if(!ReplicatedAnimInstance) { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::GetReferences - ReplicatedAnimInstance is null.")); }

	FPSAnimInstance = Cast<UMyAnimInstance>(FPSMeshComponent->GetAnimInstance());
	if (!FPSAnimInstance) { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::GetReferences - FPSAnimInstance is null.")); }
}

void AMyCharacter::SetMeshes()
{
	FPSMeshComponent = GetMesh();
	if (FPSMeshComponent)
	{
		FPSMeshComponent->SetOnlyOwnerSee(true);
		FPSMeshComponent->SetOwnerNoSee(true);
		FPSMeshComponent->SetupAttachment(Camera);
		FPSMeshComponent->CastShadow = false;
		FPSMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FPSMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetMeshes - FPSMeshComponent is null.")); }

	ReplicatedMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ReplicatedMeshComponent"));
	if (ReplicatedMeshComponent)
	{
		ReplicatedMeshComponent->SetOwnerNoSee(true);
		ReplicatedMeshComponent->SetupAttachment(RootComponent);
		ReplicatedMeshComponent->CastShadow = true;
		ReplicatedMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ReplicatedMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetMeshes - ReplicatedMeshComponent is null.")); }
}

void AMyCharacter::SetDefaults()
{
	bReplicates = true;

	SetCharacterMovement();

	CapsuleComponent = GetCapsuleComponent();
	if (CapsuleComponent)
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	if (Camera) { Camera->SetupAttachment(RootComponent); Camera->FieldOfView = 90.0f; SetMeshes(); }
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetDefaults - Camera is null.")); }

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	if (CombatComponent)
	{
		CombatComponent->SetIsReplicated(true);
	}

	bUseControllerRotationYaw = true;
}

void AMyCharacter::SetCharacterMovement()
{
	CharMovement = GetCharacterMovement();
	if(CharMovement)
	{
		CharMovement->MaxWalkSpeed = 300.0f;
	}
}

void AMyCharacter::SetupEnhancedInput()
{
	if (IsLocallyControlled() && PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupEnhancedInput - Subsystem is null.")); }
	}
	else if(!PlayerController) { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupEnhancedInput - PlayerController is null.")); }
}

void AMyCharacter::Move(const FInputActionValue& InputValue1)
{
	const FVector2D Value = InputValue1.Get<FVector2D>();

	if (PlayerController)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator ControlYawRotation(0, ControlRotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::Move - PlayerController is null.")); }
}

void AMyCharacter::Look(const FInputActionValue& InputValue1)
{
	const FVector2D Value = InputValue1.Get<FVector2D>();

	if (PlayerController)
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::Look - PlayerController is null.")); }
}

void AMyCharacter::Interact()
{
	if (CombatComponent && InteractableActor && InteractableActor->ActorHasTag("Weapon"))
	{
		AMyWeapon* Weapon = Cast<AMyWeapon>(InteractableActor);
		if (Weapon)
		{
			if (CharacterState != ECharacterState::ECS_UnEquipped)
			{
				if (HasAuthority())
				{
					CombatComponent->DropWeapon(Weapon);
				}
				else { Server_DropWeapon(Weapon); }
			}

			if (HasAuthority())
			{
				CombatComponent->EquipWeapon(Weapon);
			}
			else { Server_EquipWeapon(Weapon); }
		}
	}
	else if(!CombatComponent) { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::Interact - CombatComponent is null.")); }
}

void AMyCharacter::FireWeapon()
{
	if (CharacterState != ECharacterState::ECS_UnEquipped)
	{
		Server_FireWeapon();
		if(IsLocallyControlled()) { PlayFireMontage(FPSAnimInstance, FPSFireAnimMontage); }
	}
}

void AMyCharacter::Multicast_FireWeapon_Implementation()
{
	PlayFireMontage(ReplicatedAnimInstance, ReplicatedFireAnimMontage);
}

void AMyCharacter::Server_FireWeapon_Implementation()
{
	Multicast_FireWeapon();
}

void AMyCharacter::PlayFireMontage(UAnimInstance* AnimInstance1, UAnimMontage* MontageToPlay1)
{
	if (AnimInstance1 && MontageToPlay1 && !AnimInstance1->Montage_IsPlaying(MontageToPlay1))
	{
		FName SectionName{};
		switch (CharacterState)
		{
		case ECharacterState::ECS_EquippedRifle:
			SectionName = FName("Fire_Rifle");
			break;
		case ECharacterState::ECS_EquippedShotgun:
			SectionName = FName("Fire_Shotgun");
			break;
		default:
			break;
		}

		AnimInstance1->Montage_Play(MontageToPlay1);
		AnimInstance1->Montage_JumpToSection(SectionName, MontageToPlay1);

		if (CombatComponent && CombatComponent->EquippedWeapon)
		{
			AMyWeapon* CurrentWeapon = CombatComponent->EquippedWeapon;
			if (CurrentWeapon)
			{
				if (IsLocallyControlled())
				{
					CurrentWeapon->Fire(CurrentWeapon->GetWeaponFPSMesh());
				}
				else
				{
					CurrentWeapon->Fire(CurrentWeapon->GetWeaponReplicatedMesh());
				}
			}
		}
	}
	else if (!MontageToPlay1) { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::PlayFireMontage - MontageToPlay1 is null.")); }
}

void AMyCharacter::SetCharacterState(ECharacterState CharacterState1)
{
	CharacterState = CharacterState1;
	switch (CharacterState)
	{
	case ECharacterState::ECS_UnEquipped:
		FPSMeshComponent->SetOwnerNoSee(true);
		break;
	default:
		FPSMeshComponent->SetOwnerNoSee(false);
		break;
	}
}

void AMyCharacter::Server_EquipWeapon_Implementation(AMyWeapon* WeaponToEquip1)
{
	CombatComponent->EquipWeapon(WeaponToEquip1);
}
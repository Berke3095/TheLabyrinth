#include "MyCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Camera/CameraComponent.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Widgets/InteractionWidget.h"

#include "Net/UnrealNetwork.h"

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
		else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupPlayerInputComponent - MoveAction is null.")) }

		if(LookAction) { EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look); }
		else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupPlayerInputComponent - LookAction is null.")) }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupPlayerInputComponent - EnhancedInputComponent is null.")) }
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AMyCharacter::EyeTrace()
{
	if (IsLocallyControlled() && PlayerController)
	{
		int32 ViewportSizeX{}, ViewportSizeY{};
		PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);
		FVector2D ScreenLocation(ViewportSizeX / 2.0f, ViewportSizeY / 2.0f);

		FVector WorldLocation{}, WorldDirection{};
		if (PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection))
		{
			FVector End = WorldLocation + (WorldDirection * 100.0f);
			FHitResult HitResult{};

			FCollisionQueryParams CollisionParams{};
			CollisionParams.AddIgnoredActor(this);

			DrawDebugLine(GetWorld(), WorldLocation, End, FColor::Red, false, -1.0f, 0.0f, 0.1f);

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
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::EyeTrace - PlayerController is null.")) }
}

void AMyCharacter::HandleInteractionWidget()
{
	if (!InteractionWidget)
	{
		if (InteractableActor && InteractableActor->ActorHasTag("Weapon"))
		{
			InteractionWidget = CreateWidget<UInteractionWidget>(PlayerController, InteractionWidgetClass);
			if (InteractionWidget) 
			{ 
				if (InteractableActor->ActorHasTag("Rifle")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Pickup Rifle"); }
				else if (InteractableActor->ActorHasTag("Shotgun")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Pickup Shotgun"); }
				InteractionWidget->AddToViewport(); 
			}
			else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::HandleInteractionWidget - InteractionWidget is null.")) }
		}
	}
	else
	{
		if (InteractableActor->ActorHasTag("Rifle")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Pickup Rifle"); }
		else if (InteractableActor->ActorHasTag("Shotgun")) { InteractionWidget->SetText(InteractionWidget->GetInteractionText(), "E - Pickup Shotgun"); }
	}
}

void AMyCharacter::GetReferences()
{
	PlayerController = Cast<APlayerController>(GetController());
}

void AMyCharacter::SetMeshes()
{
	FPSMeshComponent = GetMesh();
	if (FPSMeshComponent)
	{
		FPSMeshComponent->SetOnlyOwnerSee(true);
		FPSMeshComponent->SetOwnerNoSee(true);
		FPSMeshComponent->SetupAttachment(RootComponent);
		FPSMeshComponent->CastShadow = false;
		FPSMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		FPSMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetMeshes - FPSMeshComponent is null.")) }

	ReplicatedMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ReplicatedMeshComponent"));
	if (ReplicatedMeshComponent)
	{
		ReplicatedMeshComponent->SetOwnerNoSee(true);
		ReplicatedMeshComponent->SetupAttachment(RootComponent);
		ReplicatedMeshComponent->CastShadow = true;
		ReplicatedMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ReplicatedMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetMeshes - ReplicatedMeshComponent is null.")) }
}

void AMyCharacter::SetDefaults()
{
	bReplicates = true;

	SetCharacterMovement();
	SetMeshes();

	CapsuleComponent = GetCapsuleComponent();
	if (CapsuleComponent)
	{
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	if (Camera) { Camera->SetupAttachment(RootComponent); Camera->FieldOfView = 90.0f; }
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetDefaults - Camera is null.")) }

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
		else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupEnhancedInput - Subsystem is null.")) }
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetupEnhancedInput - PlayerController is null.")) }
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
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::Move - PlayerController is null.")) }
}

void AMyCharacter::Look(const FInputActionValue& InputValue1)
{
	const FVector2D Value = InputValue1.Get<FVector2D>();

	if (PlayerController)
	{
		AddControllerYawInput(Value.X);
		AddControllerPitchInput(Value.Y);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::Look - PlayerController is null.")) }
}
#include "MyCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h" 

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

	UseControllerYaw(DeltaTime);
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
		FPSMeshComponent->SetupAttachment(RootComponent);
		FPSMeshComponent->CastShadow = false;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetMeshes - FPSMeshComponent is null.")) }

	ReplicatedMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ReplicatedMeshComponent"));
	if (ReplicatedMeshComponent)
	{
		ReplicatedMeshComponent->SetOwnerNoSee(true);
		ReplicatedMeshComponent->SetupAttachment(RootComponent);
		ReplicatedMeshComponent->CastShadow = true;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetMeshes - ReplicatedMeshComponent is null.")) }

	FullBodyMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FullBodyMeshComponent"));
	if (FullBodyMeshComponent)
	{
		FullBodyMeshComponent->SetOnlyOwnerSee(true);
		FullBodyMeshComponent->SetupAttachment(RootComponent);
		FullBodyMeshComponent->CastShadow = true;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetMeshes - FullBodyMeshComponent is null.")) }

	LegsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsMeshComponent"));
	if (LegsMeshComponent)
	{
		LegsMeshComponent->SetOnlyOwnerSee(true);
		LegsMeshComponent->SetupAttachment(RootComponent);
		LegsMeshComponent->CastShadow = false;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetMeshes - LegsMeshComponent is null.")) }
}

void AMyCharacter::SetDefaults()
{
	CapsuleComponent = GetCapsuleComponent();

	SetMeshes();

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	if (Camera) { Camera->SetupAttachment(RootComponent); }
	else { UE_LOG(LogTemp, Warning, TEXT("AMyCharacter::SetDefaults - Camera is null.")) }

	bUseControllerRotationYaw = false;
}

void AMyCharacter::SetupEnhancedInput()
{
	if (PlayerController)
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

void AMyCharacter::UseControllerYaw(float Deltatime1)
{
	FRotator TargetActorRotation(0.0f, GetControlRotation().Yaw, 0.0f);
	FRotator InterpolatedRotation = FMath::RInterpTo(GetActorRotation(), TargetActorRotation, Deltatime1, 20.0f);
	SetActorRotation(InterpolatedRotation);
}


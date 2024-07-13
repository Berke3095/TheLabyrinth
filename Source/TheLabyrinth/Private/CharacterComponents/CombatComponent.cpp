#include "CharacterComponents/CombatComponent.h"

#include "Weapons/MyWeapon.h"
#include "MyCharacter.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetMathLibrary.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	BulletTrace();
}

void UCombatComponent::EquipWeapon(AMyWeapon* WeaponToEquip1)
{
	if (MyCharacter && WeaponToEquip1)
	{
		EquippedWeapon = WeaponToEquip1;
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_IsEquipped);
		EquippedWeapon->SetOwner(MyCharacter);

		if (EquippedWeapon->ActorHasTag("Rifle")) { MyCharacter->SetCharacterState(ECharacterState::ECS_EquippedRifle); }
		else if(EquippedWeapon->ActorHasTag("Shotgun")) { MyCharacter->SetCharacterState(ECharacterState::ECS_EquippedShotgun); }

		if (const USkeletalMeshSocket* HandSocket = MyCharacter->GetReplicatedMesh()->GetSocketByName(FName("Replicated_Weapon_Socket")))
		{
			HandSocket->AttachActor(EquippedWeapon, MyCharacter->GetReplicatedMesh());
		}
		else { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - HandSocket is null.")); }
		Client_AttachWeapon(EquippedWeapon->GetWeaponFPSMesh(), MyCharacter->GetFPSMesh());
	}
	else if(!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - MyCharacter is null.")); }
	else if(!WeaponToEquip1) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - WeaponToEquip1 is null.")) }
}

void UCombatComponent::DropWeapon(AMyWeapon* SwapWeapon1)
{
	if (MyCharacter && EquippedWeapon)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_IsDropped);
		EquippedWeapon->SetOwner(nullptr);
		MyCharacter->SetCharacterState(ECharacterState::ECS_UnEquipped);

		Client_DeAttachWeapon(EquippedWeapon->GetWeaponFPSMesh());
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		Server_PlaceWeapon(EquippedWeapon, SwapWeapon1);

		EquippedWeapon = nullptr;
	}
	else if (!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::DropWeapon - MyCharacter is null.")); }
	else if (!EquippedWeapon) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::DropWeapon - EquippedWeapon is null.")) }
}

void UCombatComponent::Server_PlaceWeapon_Implementation(AMyWeapon* CurrentWeapon1, AMyWeapon* SwapWeapon1)
{
	Multicast_PlaceWeapon(CurrentWeapon1, SwapWeapon1);
}

void UCombatComponent::BulletTrace()
{
	if (MyCharacter->IsLocallyControlled() && MyCharacter->GetPlayerController())
	{
		int32 ViewportSizeX{}, ViewportSizeY{};
		MyCharacter->GetPlayerController()->GetViewportSize(ViewportSizeX, ViewportSizeY);
		FVector2D ScreenLocation(ViewportSizeX / 2.0f, ViewportSizeY / 2.0f);

		float BulletSpread{};
		float RandomPitch{};
		float RandomYaw{};

		UCharacterMovementComponent* MyCharacterMovement = MyCharacter->GetCharacterMovement();
		float GroundSpeed = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);

		if (GroundSpeed > 0) { BulletSpread = 1.5f; }
		else { BulletSpread = 0.5f; }
		
		RandomPitch = FMath::RandRange(-BulletSpread, BulletSpread);
		RandomYaw = FMath::RandRange(-BulletSpread, BulletSpread);

		FVector WorldLocation{}, WorldDirection{};
		bool bScreenToWorld = MyCharacter->GetPlayerController()->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection);
		if (bScreenToWorld)
		{
			WorldDirection = FQuat(WorldDirection.Rotation() + FRotator(RandomPitch, RandomYaw, 0.0f)).Vector();

			FVector End = WorldLocation + (WorldDirection * BulletTraceLength);
			FHitResult HitResult{};

			FCollisionQueryParams CollisionParams{};
			CollisionParams.AddIgnoredActor(GetOwner());

			bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, End, ECC_GameTraceChannel2, CollisionParams); // Bullet trace 

			if (bHit)
			{
				DrawDebugSphere(
					GetWorld(),
					HitResult.ImpactPoint,
					12.0f,
					12,
					FColor::Red
				);
			}
		}
	}
	else if (!MyCharacter->GetPlayerController()) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::BulletTrace - PlayerController is null.")); }
}

void UCombatComponent::Client_AttachWeapon_Implementation(USkeletalMeshComponent* WeaponFPSMesh1, USkeletalMeshComponent* CharacterFPSMesh1)
{
	if (WeaponFPSMesh1 && CharacterFPSMesh1)
	{
		WeaponFPSMesh1->AttachToComponent(CharacterFPSMesh1, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("FPS_Weapon_Socket"));
	}
	else if(!WeaponFPSMesh1) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::Client_AttachWeapon_Implementation - WeaponFPSMesh1 is null.")); }
	else if (!CharacterFPSMesh1) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::Client_AttachWeapon_Implementation - CharacterFPSMesh1 is null.")); }
}

void UCombatComponent::Client_DeAttachWeapon_Implementation(USkeletalMeshComponent* WeaponFPSMesh1)
{
	if(WeaponFPSMesh1) { WeaponFPSMesh1->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform); }
}

void UCombatComponent::Multicast_PlaceWeapon_Implementation(AMyWeapon* CurrentWeapon1, AMyWeapon* SwapWeapon1)
{
	if (CurrentWeapon1)
	{
		FTransform EquipWeaponTransform{};
		EquipWeaponTransform = SwapWeapon1->GetActorTransform();
		CurrentWeapon1->SetActorTransform(EquipWeaponTransform);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::Multicast_PlaceWeapon_Implementation - CurrentWeapon1 is null.")) }
}

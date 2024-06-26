#include "CharacterComponents/CombatComponent.h"

#include "Weapons/MyWeapon.h"
#include "MyCharacter.h"

#include "Engine/SkeletalMeshSocket.h"

#include "Net/UnrealNetwork.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UCombatComponent::EquipWeapon(AMyWeapon* WeaponToEquip1)
{
	if (MyCharacter && WeaponToEquip1)
	{
		EquippedWeapon = WeaponToEquip1;
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_IsEquipped);
		EquippedWeapon->SetOwner(MyCharacter);
		MyCharacter->SetCharacterState(ECharacterState::ECS_Equipped);

		const USkeletalMeshSocket* HandSocket = MyCharacter->GetReplicatedMesh()->GetSocketByName(FName("Weapon_Socket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(EquippedWeapon, MyCharacter->GetReplicatedMesh());
		}
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

		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		Multicast_PlaceWeapon(EquippedWeapon, SwapWeapon1);

		EquippedWeapon = nullptr;
	}
	else if (!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::DropWeapon - MyCharacter is null.")); }
	else if (!EquippedWeapon) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::DropWeapon - EquippedWeapon is null.")) }
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

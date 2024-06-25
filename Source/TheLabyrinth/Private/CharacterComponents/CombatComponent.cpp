#include "CharacterComponents/CombatComponent.h"

#include "Weapons/MyWeapon.h"
#include "MyCharacter.h"

#include "Engine/SkeletalMeshSocket.h"

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

void UCombatComponent::EquipWeapon(AMyWeapon* WeaponToEquip1)
{
	if (MyCharacter && WeaponToEquip1)
	{
		EquippedWeapon = WeaponToEquip1;
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_IsEquipped);
		EquippedWeapon->SetEquippedWeaponSettings();
		MyCharacter->SetCharacterState(ECharacterState::ECS_Equipped);
		EquippedWeapon->SetOwner(MyCharacter);
		const USkeletalMeshSocket* HandSocket = MyCharacter->GetReplicatedMesh()->GetSocketByName(FName("Weapon_Socket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(EquippedWeapon, MyCharacter->GetReplicatedMesh());
		}
	}
	else if(!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - MyCharacter is null.")); }
	else if(!WeaponToEquip1) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - WeaponToEquip1 is null.")) }
}

void UCombatComponent::DropWeapon()
{
	if (MyCharacter && EquippedWeapon)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_IsDropped);
		EquippedWeapon->SetDroppedWeaponSettings();
		EquippedWeapon->SetOwner(nullptr);
		MyCharacter->SetCharacterState(ECharacterState::ECS_UnEquipped);
		const USkeletalMeshSocket* HandSocket = MyCharacter->GetReplicatedMesh()->GetSocketByName(FName("Weapon_Socket"));
		if (HandSocket)
		{
			EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}
		EquippedWeapon = nullptr;
	}
	else if (!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::DropWeapon - MyCharacter is null.")); }
	else if (!EquippedWeapon) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::DropWeapon - EquippedWeapon is null.")) }
}


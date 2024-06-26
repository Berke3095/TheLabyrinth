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

void UCombatComponent::EquipWeapon(AActor* WeaponToEquip1)
{
	if (MyCharacter && WeaponToEquip1)
	{
		AMyWeapon* Weapon = Cast<AMyWeapon>(WeaponToEquip1);
		if (Weapon)
		{
			EquippedWeapon = Weapon;
			EquippedWeapon->SetWeaponState(EWeaponState::EWS_IsEquipped);
			EquippedWeapon->SetOwner(MyCharacter);
			MyCharacter->SetCharacterState(ECharacterState::ECS_Equipped);
			
			const USkeletalMeshSocket* HandSocket = MyCharacter->GetReplicatedMesh()->GetSocketByName(FName("Weapon_Socket"));
			if (HandSocket)
			{
				HandSocket->AttachActor(EquippedWeapon, MyCharacter->GetReplicatedMesh());
			}
		}
	}
	else if(!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - MyCharacter is null.")); }
	else if(!WeaponToEquip1) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - WeaponToEquip1 is null.")) }
}

void UCombatComponent::DropWeapon(AActor* SwapWeapon1)
{
	if (MyCharacter && EquippedWeapon)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_IsDropped);
		EquippedWeapon->SetOwner(nullptr);
		MyCharacter->SetCharacterState(ECharacterState::ECS_UnEquipped);

		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		EquipWeaponTransform = SwapWeapon1->GetActorTransform();
		EquippedWeapon->SetActorTransform(EquipWeaponTransform);
		EquippedWeapon = nullptr;
	}
	else if (!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::DropWeapon - MyCharacter is null.")); }
	else if (!EquippedWeapon) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::DropWeapon - EquippedWeapon is null.")) }
}
#include "CharacterComponents/CombatComponent.h"

#include "Weapons/MyWeapon.h"

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

	}
	else if(!MyCharacter) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - MyCharacter is null.")) }
	else if(!WeaponToEquip1) { UE_LOG(LogTemp, Warning, TEXT("UCombatComponent::EquipWeapon - %s is null."), *WeaponToEquip1->GetName()) }
}


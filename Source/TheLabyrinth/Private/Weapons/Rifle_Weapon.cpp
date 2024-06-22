#include "Weapons/Rifle_Weapon.h"

ARifle_Weapon::ARifle_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SetDefaults();
}

void ARifle_Weapon::BeginPlay()
{
	Super::BeginPlay();

}

void ARifle_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARifle_Weapon::SetDefaults()
{
	Tags.Add("Rifle");
}

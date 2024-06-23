#include "Weapons/Shotgun_Weapon.h"

AShotgun_Weapon::AShotgun_Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SetDefaults();
}

void AShotgun_Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void AShotgun_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShotgun_Weapon::SetDefaults()
{
	Tags.Add("Shotgun");
}

#pragma once

#include "CoreMinimal.h"
#include "Weapons/MyWeapon.h"
#include "Shotgun_Weapon.generated.h"

UCLASS()
class THELABYRINTH_API AShotgun_Weapon : public AMyWeapon
{
	GENERATED_BODY()
	
public:

	AShotgun_Weapon();

private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetDefaults();
};

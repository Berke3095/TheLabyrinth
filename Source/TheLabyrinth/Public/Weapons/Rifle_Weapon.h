#pragma once

#include "CoreMinimal.h"
#include "Weapons/MyWeapon.h"
#include "Rifle_Weapon.generated.h"

UCLASS()
class THELABYRINTH_API ARifle_Weapon : public AMyWeapon
{
	GENERATED_BODY()

public:

	ARifle_Weapon();
	
private:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetDefaults();
};

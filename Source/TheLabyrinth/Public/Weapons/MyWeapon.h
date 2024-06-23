#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped State")
};

UCLASS()
class THELABYRINTH_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	

	AMyWeapon();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponMesh{};

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* AreaSphere{};

	void SetDefaults();

	EWeaponState WeaponState{ EWeaponState::EWS_Initial };
};

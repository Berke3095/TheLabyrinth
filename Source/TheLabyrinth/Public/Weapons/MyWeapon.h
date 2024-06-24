#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped State"),
	EWS_Dropped UMETA(DisplayName = "Dropped State"),
	EWS_NONE UMETA(DisplayName = "NONE")
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
	USkeletalMeshComponent* WeaponReplicatedMesh{};

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* AreaSphere{};

	void SetDefaults();

	EWeaponState WeaponState{ EWeaponState::EWS_Initial };

public:

	void SetWeaponState(EWeaponState State1) { WeaponState = State1; }

	void SetEquippedWeaponSettings();
	void SetDroppedWeaponSettings();
};

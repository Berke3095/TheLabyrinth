#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_IsEquipped UMETA(DisplayName = "IsEquipped State"),
	EWS_IsDropped UMETA(DisplayName = "IsDropped State"),
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	void SetDefaults();

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_WeaponProperties)
	USkeletalMeshComponent* WeaponReplicatedMesh{};

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_WeaponProperties)
	USphereComponent* AreaSphere{};
	UPROPERTY(ReplicatedUsing = OnRep_WeaponProperties)
	EWeaponState WeaponState{ EWeaponState::EWS_Initial };
	UFUNCTION()
	void OnRep_WeaponProperties();

public:

	void SetWeaponState(EWeaponState WeaponState1) { WeaponState = WeaponState1; }

	void SetEquippedWeaponSettings();
	void SetDroppedWeaponSettings();
};

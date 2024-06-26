#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_IsDropped UMETA(DisplayName = "IsDropped State"),
	EWS_IsEquipped UMETA(DisplayName = "IsEquipped State"),
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

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponReplicatedMesh{};

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponFPSMesh{};

	void SetMeshes();

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* AreaSphere{};

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState)
	EWeaponState WeaponState{ EWeaponState::EWS_IsDropped };

	UFUNCTION()
	void OnRep_WeaponState();

public:

	void SetWeaponState(EWeaponState WeaponState1);

	void SetEquippedWeaponSettings();
	void SetDroppedWeaponSettings();

	FORCEINLINE USkeletalMeshComponent* GetWeaponReplicatedMesh() const { return WeaponReplicatedMesh ? WeaponReplicatedMesh : nullptr; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponFPSMesh() const { return WeaponFPSMesh ? WeaponFPSMesh : nullptr; }
};

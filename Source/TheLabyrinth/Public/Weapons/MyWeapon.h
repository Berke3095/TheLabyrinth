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

USTRUCT(BlueprintType)
struct FWeaponProperties
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponReplicatedMesh;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* AreaSphere;

	UPROPERTY(EditDefaultsOnly)
	EWeaponState WeaponState{ EWeaponState::EWS_Initial };
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
	FWeaponProperties WeaponProperties{};
	UFUNCTION()
	void OnRep_WeaponProperties();

public:

	void SetEquippedWeaponSettings();
	void SetDroppedWeaponSettings();
};

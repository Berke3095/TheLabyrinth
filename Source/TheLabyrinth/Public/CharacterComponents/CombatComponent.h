#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AMyWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THELABYRINTH_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCombatComponent();
	friend class AMyCharacter;
private:

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AMyCharacter* MyCharacter{};
	AMyWeapon* EquippedWeapon{};

	void EquipWeapon(AMyWeapon* WeaponToEquip1);
	void DropWeapon(AMyWeapon* SwapWeapon1);
	UFUNCTION(Server, Reliable)
	void Server_PlaceWeapon(AMyWeapon* CurrentWeapon1, AMyWeapon* SwapWeapon1);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlaceWeapon(AMyWeapon* CurrentWeapon1, AMyWeapon* SwapWeapon1);
	UFUNCTION(Client, Reliable)
	void Client_AttachWeapon(USkeletalMeshComponent* CharacterFPSMesh1, USkeletalMeshComponent* WeaponFPSMesh1);
	UFUNCTION(Client, Reliable)
	void Client_DeAttachWeapon(USkeletalMeshComponent* WeaponFPSMesh1);

	const float BulletTraceLength{ 50000.0f };
	void BulletTrace();
};

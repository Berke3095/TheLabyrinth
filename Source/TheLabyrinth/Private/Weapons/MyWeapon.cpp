#include "Weapons/MyWeapon.h"

#include "Components/SphereComponent.h"

AMyWeapon::AMyWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	SetDefaults();
}

void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AMyWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyWeapon::SetDefaults()
{
	bReplicates = true;

	Tags.Add("Interactable");
	Tags.Add("Weapon");

	WeaponReplicatedMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponReplicatedMesh"));
	if (WeaponReplicatedMesh)
	{
		SetRootComponent(WeaponReplicatedMesh);
		WeaponReplicatedMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponReplicatedMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponReplicatedMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDefaults - WeaponReplicatedMesh is null.")); }

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	if (AreaSphere)
	{
		AreaSphere->SetupAttachment(RootComponent);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDefaults - AreaSphere is null.")); }
}

void AMyWeapon::SetEquippedWeaponSettings()
{
	if (AreaSphere)
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetEquippedWeaponSettings - AreaSphere is null.")); }

	if (WeaponReplicatedMesh)
	{
		WeaponReplicatedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetEquippedWeaponSettings - WeaponReplicatedMesh is null.")); }

	WeaponState = EWeaponState::EWS_Equipped;

	UE_LOG(LogTemp, Warning, TEXT("Equip settings applied"));
}

void AMyWeapon::SetDroppedWeaponSettings()
{
	if (AreaSphere)
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetEquippedWeaponSettings - AreaSphere is null.")); }

	if (WeaponReplicatedMesh)
	{
		WeaponReplicatedMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetEquippedWeaponSettings - WeaponReplicatedMesh is null.")); }

	WeaponState = EWeaponState::EWS_Dropped;
}

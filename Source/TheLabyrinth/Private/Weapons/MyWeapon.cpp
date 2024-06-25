#include "Weapons/MyWeapon.h"

#include "Components/SphereComponent.h"

#include "Net/UnrealNetwork.h"

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

void AMyWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyWeapon, WeaponReplicatedMesh);
	DOREPLIFETIME(AMyWeapon, AreaSphere);
	DOREPLIFETIME(AMyWeapon, WeaponState);
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

void AMyWeapon::OnRep_WeaponProperties()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_IsEquipped:
		SetEquippedWeaponSettings();
		break;
	case EWeaponState::EWS_IsDropped:
		SetDroppedWeaponSettings();
		break;
	default:
		break;
	}
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

	WeaponState = EWeaponState::EWS_IsEquipped;
}

void AMyWeapon::SetDroppedWeaponSettings()
{
	if (AreaSphere)
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDroppedWeaponSettings - AreaSphere is null.")); }

	if (WeaponReplicatedMesh)
	{
		WeaponReplicatedMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDroppedWeaponSettings - WeaponReplicatedMesh is null.")); }

	WeaponState = EWeaponState::EWS_IsDropped;
}
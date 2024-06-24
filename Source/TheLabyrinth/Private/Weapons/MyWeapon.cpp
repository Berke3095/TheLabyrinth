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

	DOREPLIFETIME(AMyWeapon, WeaponProperties);
}

void AMyWeapon::SetDefaults()
{
	bReplicates = true;

	Tags.Add("Interactable");
	Tags.Add("Weapon");

	WeaponProperties.WeaponReplicatedMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponReplicatedMesh"));
	if (WeaponProperties.WeaponReplicatedMesh)
	{
		SetRootComponent(WeaponProperties.WeaponReplicatedMesh);
		WeaponProperties.WeaponReplicatedMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponProperties.WeaponReplicatedMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponProperties.WeaponReplicatedMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDefaults - WeaponReplicatedMesh is null.")); }

	WeaponProperties.AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	if (WeaponProperties.AreaSphere)
	{
		WeaponProperties.AreaSphere->SetupAttachment(RootComponent);
		WeaponProperties.AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponProperties.AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponProperties.AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDefaults - AreaSphere is null.")); }
}

void AMyWeapon::OnRep_WeaponProperties()
{
	if (WeaponProperties.WeaponState == EWeaponState::EWS_Equipped)
	{
		SetEquippedWeaponSettings();
	}
	else if (WeaponProperties.WeaponState == EWeaponState::EWS_Dropped)
	{
		SetDroppedWeaponSettings();
	}
}

void AMyWeapon::SetEquippedWeaponSettings()
{
	if (WeaponProperties.AreaSphere)
	{
		WeaponProperties.AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetEquippedWeaponSettings - AreaSphere is null.")); }

	if (WeaponProperties.WeaponReplicatedMesh)
	{
		WeaponProperties.WeaponReplicatedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetEquippedWeaponSettings - WeaponReplicatedMesh is null.")); }

	WeaponProperties.WeaponState = EWeaponState::EWS_Equipped;

	UE_LOG(LogTemp, Warning, TEXT("Equip settings applied"));
}

void AMyWeapon::SetDroppedWeaponSettings()
{
	if (WeaponProperties.AreaSphere)
	{
		WeaponProperties.AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDroppedWeaponSettings - AreaSphere is null.")); }

	if (WeaponProperties.WeaponReplicatedMesh)
	{
		WeaponProperties.WeaponReplicatedMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDroppedWeaponSettings - WeaponReplicatedMesh is null.")); }

	WeaponProperties.WeaponState = EWeaponState::EWS_Dropped;
}
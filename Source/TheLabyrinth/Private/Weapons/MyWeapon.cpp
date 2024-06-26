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

	DOREPLIFETIME(AMyWeapon, WeaponState);
}

void AMyWeapon::SetDefaults()
{
	bReplicates = true;

	Tags.Add("Interactable");
	Tags.Add("Weapon");

	SetMeshes();

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

void AMyWeapon::SetMeshes()
{
	WeaponReplicatedMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponReplicatedMesh"));
	if (WeaponReplicatedMesh)
	{
		SetRootComponent(WeaponReplicatedMesh);
		WeaponReplicatedMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		WeaponReplicatedMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponReplicatedMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		WeaponReplicatedMesh->SetOwnerNoSee(true);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDefaults - WeaponReplicatedMesh is null.")); }

	WeaponFPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponFPSMesh"));
	if (WeaponFPSMesh)
	{
		WeaponFPSMesh->SetupAttachment(RootComponent);
		WeaponFPSMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		WeaponFPSMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		WeaponFPSMesh->bHiddenInGame = true;
		WeaponFPSMesh->SetOnlyOwnerSee(true);
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDefaults - WeaponFPSMesh is null.")); }
}

void AMyWeapon::OnRep_WeaponState()
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

void AMyWeapon::SetWeaponState(EWeaponState WeaponState1)
{
	WeaponState = WeaponState1;
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

	if (WeaponFPSMesh)
	{
		WeaponFPSMesh->bHiddenInGame = false;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetEquippedWeaponSettings - WeaponFPSMesh is null.")); }
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

	if (WeaponFPSMesh)
	{
		WeaponFPSMesh->bHiddenInGame = true;
	}
	else { UE_LOG(LogTemp, Warning, TEXT("AMyWeapon::SetDroppedWeaponSettings - WeaponFPSMesh is null.")); }
}
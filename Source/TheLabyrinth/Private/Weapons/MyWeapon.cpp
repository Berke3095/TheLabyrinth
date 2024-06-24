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
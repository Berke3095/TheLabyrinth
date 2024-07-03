#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "UnEquipped State"),
	ECS_EquippedRifle UMETA(DisplayName = "EquippedRifle State"),
	ECS_EquippedShotgun UMETA(DisplayName = "EquippedShotgun State"),
};
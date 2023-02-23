// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Base/AttackStruct.h"
#include "ItemStruct.Generated.h"

class AItem;
class ABasicWeapon;
class UStaticMesh;

UENUM(BlueprintType)
enum class EItemType: uint8
{
	None UMETA(DisplayName = "None"),
	Consumption UMETA(DisplayName = "Consumption"),
	Equipment UMETA(DisplayName = "Equipment"),
	Resource UMETA(DisplayName = "Resource")
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	Normal UMETA(DisplayName = "Normal"),
	Rare UMETA(DisplayName = "Rare"),
	Unique UMETA(DisplayName = "Unique"),
	Epic UMETA(DisplayName = "Epic")
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	None UMETA(DisplayName = "None"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield"),
	Armor UMETA(DisplayName = "Armor"),
	Accessory UMETA(DisplayName = "Accessory")
};

//Using In Equipment Function.
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None UMETA(DisplayName = "None"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield"),
	Armor UMETA(DisplayName = "Armor"),
	Accessory1 UMETA(DisplayName = "Accessory1"),
	Accessory2 UMETA(DisplayName = "Accessory2")
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		bool bIsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 MaxQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TSubclassOf<AItem> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		EItemRarity ItemRarity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 ItemPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		UStaticMesh* ItemMesh;

	FItemData();

	FORCEINLINE bool operator==(const FItemData& Other)
	{
		return (Name == Other.Name);
	}
	FORCEINLINE bool operator!=(const FItemData& Other)
	{
		return !(Name == Other.Name);
	}

	bool IsEmpty();

};

USTRUCT(BlueprintType)
struct FEquipmentItemData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		float CriticalHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		EGameAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		EGameAttackRangeType AttackRangeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		EGameDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		EGameElementType ElementType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
		TSubclassOf<ABasicWeapon> WeaponClass;

	FEquipmentItemData();

	FORCEINLINE bool operator==(const FEquipmentItemData& Other)
	{
		return (Name == Other.Name);
	}
	FORCEINLINE bool operator!=(const FEquipmentItemData& Other)
	{
		return !(Name == Other.Name);
	}

	bool IsSameEquipment(const FEquipmentItemData Other);
	bool IsEmpty();

};

USTRUCT(BlueprintType)
struct FConsumableItemData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString Name;

	FConsumableItemData();
};

USTRUCT(BlueprintType)
struct FItemSlotData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Quantity;

	FItemSlotData();

	FORCEINLINE bool operator==(const FItemSlotData& Other)
	{
		return (ItemName == Other.ItemName);
	}
	FORCEINLINE bool operator!=(const FItemSlotData& Other)
	{
		return !(ItemName == Other.ItemName);
	}

	bool IsSameItem(const FItemSlotData Other);
	bool IsEmpty();
};

USTRUCT(BlueprintType)
struct FShopItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		bool bLimited;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Stock;

	FShopItemData();

};

USTRUCT(BlueprintType)
struct FItemRecipeData : public FTableRowBase
{
	GENERATED_BODY()

public:

	//Result Item Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		FString ItemName;

	//Result Item Quantity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		int32 Quantity;

	//Recipe (ResourceItemName, ResourceQuantity)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		TMap<FString, int32> Recipe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		float LeadTime;
};
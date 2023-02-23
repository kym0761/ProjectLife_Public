// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Item/ItemStruct.h"
#include "GamePlay/GameTimeStruct.h"
#include "Quest/Quest.h"
#include "ProjectLIfeGameInstance.generated.h"

class UDataTable;

USTRUCT(BlueprintType)
struct FInventory
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Money;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FItemSlotData> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxCapacity;

	FInventory();

};

/**
 * ���ӿ��� �������� ����� Data Table ���� GameInstance ���.
 */
UCLASS()
class PROJECTLIFE_API UProjectLIfeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

#pragma region dataTable

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance")
		TObjectPtr<UDataTable> EquipmentDataTable;

	FItemData GetItemDataFromTable(FString Name);
	FEquipmentItemData GetEquipmentItemDataFromTable(FString Name);

#pragma endregion

#pragma region quest
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Quest")
		TArray<FQuest> QuestList;

	UFUNCTION(BlueprintCallable)
		void AddQuest(FString QuestName);

	UFUNCTION(BlueprintCallable)
		TArray<FQuest> GetAllQuests();

	UFUNCTION(BlueprintCallable)
		void ClearQuest(FString QuestName);
#pragma endregion

#pragma region inventory
	/*Inventory Store Functions*/

	//0��° ������ �� ����, �Ŀ� Ȯ��� 1 2 3... �� ����
	TMap<int32, FInventory> PlayerInventory;

	//�ʿ� �����ϴ� ���ڵ� ����
	TMap<int32, FInventory> StorageInventory;

	void GetAllInventories();
	void SetAllInventories();

	UFUNCTION(BlueprintCallable)
	void Save();
	UFUNCTION(BlueprintCallable)
	void Load();
#pragma endregion

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UUniformGridPanel;
class UItemSlot;
class UEquipSlot;

/**
 * 
 */
UCLASS(abstract)
class PROJECTLIFE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	////���� ItemSlot�� �ڵ� �󿡼� Spawn�� �����̶��, �׸��� �гο� ������ �ʿ䰡 �����Ƿ� ���� �������� �� ��.
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	//	UUniformGridPanel* GridPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
		TArray<TObjectPtr<UItemSlot>> ItemSlotArray;

	virtual void NativeConstruct() override;

	void InitInventoryWidget();

	UFUNCTION()
	void UpdateInventoryWidget();

protected:

	virtual bool Initialize() override;

	virtual void NativePreConstruct() override;
};

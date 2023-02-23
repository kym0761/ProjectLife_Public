// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

class UQuickSlotWidget;
class UInventoryWidget;
class UConditionWidget;
class UStorageWidget;
class UInteractNotifyWidget;
class UEquipmentWidget;
class UWidgetAnimation;

class AAbility;
/**
 * 
 */
UCLASS(abstract)
class PROJECTLIFE_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UQuickSlotWidget> Widget_QuickSlot;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UInventoryWidget> Widget_Inventory;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UConditionWidget> Widget_Condition;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UInteractNotifyWidget> Widget_InteractNotify;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UStorageWidget> Widget_Storage;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UEquipmentWidget> Widget_Equipment;

	UPROPERTY()
		bool bInventoryUsing;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* InventoryAnimation;

	UPROPERTY()
		bool bStorageUsing;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> StorageAnimation;

public:

	virtual void NativeConstruct() override;

	void ToggleInventory();

	void UpdateEquipment();

	void ToggleStorageWidget(AActor* StorageActorRef);

	void AddConditionIcon(AAbility* ConditionActorRef);
	void RemoveConditionIcon(AAbility* ConditionActorRef);

	void NotifyInteract(UObject* InObject);
	void UnnotifyInteract();

};

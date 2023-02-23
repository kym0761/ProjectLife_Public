// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/Item.h"
#include "Base/BasicCharacter.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Base/BasicPlayerController.h"
#include "Equipment/EquipSlot.h"
#include "Equipment/EquipmentComponent.h"

#include "../GamePlay/ProjectLIfeGameInstance.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "Kismet/KismetInputLibrary.h"
//#include "Blueprint/SlateBlueprintLibrary.h"



void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Visible);
}

FReply UItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry,InMouseEvent);

	//Drag Function Activate.
	FEventReply reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	
	return reply.NativeReply;
}

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry,InMouseEvent,OutOperation);

	if (!IsValid(InventoryComponentRef))
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryRef is Invalid..."));
		return;
	}

	if (!InventoryComponentRef->GetItems().IsValidIndex(InventorySlotNumber))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Inventory Slot Number"));
		return;
	}

	FItemSlotData itemSlotData = InventoryComponentRef->GetInventoryItem(InventorySlotNumber);

	UProjectLIfeGameInstance* gameInstance = Cast<UProjectLIfeGameInstance>(GetGameInstance());
	if (!IsValid(gameInstance))
	{
		UE_LOG(LogTemp, Warning, TEXT("Access to GameInstance Failed"));
		return;
	}

	FItemData itemData = gameInstance->GetItemDataFromTable(itemSlotData.ItemName);

	//Todo : change to is empty?
	if (itemData == FItemData())
	{
		return;
	}

	if (!IsValid(ItemSlotClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSlotClass is not Exist. Add Slot Class."));
		return;
	}


	//Create DragDisplayUI
	UItemSlot* dragDisplay = CreateWidget<UItemSlot>(GetOwningPlayer(), ItemSlotClass);

	if (!IsValid(dragDisplay))
	{
		UE_LOG(LogTemp, Warning, TEXT("dragDisplay Create Failed."));
		return;

	}

	dragDisplay->InventoryComponentRef = InventoryComponentRef;
	dragDisplay->InventorySlotNumber = InventorySlotNumber;
	dragDisplay->UpdateItemSlot();

	//Make DragDropEvent And Assign it.
	UDragDropOperation* dragDropOper = NewObject<UDragDropOperation>();
	dragDropOper->Payload = this;
	dragDropOper->DefaultDragVisual = dragDisplay;
	dragDropOper->Pivot = EDragPivot::CenterCenter;

	OutOperation = dragDropOper;

}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	//check InOperation is Existed.
	if (!IsValid( InOperation))
	{
		UE_LOG(LogTemp, Warning, TEXT("InOperation in ItemSlot is NULL."));
		return false;
	}


	/*ItemSlot*/
	{
		//Get Payload and Cast To ItemSlot.
		UItemSlot* droppedItemSlot = Cast<UItemSlot>(InOperation->Payload);

		if (IsValid(droppedItemSlot))
		{
			//drop�� �κ��丮 ������ �޾ƿ�.
			int32 drop_InventorySlotNumber = droppedItemSlot->InventorySlotNumber;
			UInventoryComponent* drop_InventoryComponent = droppedItemSlot->InventoryComponentRef;

			if (IsValid(drop_InventoryComponent) && IsValid(InventoryComponentRef)) //drop�� ���� ������ Inventory Component Ȯ��.
			{
				//drop�� ���� ��ü
				bool bSucceed = UInventoryComponent::SwapItemBetweenInventory(drop_InventoryComponent, drop_InventorySlotNumber, InventoryComponentRef, InventorySlotNumber);
				if (bSucceed)
				{
					//�κ��丮 ������Ʈ
					ABasicPlayerController* playerController = Cast<ABasicPlayerController>(GetOwningPlayer());
					if (playerController)
					{
						//UE_LOG(LogTemp,Warning,TEXT("Attempt Update Inventory"));
						//playerController->UpdateInventory();

						//TODO : Equipment�� �κ��丮ó�� ���ο��� �˾Ƽ� ���� �����͵��� ������Ʈ �ǵ��� �ؾ���.
						playerController->UpdateEquipment();
						return true;
					}
				}
			}
		}
	}


	/*EquipSlot, similar With [Item Slot] code*/
	{
		UEquipSlot* droppedEquipSlot = Cast<UEquipSlot>(InOperation->Payload);
		if (IsValid(droppedEquipSlot))
		{
			UEquipmentComponent* equipmentComp = droppedEquipSlot->GetEquipmentCompRef();
			EEquipmentSlot droppedEquipmentslot = droppedEquipSlot->EquipmentSlot;

			if (IsValid(equipmentComp))
			{
				bool bSucceed = equipmentComp->SwapWithInventory(droppedEquipmentslot, InventoryComponentRef, InventorySlotNumber);
				if (bSucceed)
				{
					UE_LOG(LogTemp, Warning, TEXT("swap inventory - equip OK? "));

					ABasicPlayerController* playerController = Cast<ABasicPlayerController>(GetOwningPlayer());
					if (playerController)
					{
						//UE_LOG(LogTemp, Warning, TEXT("Attempt Update Equipment"));
						//playerController->UpdateInventory();
						//TODO : �ٷ� ���� TODO�� ����
						playerController->UpdateEquipment();
						return true;
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("swap inventory - equip NO? "));
				}
			}
		}
	}


	return false;
}

FReply UItemSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	//TODO : �κ��丮 ������ ��� ��ɺ��� �߰��ؾ���.

	if (IsValid(InventoryComponentRef))
	{
		bool bSucceed = InventoryComponentRef->UseItemInInventory(InventorySlotNumber);
		if (bSucceed)
		{
			UpdateItemSlot();

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage((int32)FMath::Rand(), 5.0f, FColor::Turquoise, TEXT("Using Item in your Inventory"));
			}
		}
	}

	return FReply::Handled();
}

void UItemSlot::InitItemSlot(AActor* HasInventoryComponent)
{
	UInventoryComponent* inventoryComponent = HasInventoryComponent->FindComponentByClass<UInventoryComponent>();

	if (IsValid(inventoryComponent))
	{
		InventoryComponentRef = inventoryComponent;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("can't Find InventoryComponent"));
	}

	UpdateItemSlot();
}

void UItemSlot::UpdateItemSlot()
{
	// Update Slot with Current Inventory Infomation.

	if (!IsValid(InventoryComponentRef))
	{
		return;
	}

	if (!InventoryComponentRef->GetItems().IsValidIndex(InventorySlotNumber))
	{
		return;
	}

	//�κ��丮 �����͸� ������
	FItemSlotData itemSlotData = InventoryComponentRef->GetItems()[InventorySlotNumber];
	
	UProjectLIfeGameInstance* gameInstance = Cast<UProjectLIfeGameInstance>(GetGameInstance());
	if (IsValid(gameInstance))
	{
		//�κ��丮 �����͸� ������ �����ͷ� ��ȯ.
		FItemData itemData = gameInstance->GetItemDataFromTable(itemSlotData.ItemName);

		if (IsValid(SlotImage))//Set Image
		{
			if (IsValid(itemData.Thumbnail))
			{
				SlotImage->SetBrushFromTexture(itemData.Thumbnail);
			}
			else
			{
				SlotImage->SetBrushFromTexture(nullptr);
			}
		}

		if (IsValid(SlotItemNum))//Set Number
		{
			FText QuantityText;

			if (itemData.bIsStackable)
			{
				QuantityText = FText::FromString(FString::FromInt(itemSlotData.Quantity));
			}
			else
			{
				QuantityText = FText::GetEmpty();
			}

			SlotItemNum->SetText(QuantityText);
		}
	}
	else // set default
	{
		if (IsValid(SlotImage))
		{
			SlotImage->SetBrushFromTexture(nullptr);
		}
		if (IsValid(SlotItemNum))
		{
			SlotItemNum->SetText(FText::GetEmpty());
		}
	}
	
}



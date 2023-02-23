// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftWidget.h"
#include "Inventory/ItemSlot.h"
#include "CraftActor.h"
#include "Inventory/InventoryComponent.h"
#include "CraftComponent.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "CraftSelectionSlot.h"
#include "Components/ProgressBar.h"

void UCraftWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i < 5; i++)
	{
		FString slotName = FString("ItemSlot_") + FString::FromInt(i);
		UItemSlot* slot = Cast<UItemSlot>(GetWidgetFromName(FName(*slotName)));
		ItemSlotArray.Add(slot);
	}

}

void UCraftWidget::NativeDestruct()
{
	//Widget������ ��������Ʈ ���ε带 ����
	UInventoryComponent* inventoryComponent = CraftActorRef->FindComponentByClass<UInventoryComponent>();
	if (IsValid(inventoryComponent))
	{
		inventoryComponent->OnInventoryDataChanged.RemoveDynamic(this, &UCraftWidget::UpdateCraftWidget);
	}

	CraftActorRef->CraftComponent->OnCrafting.RemoveDynamic(this, &UCraftWidget::SetProgress);

	Super::NativeDestruct();
}

void UCraftWidget::InitCraftWidget(ACraftActor* CraftActor)
{
	if (!IsValid(CraftActor))
	{
		//Input failed.
		return;
	}

	CraftActorRef = CraftActor;

	if (!IsValid(CraftActorRef))
	{
		//Ref failed.
		return;
	}

	//CraftActor�� �κ��丮�� �����ϰ�, itemSlotArray�� �κ��丮�� Bind...�� 5��
	UInventoryComponent* inventoryComponent = CraftActorRef->FindComponentByClass<UInventoryComponent>();

	for (int32 i = 0; i < ItemSlotArray.Num(); i++)
	{
		ItemSlotArray[i]->InventoryComponentRef = inventoryComponent;
		ItemSlotArray[i]->InventorySlotNumber = i;
	}

	//Craft ����� ������ ���԰� �κ��丮 Bind...10��° ��ġ
	ItemSlot_Result->InventoryComponentRef = inventoryComponent;
	ItemSlot_Result->InventorySlotNumber = 10;

	UpdateCraftWidget();

	//Inventory ���� ����Ǹ� �ڵ����� Inventory Slot�� ������Ʈ�� ���� ��������Ʈ�� Bind��.
	inventoryComponent->OnInventoryDataChanged.AddDynamic(this, &UCraftWidget::UpdateCraftWidget);
	CraftActorRef->CraftComponent->OnCrafting.AddDynamic(this, &UCraftWidget::SetProgress);
}

void UCraftWidget::UpdateCraftWidget()
{
	for (int32 i = 0; i < ItemSlotArray.Num(); i++)
	{
		ItemSlotArray[i]->UpdateItemSlot();
	}

	ItemSlot_Result->UpdateItemSlot();

	UpdateSelections();
}

void UCraftWidget::Clicked_DoCrafting()
{
	//CraftResultName�� ���� ������ CraftActor�� ������ �ϵ��� ��û

	if (!IsValid(CraftActorRef))
	{
		//failed.
		return;
	}

	//Craft Actor���� ���� ��û
	CraftActorRef->RequestCrafting(CraftResultName);

}

void UCraftWidget::UpdateSelections()
{
	//���� ������ �������� ����� �����
	//CraftActor�� �κ��丮�� �����ؼ� ������ �丮�� ã�ƾ��ϹǷ� CraftActor���� ����� �޾ƿͼ� ó��

	//������ ���� �� �ִ� ������ ����� ���� Clear
	VerticalBox_CanDo->ClearChildren();

	if (!IsValid(CraftActorRef))
	{
		//Ref failed.
		return;
	}

	if (!IsValid(CraftActorRef->CraftComponent))
	{
		//Craft Comp Ref failed.
		return;
	}

	if (!IsValid(CraftSelectionSlotClass))
	{
		//failed.
		return;
	}

	//���� �� �ִ� ������ ���
	TArray<FItemRecipeData> list = CraftActorRef->CraftComponent->GetCanMakeList();

	//CraftResultName�� �ʱ�ȭ�ؾ� �ϴ��� Ȯ��.
	bool bClearResultName = true;

	for (FItemRecipeData i : list)
	{
		UCraftSelectionSlot* slot = NewObject<UCraftSelectionSlot>(GetOwningPlayer(), CraftSelectionSlotClass);
		VerticalBox_CanDo->AddChild(slot);
		slot->InitCraftSelectionSlot(this, i);


		if (i.ItemName == CraftResultName) // ������ ������� �� �߰�.
		{
			bClearResultName = false;
		}

	}

	//�ʱ�ȭ �� �ʿ� �ִٸ� �ʱ�ȭ.
	if (bClearResultName)
	{
		if (VerticalBox_CanDo->HasAnyChildren())
		{
			UCraftSelectionSlot* slot = Cast<UCraftSelectionSlot>(VerticalBox_CanDo->GetChildAt(0));
			if (IsValid(slot))
			{
				CraftResultName = slot->GetItemName();
			}
			else
			{
				CraftResultName = FString("");
			}
		}
		else
		{
			CraftResultName = FString("");
		}
	}

}

void UCraftWidget::SetCraftResultName(FString InVal)
{
	CraftResultName = InVal;
}

void UCraftWidget::SetProgress(float InVal)
{
	if (!IsValid(ProgressBar_Crafting))
	{
		return;
	}

	//InVal�� 0.0f ~ 1.0f ������, �̹� �����ϰ� Parameter�� �����Ƿ� ����� �ʿ�� ���� ��.
	ProgressBar_Crafting->SetPercent(InVal);
}

bool UCraftWidget::Initialize()
{
	bool succeed = Super::Initialize();

	if (!succeed)
	{
		return false;
	}

	//��ư ��� Bind
	if (IsValid(Button_DoCrafting))
	{
		Button_DoCrafting->OnClicked.AddDynamic(this, &UCraftWidget::Clicked_DoCrafting);
	}

	return true;
}

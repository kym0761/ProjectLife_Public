// Fill out your copyright notice in the Description page of Project Settings.


#include "ConfirmShopping.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Base/BasicPlayerController.h"
#include "ShoppingActor.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

void UConfirmShopping::NativeConstruct()
{
	Super::NativeConstruct();
}

void UConfirmShopping::Clicked_OK()
{
	//Transaction
	//ShoppingActor�� �������� ����, ���� ����.
	//ShoppingActor�� inventory Manager ���� ������ �����ϹǷ� InventoryManager ���� ���δ� ������ ����.
	
	if (IsValid(ShopOwnerRef))
	{
		bool bSucceed = ShopOwnerRef->Transaction(InventoryComponentRef, ShopItemIndex, Quantity);
		////�ŷ� ������, inventory ���� ������Ʈ.
		//if (bSucceed)
		//{
		//	ABasicPlayerController* playerController = Cast<ABasicPlayerController>(GetOwningPlayer());
		//	if (IsValid(playerController))
		//	{
		//		playerController->UpdateInventory();
		//	}
		//}
	}

	//�ŷ� �Ϸᰡ �Ƶ� ���а� �Ƶ� ��� ���ᰡ �Ǹ� �����.
	RemoveFromParent();
}

void UConfirmShopping::Clicked_Cancel()
{
	RemoveFromParent();
}

void UConfirmShopping::Clicked_Plus()
{
	int32 tempQuantity = Quantity + 1;
	bool bAffordable = false;


	if (IsValid(InventoryComponentRef))
	{
		int32 tempPrice = tempQuantity * ShopOwnerRef->Items[ShopItemIndex].ItemPrice;
		bAffordable = InventoryComponentRef->CheckEnoughMoney(tempPrice);
	}


	if (bAffordable)
	{
		Quantity = FMath::Clamp(tempQuantity, 0, ShopOwnerRef->Items[ShopItemIndex].MaxQuantity);
	}
	else
	{
		UE_LOG(LogTemp,Warning, TEXT("You Can't Buy. You Don't Have Enough Money"));
	}
}

void UConfirmShopping::Clicked_10Plus()
{
	int32 tempQuantity = Quantity + 10;
	bool bAffordable = false;

	if (IsValid(InventoryComponentRef))
	{
		int32 tempPrice = tempQuantity * ShopOwnerRef->Items[ShopItemIndex].ItemPrice;
		bAffordable = InventoryComponentRef->CheckEnoughMoney(tempPrice);
	}

	if (bAffordable)
	{
		Quantity = FMath::Clamp(tempQuantity, 0, ShopOwnerRef->Items[ShopItemIndex].MaxQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("You Can't Buy. You Don't Have Enough Money"));
	}
}

void UConfirmShopping::Clicked_Minus()
{
	Quantity = FMath::Clamp(Quantity - 1, 0, ShopOwnerRef->Items[ShopItemIndex].MaxQuantity);
}

void UConfirmShopping::Clicked_10Minus()
{
	Quantity = FMath::Clamp(Quantity - 10, 0, ShopOwnerRef->Items[ShopItemIndex].MaxQuantity);
}

FText UConfirmShopping::SetQuantityText()
{
	return FText::FromString(FString::FromInt(Quantity));
}

void UConfirmShopping::InitConfirmShopping(AShoppingActor* ShopOwner, int32 Index)
{
	if (IsValid(ShopOwner) && ShopOwner->Items.IsValidIndex(Index))
	{
		ShopOwnerRef = ShopOwner;
		ShopItemIndex = Index;
	}
	else // if not valid, Delete.
	{
		RemoveFromParent();
	}
}

bool UConfirmShopping::Initialize()
{
	bool succeed = Super::Initialize();

	if (!succeed)
	{
		return false;
	}


	if (IsValid(Button_OK))
	{
		Button_OK->OnClicked.AddDynamic(this, &UConfirmShopping::Clicked_OK);
	}

	if (IsValid(Button_Cancel))
	{
		Button_Cancel->OnClicked.AddDynamic(this, &UConfirmShopping::Clicked_Cancel);
	}

	if (IsValid(Button_Plus))
	{
		Button_Plus->OnClicked.AddDynamic(this, &UConfirmShopping::Clicked_Plus);
	}

	if (IsValid(Button_10Plus))
	{
		Button_10Plus->OnClicked.AddDynamic(this, &UConfirmShopping::Clicked_10Plus);
	}

	if (IsValid(Button_Minus))
	{
		Button_Minus->OnClicked.AddDynamic(this, &UConfirmShopping::Clicked_Minus);
	}

	if (IsValid(Button_10Minus))
	{
		Button_10Minus->OnClicked.AddDynamic(this, &UConfirmShopping::Clicked_10Minus);
	}

	if (IsValid(TextBlock_Quantity))
	{
		TextBlock_Quantity->TextDelegate.BindDynamic(this, &UConfirmShopping::SetQuantityText);
		TextBlock_Quantity->SynchronizeProperties();
	}

	InventoryComponentRef = GetOwningPlayer()->FindComponentByClass<UInventoryComponent>();

	return true;
}

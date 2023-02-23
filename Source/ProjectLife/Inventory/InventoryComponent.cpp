// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "GamePlay/ProjectLIfeGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "../Item/Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//Disabled
	PrimaryComponentTick.bCanEverTick = false;

	for (int32 i = 0; i < 30; i++)
	{
		Items.Add(FItemSlotData());
	}

	//temp
	Money = 10000;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Disabled
}

bool UInventoryComponent::CheckInventoryHasSpace()
{
	bool bHasSpace = false;

	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].Quantity == 0)
		{
			bHasSpace = true;
			break;
		}
	}

	return bHasSpace;
}

bool UInventoryComponent::SpendMoney(int32 Spend)
{
	if (Money < Spend)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(256, 5.0f, FColor::Silver, TEXT("Can't Spend Money, Demand is Over the In-Hand"));
		}

		return false;
	}
	else
	{
		Money = FMath::Clamp(Money - Spend, 0, TNumericLimits<int32>::Max());
		return true;
	}
}

void UInventoryComponent::GainMoney(int32 Gain)
{
	Money = FMath::Clamp(Money + Gain, 0, TNumericLimits<int32>::Max());

	if (Money == TNumericLimits<int32>::Max())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(256, 5.0f, FColor::Silver, TEXT("Warning! Your Money is Full."));
		}
	}
}

bool UInventoryComponent::CheckEnoughMoney(int32 ToCompare)
{
	if (Money >= ToCompare)
	{
		return true;
	}

	return false;
}

void UInventoryComponent::TryMakeInventorySpace(int32 Num)
{
	//�� ������ ����
	FItemSlotData slot;
	for (int32 i = 0; i < Num; i++)
	{
		Items.Add(slot);
	}
}

bool UInventoryComponent::SwapItemBetweenInventory(UInventoryComponent* From, int32 FromSlot, UInventoryComponent* To, int32 ToSlot)
{
	//static Function���� GetWorld()�� �ٷ� �� �� ����.
	//GEngine�� ���� �ҷ��ͼ� Valid üũ�� �� �ڿ� GetWorld�� Ȯ���ؾ���.

	if (!(IsValid(From) && IsValid(To) && IsValid(GEngine)))
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Valid Inventory"));
		return false;
	}

	if (From->Items.IsValidIndex(FromSlot) && To->Items.IsValidIndex(ToSlot))
	{
		FItemSlotData i1 = From->Items[FromSlot];
		FItemSlotData i2 = To->Items[ToSlot];

		if (!From->Items[FromSlot].IsSameItem(To->Items[ToSlot])) //�ٸ� �������� ����ִ� �����̶��, Swap��.
		{
			From->Items[FromSlot] = i2;
			To->Items[ToSlot] = i1;

			From->OnInventoryDataChanged.Broadcast();
			To->OnInventoryDataChanged.Broadcast();
			return true;
		}
		else  //���� �������� �����Ѵٸ� join
		{
			//static Function�̹Ƿ� World�� GEngine���� �޾ƿ;���.
			UWorld* world = GEngine->GetWorldFromContextObject(From, EGetWorldErrorMode::LogAndReturnNull);

			if (!IsValid(world))
			{
				//world null?
				return false;
			}

			UProjectLIfeGameInstance* gameInstance = Cast<UProjectLIfeGameInstance>(UGameplayStatics::GetGameInstance(world));

			if (!IsValid(gameInstance))
			{
				UE_LOG(LogTemp, Warning, TEXT("Access GameInstance Failed"));
				return false;
			}

			FItemData itemData = gameInstance->GetItemDataFromTable(i1.ItemName); // Get Item Data

			if (itemData.MaxQuantity >= i1.Quantity + i2.Quantity) //ToSlot ������ FromSlot ������ ���� �� ���Կ� �� ������ ����ϸ�.. "ToSlot"�� �������� ���� ��. "FromSlot"�� �� ������ ��.
			{
				i1.Quantity += i2.Quantity;
				i2 = FItemSlotData();

				From->Items[FromSlot] = i2;
				To->Items[ToSlot] = i1;

				//UE_LOG(LogTemp, Warning, TEXT("join1 Success"));
				From->OnInventoryDataChanged.Broadcast();
				To->OnInventoryDataChanged.Broadcast();
				return true;
			}
			else //���� ���ļ� �ִ� ������ �ʰ��ϸ�.. "ToSlot"���� Max Quantity ��ŭ �� , "FromSlot"���� �������� ��.
			{
				int32 temp = i1.Quantity + i2.Quantity;
				i1.Quantity = itemData.MaxQuantity;

				temp -= i1.Quantity;
				i2.Quantity = temp;

				From->Items[FromSlot] = i2;
				To->Items[ToSlot] = i1;

				//UE_LOG(LogTemp, Warning, TEXT("join2 Success"));
				From->OnInventoryDataChanged.Broadcast();
				To->OnInventoryDataChanged.Broadcast();
				return true;
			}
		}
	}

	return false;
}

FItemSlotData UInventoryComponent::GetInventoryItem(int32 SlotNumber)
{
	if (Items.IsValidIndex(SlotNumber))
	{
		//������ ������ �����ϴ� � ��?
		return Items[SlotNumber];
	}

	//���н� ��.
	return FItemSlotData();
}

bool UInventoryComponent::SetInventoryItem(int32 SlotNumber, FItemSlotData InData)
{
	if (Items.IsValidIndex(SlotNumber))
	{
		Items[SlotNumber] = InData;
		OnInventoryDataChanged.Broadcast();
		return true;
	}

	return false;
}

int32 UInventoryComponent::AddItemToInventory(FItemSlotData InData)
{
	// -1 ���� ����, 0 --> ���������� �� ��, 1 �̻�--> ����.

	if (InData.IsEmpty())
	{
		// �� ������ ���� ����.
		return -1;
	}

	UProjectLIfeGameInstance* gameinstance = Cast<UProjectLIfeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (!IsValid(gameinstance))
	{
		//������ ���� Ȯ���� �Ұ���.
		return -1;
	}

	FItemData itemData = gameinstance->GetItemDataFromTable(InData.ItemName);
	if (itemData.IsEmpty())
	{
		//�߸��� ����. ���� ����.
		return -1;
	}


	//!! : ���� �� ���̶� ��� �ƴٸ�, �κ��丮�� �ݿ��� �ǹǷ� Broadcast �ؾ���.
	FItemSlotData leftover = InData;
	for (int i = 0; i < Items.Num(); i++)
	{
		//�κ��丮���� �ְ� ���� �����۰� ���� �������� ������ ã��, ���� �� �� ������ �ƴ϶�� ���빰�� ä���.
		if (Items[i].ItemName == leftover.ItemName && Items[i].Quantity < itemData.MaxQuantity)
		{
			int32 extra = itemData.MaxQuantity - Items[i].Quantity;
			int32 tempOffset = FMath::Clamp(leftover.Quantity, 0, extra);

			Items[i].Quantity += tempOffset;
			leftover.Quantity -= tempOffset;

			if (leftover.Quantity == 0)
			{
				//������ ������ ����.
				OnInventoryDataChanged.Broadcast();
				return 0;
			}
			else if (leftover.Quantity < 0)
			{
				//������ ������ �ֱ���. �ٸ�, �κ��丮�� �ݿ��� �����Ƿ� �ϴ� 0���� ���
				OnInventoryDataChanged.Broadcast();
				return 0;
			}
		}
	}
	//������� �Դٸ�, leftover�� ���� �����ְų�, Ȥ�� ���� ������ ã�� ����.
	//�� ���� ������ ������ �ִ´�.
	for (int i = 0; i < Items.Num(); i++)
	{
		if (Items[i].IsEmpty())
		{
			SetInventoryItem(i, leftover);
			OnInventoryDataChanged.Broadcast();
			return 0;
		}
	}
	
	//Warning 1 : leftover�� �������� �ణ ����µ�, �ٴڿ� ������ ������ ó���� ����� ���� ����.
	//Warning 2 : ������ ������ �ߴµ�, ������ �ʰ��Ǽ� ���� or �κ��丮 ������ ��� ������ �ƿ� ���� ����.
	OnInventoryDataChanged.Broadcast();
	return leftover.Quantity;

}

bool UInventoryComponent::CheckPlayerInventoryHasSpace()
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		FItemSlotData item = Items[i];
		if (item.IsEmpty())
		{
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::UseItemInInventory(int32 ItemIndex)
{
	//TODO : Item ������ ���� ������ ������ �����ų� ����� ��� �ʿ���.

	if (Items.IsValidIndex(ItemIndex))
	{
		UProjectLIfeGameInstance* gameinstance = Cast<UProjectLIfeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (IsValid(gameinstance))
		{
			FItemData itemData = gameinstance->GetItemDataFromTable(Items[ItemIndex].ItemName);
			AItem* item = GetWorld()->SpawnActor<AItem>(itemData.ItemClass);
			if (IsValid(item))
			{
				item->SetOwner(GetOwner());
				item->UseItem();
				OnInventoryDataChanged.Broadcast();
				return true;
			}
		}

	}

	return false;
}

bool UInventoryComponent::CheckItemInInventory(FString ItemName, int32 Quantity)
{
	int32 t_Quantity = Quantity;

	for (int32 i = 0; i < Items.Num(); i++)
	{
		//������ ã�Ƽ� �ʿ��� ���� ã�Ƽ� ����. �׸��� t_Quantity�� <=0�� �ȴٸ� �������� ����� ������ ����.
		if (ItemName == Items[i].ItemName)
		{
			t_Quantity -= Items[i].Quantity;
			if (t_Quantity <= 0)
			{
				return true;
			}
		}
	}

	return false;
}

bool UInventoryComponent::UseItemsInInventory(FString ItemName, int32 Quantity)
{
	//�Ƹ� �� �Լ��� ����ϱ� ���� �ѹ� üũ�� �غ� �� ������, ��� ��� �ȿ����� �ѹ� �� üũ��.
	bool b1 = CheckItemInInventory(ItemName, Quantity);
	if (b1 == false)
	{
		return false;
	}

	int32 t_Quantity = Quantity;

	for (int32 i = 0; i < Items.Num(); i++)
	{
		
		if (ItemName == Items[i].ItemName)
		{
			int32 i_Quantity = Items[i].Quantity;

			if (i_Quantity == t_Quantity)
			{
				//���� ���ٸ� �Ѵ� 0���� �־���.
				t_Quantity = 0;
				Items[i] = FItemSlotData();
			}
			else if (i_Quantity > t_Quantity)
			{
				//�κ��丮 ���Կ� �ִ� �縸ŭ ����ϰ� ���� ���� ���� �κ��丮 ���Կ� �־���
				int32 temp = i_Quantity - t_Quantity;
				t_Quantity = 0;
				Items[i].Quantity = temp;
			}
			else //i_Quantity < t_Quantity
			{
				//�κ��丮 ������ ���� �ʿ��� �纸�� ������, �ʿ��� ���� ���, ���Կ��� ���� �ִ´�.
				t_Quantity -= i_Quantity;
				Items[i] = FItemSlotData();
			}

			//�ʿ��� ���� ���� ������� ����			
			if (t_Quantity <= 0)
			{
				OnInventoryDataChanged.Broadcast();
				return true;
			}
		}
	}

	//��� �� ������ ����Ǵ� ���� �κ��丮�� �̹� �������� ���� ���� �������� ���� �۵��ϹǷ�, ������� ���� ���� ����.
	return false;
}

int32 UInventoryComponent::GetItemQuantity(FString ItemName)
{
	int32 temp = TNumericLimits<int32>::Max();

	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (ItemName == Items[i].ItemName)
		{
			temp = Items[i].Quantity;
		}
	}

	return temp;
}

int32 UInventoryComponent::GetMoney() const
{
	return Money;
}

void UInventoryComponent::SetMoney(int32 InVal)
{
	Money = InVal;
}

TArray<FItemSlotData> UInventoryComponent::GetItems() const
{
	return Items;
}

void UInventoryComponent::SetItems(TArray<FItemSlotData> InVal)
{
	Items = InVal;
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftComponent.h"
#include "Inventory/InventoryComponent.h"


// Sets default values for this component's properties
UCraftComponent::UCraftComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCraftComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCraftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCraftComponent::CheckCanMake(FString ItemNameToCraft)
{
	if (!IsValid(RecipeDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : Recipe invalid"));
		return false;
	}

	FItemRecipeData* craftingRecipe = RecipeDataTable->FindRow<FItemRecipeData>(FName(*ItemNameToCraft), "");

	//�ùٸ��� ���� �丮 �����Ǵ� �۵� �Ұ���.
	if (craftingRecipe == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : no Recipe"));
		return false;
	}

	AActor* owner = GetOwner();
	if (!IsValid(owner))
	{
		//owner not valid
		return false;
	}

	UInventoryComponent* inventoryComponent = owner->FindComponentByClass<UInventoryComponent>();
	if (!IsValid(inventoryComponent))
	{
		// inventory not valid
		return false;
	}

	bool bHasSpace = inventoryComponent->CheckInventoryHasSpace();
	bool bPotentiallyHasSpace = false;

	//������ ������ �ִ��� Ȯ����. ���� Ȯ���� �ڿ� �丮�� ������ �� ����.
	for (TPair<FString, int32> i : craftingRecipe->Recipe)
	{
		FString ingredientName = i.Key; //����̸�
		int32 ingredientQuantity = i.Value; //�ʿ��� ��

		bool bCheckHasEnoughIngredient = inventoryComponent->CheckItemInInventory(ingredientName, ingredientQuantity);

		if (bCheckHasEnoughIngredient == false) //��� ������ false.
		{
			UE_LOG(LogTemp, Warning, TEXT("Warning : no enough Ingredient"));
			return false;
		}

		int32 currentQuantity = inventoryComponent->GetItemQuantity(ingredientName);
		if (currentQuantity == ingredientQuantity)
		{
			bPotentiallyHasSpace = true;
		}
	}


	//�������� �Ҹ��ϸ� ������ ���� ���� �ְų�, Ȥ�� ���� ������ ����� �ִٸ� true. �װ� �ƴϸ� false.
	return bPotentiallyHasSpace || bHasSpace;
}

void UCraftComponent::StartCrafting(FString ItemNameToCraft, bool bSlotDecide, int32 SlotNum)
{
	FItemRecipeData* recipeData = GetRecipeDataFromDataTable(ItemNameToCraft);
	if (recipeData == nullptr)
	{
		return;
	}

	Progress = 0.0f;

	//Timer�� ��ɿ� Parameter �߰��ϱ� ���� ��������Ʈ ����
	FTimerDelegate timerDelegate = FTimerDelegate::CreateUObject(this, &UCraftComponent::WhileCrafting, *recipeData, bSlotDecide, SlotNum);

	// 1/60�ʸ��� Timer�� ����ǵ��� ��.
	float interval = 1 / 60.0f;

	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().SetTimer(CraftTimer, timerDelegate, interval, true);
	}
}

void UCraftComponent::WhileCrafting(FItemRecipeData RecipeData, bool bSlotDecide, int32 SlotNum)
{
	//���� Progress�� LeadTime��ŭ ����ƴٸ� Ÿ�̸Ӹ� �����ϰ� �ϼ����� �Ѿ
	if (Progress >= RecipeData.LeadTime)
	{
		if (IsValid(GetWorld()))
		{
			GetWorld()->GetTimerManager().ClearTimer(CraftTimer);
		}
		FinishCrafting(RecipeData.ItemName, bSlotDecide, SlotNum);
	}

	//Widget�� ��� ������ �����.
	Progress += 1 / 60.0f;

	//UI�� Progress �ݿ�
	if(OnCrafting.IsBound())
	{
		OnCrafting.Broadcast(Progress/RecipeData.LeadTime);
	}
	
}

void UCraftComponent::FinishCrafting(FString ItemNameToCraft, bool bSlotDecide, int32 SlotNum)
{
	Crafting(ItemNameToCraft, true, 10);

	//�ʱ�ȭ
	Progress = 0.0f;

	//UI�� Progress �ʱ�ȭ.
	if (OnCrafting.IsBound())
	{
		OnCrafting.Broadcast(0.0f);
	}

}

void UCraftComponent::Crafting(FString ItemNameToCraft, bool bSlotDecide, int32 SlotNum)
{
	FItemRecipeData recipeData = *GetRecipeDataFromDataTable(ItemNameToCraft);

	AActor* owner = GetOwner();
	if (!IsValid(owner))
	{
		//owner not valid
		return;
	}

	UInventoryComponent* inventoryComponent = owner->FindComponentByClass<UInventoryComponent>();
	if (!IsValid(inventoryComponent))
	{
		// inventory not valid
		return;
	}

	//������ �������� �Һ���. �̹� ������ ���� ��ü�� �̹� üũ�����Ƿ�, �Һ�� üũ�� ���� ���� ����.
	bool bCanMake = CheckCanMake(ItemNameToCraft);
	if (bCanMake)
	{
		for (TPair<FString, int32> i : recipeData.Recipe)
		{
			FString ingredientName = i.Key; //����̸�
			int32 ingredientQuantity = i.Value; //�ʿ��� ��

			inventoryComponent->UseItemsInInventory(ingredientName, ingredientQuantity);
		}
	}

	//�ϼ�ǰ
	FItemSlotData result;
	result.ItemName = recipeData.ItemName;
	result.Quantity = recipeData.Quantity;

	if (bSlotDecide)
	{
		//������ ������ �ִٸ� �� ���� ��ȣ�� inventory ������ �������� �����ϵ��� �Ѵ�.
		inventoryComponent->SetInventoryItem(SlotNum, result);
	}
	else
	{
		//������ ������ ���ٸ� �� ������ ������ ������.
		//AddItemToInventory�� ������ ���� Ȯ�� ���� ó���� �Ǿ� ����.
		inventoryComponent->AddItemToInventory(result);
	}
	
}

FItemRecipeData* UCraftComponent::GetRecipeDataFromDataTable(FString ItemNameToCraft)
{

	if (!IsValid(RecipeDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : Recipe invalid"));
		return nullptr;
	}

	FItemRecipeData* craftingRecipe = RecipeDataTable->FindRow<FItemRecipeData>(FName(*ItemNameToCraft), "");
	
	return craftingRecipe;
}

TArray<FItemRecipeData> UCraftComponent::GetCanMakeList() const
{
	//���� �� �ִ� �丮 ����� UI�� ������.
	//���� Crafting�� �����ϰ� �κ��丮�� ��ᰡ �����ϴ��� Ȯ���ϰ� ������ �����Ǹ� ����Ʈ�� �־��ְ� ����.
	TArray<FItemRecipeData> result;

	if (!IsValid(RecipeDataTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : Recipe invalid"));
		return result; // empty list
	}

	AActor* owner = GetOwner();
	if (!IsValid(owner))
	{
		//owner not valid
		return result;
	}

	UInventoryComponent* inventoryComponent = owner->FindComponentByClass<UInventoryComponent>();
	if (!IsValid(inventoryComponent))
	{
		// inventory not valid
		return result;
	}

	TArray<FItemRecipeData*> arr;
	RecipeDataTable->GetAllRows<FItemRecipeData>("", arr);

	for (FItemRecipeData* recipeItem : arr)
	{
		if (recipeItem == nullptr)
		{
			continue;
		}

		bool bCanMake = true;

		for (TPair<FString, int32> i : recipeItem->Recipe)
		{
			FString ingredientName = i.Key; //����̸�
			int32 ingredientQuantity = i.Value; //�ʿ��� ��

			bool bCheckHasEnoughIngredient = inventoryComponent->CheckItemInInventory(ingredientName, ingredientQuantity);

			if (bCheckHasEnoughIngredient == false)
			{
				bCanMake = false;
				break;
			}
		}

		//������ ������ �������� ��Ͽ� �߰���.
		if (bCanMake)
		{
			result.Add(*recipeItem);
		}
	}

	return result;
}


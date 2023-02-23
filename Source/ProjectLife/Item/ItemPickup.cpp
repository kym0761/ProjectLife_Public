// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Base/BasicPlayerController.h"
#include "GamePlay/ProjectLIfeGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryComponent.h"

// Sets default values
AItemPickup::AItemPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->InitBoxExtent(FVector(32.0f));
	Box->SetCollisionProfileName(TEXT("Item"));
	//Collision->SetVisibility(false);
	Box->ShapeColor = FColor::Black;
	Box->SetSimulatePhysics(true);
	Box->GetBodyInstance()->bLockXRotation = true;
	Box->GetBodyInstance()->bLockYRotation = true;
	Box->GetBodyInstance()->bLockZRotation = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(64.0f);
	Sphere->SetupAttachment(RootComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);


}

// Called when the game starts or when spawned
void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
	
	RandomItem();

}

// Called every frame
void AItemPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemPickup::Interact_Implementation(APawn* InteractCauser)
{
	//�÷��̾��� �κ��丮�� �ִ´�.
	//����, leftover�� 1�̻��̶�� Pickup�� �����־���Ѵ�. �� pickup�� ������ leftover��ŭ �����.
	//0�̸� �κ��丮�� ������ ��. -1 ���ϸ� �߸��� ��������. �Ѵ� pickup�� ������.

	if (!IsValid(InteractCauser))
	{
		return;
	}

	UInventoryComponent* inventoryComponent = InteractCauser->GetController()->FindComponentByClass<UInventoryComponent>();

	if (!IsValid(inventoryComponent))
	{
		return;
	}

	int32 leftover = inventoryComponent->AddItemToInventory(ItemSlotData);

	//ABasicPlayerController* playerController = InteractCauser->GetController<ABasicPlayerController>();
	//if (playerController)
	//{
	//	playerController->UpdateInventory();
	//}

	if (leftover <= 0)
	{
		Destroy();
	}
	else
	{
		ItemSlotData.Quantity = leftover;
	}

}

void AItemPickup::RandomItem()
{
	//item ������ ���� �ÿ��� ���� ������ �����ͼ� ���� ������.
	if (ItemSlotData.IsEmpty())
	{
		UProjectLIfeGameInstance* gameInstance = Cast<UProjectLIfeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (IsValid(gameInstance))
		{
			TArray<FName> names = gameInstance->ItemDataTable->GetRowNames();

			int randIndex = FMath::RandRange(0, names.Num() - 1);

			FItemData itemData = gameInstance->GetItemDataFromTable(names[randIndex].ToString());

			ItemSlotData.ItemName = itemData.Name;
			ItemSlotData.Quantity = 1;

			Mesh->SetStaticMesh(itemData.ItemMesh);
		}
	}
}

//bool AItemPickup::UseItem_Implementation()
//{
//
//	FItemData itemdata;
//
//	UProjectLIfeGameInstance* gameInstance = Cast<UProjectLIfeGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	if (IsValid(gameInstance))
//	{
//		itemdata = gameInstance->GetItemDataFromTable(ItemName);
//
//		switch (itemdata.ItemType)
//		{
//		case EItemType::Consumption:
//			UE_LOG(LogTemp, Warning, TEXT("C++ UseItem(), Type is Consumption"));
//			break;
//		case EItemType::Equipment:
//			UE_LOG(LogTemp, Warning, TEXT("C++ UseItem(), Type is Equipment"));
//			break;
//		case EItemType::Resource:
//			UE_LOG(LogTemp, Warning, TEXT("Resource Item Will not Need Use Item function Maybe..?"));
//			break;
//		default:
//			UE_LOG(LogTemp, Warning, TEXT("I don't Know.."));
//			break;
//		}
//
//		return true;
//	}
//
//	return false;
//}


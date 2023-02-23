// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftActor.h"
#include "Inventory/InventoryComponent.h"
#include "CraftComponent.h" 
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Base/BasicPlayerController.h"
#include "CraftWidget.h"

// Sets default values
ACraftActor::ACraftActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionProfileName(FName("OverlapAll"));
	Sphere->SetSphereRadius(128.0f);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	CraftComponent = CreateDefaultSubobject<UCraftComponent>(TEXT("CraftComponent"));
	bCanUse = true;

}

// Called when the game starts or when spawned
void ACraftActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACraftActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACraftActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//TODO : EndOverlap�� �Ǹ� ������̴� CraftActor UI�� �����Ͽ� ��� �÷��̷� ���ư� �� �ְ� ��.
	//�ʿ���� �ȴٸ� ���� �ȸ��� ��.

	//������ �ִ� CraftWidget ����
	if (IsValid(CraftWidgetRef))
	{
		CraftWidgetRef->RemoveFromParent();
		CraftWidgetRef = nullptr;
	}
}

void ACraftActor::Interact_Implementation(APawn* InteractCauser)
{

	//Add To Viewport Storage UI.
	ABasicPlayerController* playerController = InteractCauser->GetController<ABasicPlayerController>();
	if (!IsValid(playerController))
	{
		//controller failed
		return;
	}

	//CraftWidget ���� UI �������Ʈ ���� Ȯ��
	if (!IsValid(CraftWidgetClass))
	{
		return;
	}

	//������ �ִ� CraftWidget ����
	if (IsValid(CraftWidgetRef))
	{
		CraftWidgetRef->RemoveFromParent();
		CraftWidgetRef = nullptr;
	}

	//���� �� �ʱ�ȭ.
	CraftWidgetRef = CreateWidget<UCraftWidget>(playerController, CraftWidgetClass);
	if (!IsValid(CraftWidgetRef))
	{
		return;
	}
	CraftWidgetRef->AddToViewport();
	CraftWidgetRef->InitCraftWidget(this);

}

void ACraftActor::RequestCrafting(FString ItemNameToCraft)
{
	if (bCanUse == false)
	{
		//���� ���̹Ƿ� ���� ��û�� �ް� ������ �ȵ�.
		return;
	}

	if (!IsValid(CraftComponent))
	{
		return;
	}
	
	//CraftComponent�� ���� ���� ���� üũ�ϰ� ���ۿ� ��.
	bool bCanMake = CraftComponent->CheckCanMake(ItemNameToCraft);

	if (IsValid(InventoryComponent))
	{
		FItemSlotData temp = InventoryComponent->GetInventoryItem(10);
		if (!temp.IsEmpty())
		{
			return;
		}
	}

	if (bCanMake)
	{
		CraftComponent->StartCrafting(ItemNameToCraft, true, 10);
	}

}

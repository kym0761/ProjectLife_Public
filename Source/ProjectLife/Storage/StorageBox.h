// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemStruct.h"
#include "Base/InteractiveInterface.h"
#include "GameFramework/Actor.h"
#include "StorageBox.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UStorageWidget;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class PROJECTLIFE_API AStorageBox : public AActor, public IInteractive
{
	GENERATED_BODY()
public:

	AStorageBox();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<USphereComponent> Sphere;

	//���̷�Ż �޽ø� ���� ������ ���� ���� �ݴ� �Ÿ� �޽ÿ� �־��� ������.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		TObjectPtr<USkeletalMeshComponent> StorageMesh;

	//TODO : Storage ���� ��ȣ�� �Űܼ� ������ �� ��Ծ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Storage")
		int32 StorageNumber;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Storage")
		bool bOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Storage")
		TObjectPtr<UInventoryComponent> InventoryComponent;

protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual void Interact_Implementation(APawn* InteractCauser);
};

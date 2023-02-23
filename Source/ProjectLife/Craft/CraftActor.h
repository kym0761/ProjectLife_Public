// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/ItemStruct.h"
#include "Base/InteractiveInterface.h"
#include "CraftActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UInventoryComponent;
class UCraftComponent;
class UCraftWidget;

UCLASS()
class PROJECTLIFE_API ACraftActor : public AActor, public IInteractive
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Craft")
		TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Craft")
		TObjectPtr<UCraftComponent> CraftComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
		TSubclassOf<UCraftWidget> CraftWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Craft")
		bool bCanUse;

private:

	//���� �ڽſ��� �ɸ� CraftWidgetRef ���� �뵵
	UPROPERTY()
		TObjectPtr<UCraftWidget> CraftWidgetRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual void Interact_Implementation(APawn* InteractCauser);

	void RequestCrafting(FString ItemNameToCraft);

};

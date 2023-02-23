// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemStruct.h"
#include "GameFramework/Actor.h"
#include "BasicWeapon.generated.h"

class UStaticMeshComponent;
class UStatComponent;
UCLASS()
class PROJECTLIFE_API ABasicWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
		TObjectPtr<UStatComponent> OwnerStatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FEquipmentItemData WeaponData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AttackStart();
	virtual void AttackEnd();
};

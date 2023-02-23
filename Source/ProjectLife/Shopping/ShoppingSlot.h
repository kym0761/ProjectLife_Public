// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemStruct.h"
#include "Blueprint/UserWidget.h"
#include "ShoppingSlot.generated.h"

class UImage;
class UTextBlock;
class AShoppingActor;
class UConfirmShopping;
/**
 * 
 */
UCLASS(abstract)
class PROJECTLIFE_API UShoppingSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UImage> SlotImage;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UTextBlock> SlotItemName;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UTextBlock> SlotItemPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping")
		TSubclassOf<UConfirmShopping> ConfirmShoppingClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shopping")
		int32 ItemIndex;

private:

	UPROPERTY()
		TObjectPtr<AShoppingActor> ShopOwnerRef;

public:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void InitShoppingSlot(AShoppingActor* ShopOwner, int32 Index);
};

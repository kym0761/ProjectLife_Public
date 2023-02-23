// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemStruct.h"
#include "CraftSelectionSlot.generated.h"

class UImage;
class UTextBlock;
class UCraftWidget;


/**
 * 
 */
UCLASS()
class PROJECTLIFE_API UCraftSelectionSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UImage> Image_SlotImage;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UTextBlock> TextBlock_SlotItemName;

private:

	//���� ���ý� ������ ������ �ҷ��� �� �ֵ��� ������ �̸��� ������.
	UPROPERTY()
		FString ItemNameText;

	//Parent ���ٿ�
	UPROPERTY()
		TObjectPtr<UCraftWidget> ParentCraftWidgetRef;

public:

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void InitCraftSelectionSlot(UCraftWidget* ParentCraftWidget, FItemRecipeData RecipeData);

	void SetCraftWidgetSelection();

	FString GetItemName() const;
};

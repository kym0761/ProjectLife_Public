// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftWidget.generated.h"

class UItemSlot;
class UButton;
class ACraftActor;
class UVerticalBox;
class UCraftSelectionSlot;
class UProgressBar;

/**
 * 
 */
UCLASS()
class PROJECTLIFE_API UCraftWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	//CraftActor�� �κ��丮�� ���� ������ ����... 5���� ���
	UPROPERTY()
		TArray<TObjectPtr<UItemSlot>> ItemSlotArray;

	//Crafting�� ��� ... �κ��丮 10��° ��ġ
	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UItemSlot> ItemSlot_Result;

	//Crafting ����
	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UButton> Button_DoCrafting;

	//Crafting�� ������ ������ ���.
	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UVerticalBox> VerticalBox_CanDo;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UProgressBar> ProgressBar_Crafting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
		TSubclassOf<UCraftSelectionSlot> CraftSelectionSlotClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FString CraftResultName;

private:

	UPROPERTY()
		TObjectPtr<ACraftActor> CraftActorRef;

public:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void InitCraftWidget(ACraftActor* CraftActor);

	UFUNCTION()
		void UpdateCraftWidget();

	UFUNCTION()
		void Clicked_DoCrafting();

	void UpdateSelections();

	void SetCraftResultName(FString InVal);

	//InVal == 0.0f ~ 1.0f
	UFUNCTION() // CraftComponent�� OnCrafting ��������Ʈ ����� ���� UFUNCTION()
	void SetProgress(float InVal);

protected:

	virtual bool Initialize() override;
};

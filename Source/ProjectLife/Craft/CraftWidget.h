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

	//CraftActor의 인벤토리에 접근 가능한 슬롯... 5개만 사용
	UPROPERTY()
		TArray<TObjectPtr<UItemSlot>> ItemSlotArray;

	//Crafting의 결과 ... 인벤토리 10번째 위치
	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UItemSlot> ItemSlot_Result;

	//Crafting 실행
	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UButton> Button_DoCrafting;

	//Crafting이 가능한 아이템 목록.
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
	UFUNCTION() // CraftComponent의 OnCrafting 델리게이트 사용을 위한 UFUNCTION()
	void SetProgress(float InVal);

protected:

	virtual bool Initialize() override;
};

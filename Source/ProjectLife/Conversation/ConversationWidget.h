// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConversationStruct.h"
#include "ConversationWidget.generated.h"

class UTextBlock;
class UButton; 
class UVerticalBox;
class UChoiceSlot;
/**
 * 
 */
UCLASS()
class PROJECTLIFE_API UConversationWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UTextBlock> TextBlock_NameText;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UTextBlock> TextBlock_ConversationText;

	UPROPERTY( BlueprintReadWrite, Meta = (BindWidget))
		TObjectPtr<UButton> Button_Next;

	UPROPERTY( BlueprintReadWrite, Meta = (BindWidget))
		TObjectPtr<UButton> Button_End;

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UVerticalBox> VerticalBox_Choices;

	//FTimerHandle DialogueTimer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dialogue", meta = (MinClamp = 0.01f, MaxClamp = 1.0f))
		float ConversationSpeed;

	//��ȭ ������ ��� DataTable
	UPROPERTY(BlueprintReadWrite, Category = "Conversation")
		TArray<TObjectPtr<UDataTable>> ConversationDataTable;

	//�������� ��� DataTable
	UPROPERTY(BlueprintReadWrite, Category = "Conversation")
		TArray<TObjectPtr<UDataTable>> ChoiceDataTable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Conversation")
		TSubclassOf<UChoiceSlot> ChoiceSlot_BP;

private:

	//��ȭ ���� ���
	UPROPERTY()
		TArray<FConversationData> Conversations;

	//TextOffset
	UPROPERTY()
		int32 CurrentConversationPos;

	//Current Conversation Number;
	UPROPERTY()
		int32 CurrentConversationNum;

	//Current Conversation Number;
	UPROPERTY()
		int32 CurrentConversationDataTableNum;

	//Current Choice Number;
	UPROPERTY()
		int32 CurrentChoiceDataTableNum;

	UPROPERTY()
	float ConversationTick;

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
		void InitConversationWidget(TArray<UDataTable*> InConversations);

	UFUNCTION(BlueprintCallable)
		void InitConversationWidget_Choice(TArray<UDataTable*> InChoices);

	void InitConversation();

	UFUNCTION(BlueprintCallable)
		void StartConversation();

	void SetNameText();
	void SetConversationText();

	UFUNCTION()
		void ConversationFunction();

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	void ReceiveChoice(int32 ChoiceNumber);

};

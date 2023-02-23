// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/KismetStringLibrary.h"
#include "Components/VerticalBox.h"
#include "ChoiceSlot.h"

void UConversationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ConversationSpeed = 0.1f;

	CurrentConversationPos = 0;
	CurrentConversationNum = 0;
	CurrentConversationDataTableNum = 0;
	CurrentChoiceDataTableNum = 0;

}

void UConversationWidget::InitConversationWidget(TArray<UDataTable*> InConversations)
{
	ConversationDataTable = InConversations;

	InitConversation();

	StartConversation();
}

void UConversationWidget::InitConversationWidget_Choice(TArray<UDataTable*> InChoices)
{
	ChoiceDataTable = InChoices;
}

void UConversationWidget::InitConversation()
{
	//Init All
	Conversations.Empty();
	Button_Next->SetVisibility(ESlateVisibility::Visible);
	CurrentConversationNum = -1;
	CurrentConversationPos = 0;
	VerticalBox_Choices->ClearChildren();

	//Set First Conversation
	TArray<FConversationData*> data;
	ConversationDataTable[CurrentConversationDataTableNum]->GetAllRows<FConversationData>("", data);
	for (int i = 0; i < data.Num(); i++)
	{
		Conversations.Add(*data[i]);
	}

}

void UConversationWidget::StartConversation()
{
	if (CurrentConversationNum >= Conversations.Num() - 1)
	{
		// ��ȭ ��Ȱ��ȭ
		return;
	}

	CurrentConversationNum++;

	//������ ��ȭ���, ���� ��ȭ ��ư�� ������.
	if (CurrentConversationNum>= Conversations.Num() - 1)
	{
		Button_Next->SetVisibility(ESlateVisibility::Hidden);

		//�������� �ִٸ� �����ش�.
		if (ChoiceDataTable.IsValidIndex(CurrentChoiceDataTableNum))
		{
			TArray<FConversationChoiceData*> data;
			ChoiceDataTable[CurrentChoiceDataTableNum]->GetAllRows<FConversationChoiceData>("", data);
			//������ŭ ������.
			for (int i = 0; i < data.Num(); i++)
			{
				UChoiceSlot* slot = NewObject<UChoiceSlot>(GetOwningPlayer(),ChoiceSlot_BP);
				if (IsValid(slot))
				{
					slot->AddToViewport();
					slot->InitChoiceSlot(this,*data[i], i + 1);
					VerticalBox_Choices->AddChild(slot);
					UE_LOG(LogTemp, Warning, TEXT("Slot Add OK"));
				}
			}			
		}
	}

	//�̸� , ��ȭ ���� ����
	SetNameText();
	SetConversationText();
	
	//TODO : ĳ���� �̹����� ��ȭâ�� ������ �ʿ���.

}

void UConversationWidget::SetNameText()
{
	if (IsValid(TextBlock_NameText))
	{
		TextBlock_NameText->SetText(FText::FromString(Conversations[CurrentConversationNum].Name));
	}
}

void UConversationWidget::SetConversationText()
{
	if (IsValid(TextBlock_ConversationText))
	{
		//�ؽ�Ʈ�� �ٲ� �� �ʱ�ȭ
		CurrentConversationPos = 0;
		//UI �������Ʈ�� �����ϴ� �ӽ� �ؽ�Ʈ ����
		TextBlock_ConversationText->SetText(FText());

		ConversationFunction();
	}
}

void UConversationWidget::ConversationFunction()
{
	FString conversationText = Conversations[CurrentConversationNum].ConversationText;

	if (conversationText.Len() <= CurrentConversationPos)
	{
		return;
	}

	//�۵����� �����ִ� �ؽ�Ʈ 1�� �������Ѽ� ��ȭ�ϴ� ������ ������ ��.
	CurrentConversationPos = FMath::Clamp(CurrentConversationPos + 1, 0, conversationText.Len());
	FString substring = UKismetStringLibrary::GetSubstring(conversationText, 0, CurrentConversationPos);
	TextBlock_ConversationText->SetText(FText::FromString(substring));

}

void UConversationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ConversationTick += InDeltaTime;

	//�ð��� ������ ������ ���� �ؽ�Ʈ�� ��µǵ��� ��.
	//����) �� / �ȳ� / �ȳ��� / �ȳ��ϼ� / �ȳ��ϼ���
	//Timer�� ������ Pause�Ǹ� ���߹Ƿ� UI�� Tick�� �̿��� ������.
	if (ConversationTick >= ConversationSpeed)
	{
		ConversationTick = 0.0f;
		ConversationFunction();
	}

}

void UConversationWidget::ReceiveChoice(int32 ChoiceNumber)
{

	if (ChoiceNumber <= 0)
	{
		//Error.
		return;
	}

	//TODO : ���޹��� ������ ��ȣ�� ���� ���� ��ȭ���� ��.
	CurrentConversationDataTableNum += ChoiceNumber;
	CurrentChoiceDataTableNum++;

	InitConversation();
	StartConversation();
}

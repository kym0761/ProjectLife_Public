// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeechWidgetComponent.h"
#include "Conversation/SpeechBubbleWidget.h"
USpeechWidgetComponent::USpeechWidgetComponent()
{
	//Don't false in WidgetComponent. or it won't do Working.
	PrimaryComponentTick.bCanEverTick = true;

	SetWidgetSpace(EWidgetSpace::Screen);
	SetVisibility(false);
	SetHiddenInGame(true);

}

void USpeechWidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	//SpeechWidgetComponent�ȿ� �����ϴ� ��ǳ�� �������� �������Ʈ�� ����
	USpeechBubbleWidget* bubbleRef = Cast<USpeechBubbleWidget>(GetUserWidgetObject());
	if (bubbleRef)
	{
		bubbleRef->OnSpeechEnd.AddDynamic(this, &USpeechWidgetComponent::SpeechEnd);
		bubbleRef->SetNameText(GetOwner()->GetName());
	}
}

void USpeechWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpeechWidgetComponent::SpeechTrigger(FString InString)
{
	USpeechBubbleWidget* bubbleRef = Cast<USpeechBubbleWidget>(GetUserWidgetObject());

	if(bubbleRef)
	{
		SetVisibility(true);
		SetHiddenInGame(false);
		bubbleRef->StartSpeech(InString);
	}
}

void USpeechWidgetComponent::SpeechEnd()
{
	SetVisibility(false);
	SetHiddenInGame(true);
}

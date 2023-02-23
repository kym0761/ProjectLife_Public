// Fill out your copyright notice in the Description page of Project Settings.


#include "CalendarWidget.h"
#include "Components/UniformGridPanel.h"
#include "GamePlay/GameTimeStruct.h"
#include "CalendarDummySlot.h"
#include "CalendarDateSlot.h"
#include "GamePlay/ProjectLifeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetTextLibrary.h"

void UCalendarWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCalendarWidget::InitCalendar()
{
	if (!(IsValid(CalendarGridPanel) && IsValid(CalendarDateSlotClass) && IsValid(CalendarDateSlotClass) && IsValid(CalendarDateSlot_Today_Class)))
	{
		return;
	}

	if (!(IsValid(TextBlock_Year) && IsValid(TextBlock_Month)))
	{
		return;
	}


	AProjectLifeGameState* gameState = Cast<AProjectLifeGameState>(UGameplayStatics::GetActorOfClass(GetWorld(), AProjectLifeGameState::StaticClass()));
	if (IsValid(gameState))
	{
		FInGameTime inGameTime = gameState->InGameTime;

		int32 year = inGameTime.Year;
		int32 month = inGameTime.Month;

		int32 totalDay = (inGameTime.Year * FInGameTime::MAXMONTH * FInGameTime::MAXDAY)
			+ ((inGameTime.Month - 1) * FInGameTime::MAXDAY);

		//sun = 0, mon = 1 , tues = 2, weds = 3, thur = 4 , fri = 5, sat = 6
		int32 firstDayOfMonth = totalDay % FInGameTime::WEEK;


		int32 row = 0;
		int32 column = 0;
		int32 currentDay = 1;

		//UI TextBlock �߰�
		TextBlock_Year->SetText(UKismetTextLibrary::Conv_IntToText(year));
		TextBlock_Month->SetText(UKismetTextLibrary::Conv_IntToText(month));

		//ù �� ���� ���� �߰�. ex) 1���� �������̸� �� �� ȭ�� ���� �����̾�� ��.
		for (column = 0; column < FInGameTime::WEEK; column++)
		{
			if (column < firstDayOfMonth)
			{
				UCalendarDummySlot* dummySlot = CreateWidget<UCalendarDummySlot>(GetOwningPlayer(), CalendarDummySlotClass);
				if (IsValid(dummySlot))
				{
					CalendarGridPanel->AddChildToUniformGrid(dummySlot, row, column);
				}
			}
			else
			{
				break;
			}
		}

		//���� ����� ���� Calendar Slot
		while (currentDay <= inGameTime.MAXDAY)
		{
			UCalendarDateSlot* dateSlot;
			if (currentDay == inGameTime.Day)
			{
				dateSlot = CreateWidget<UCalendarDateSlot>(GetOwningPlayer(), CalendarDateSlot_Today_Class);
			}
			else
			{
				dateSlot = CreateWidget<UCalendarDateSlot>(GetOwningPlayer(), CalendarDateSlotClass);
			}

			if (IsValid(dateSlot))
			{
				CalendarGridPanel->AddChildToUniformGrid(dateSlot, row, column);
				dateSlot->SetDay(currentDay);
				column++;
				currentDay++;
			}

			if (column >= FInGameTime::WEEK)
			{
				row++;
				column = 0;
			}
		}

		//������ �� ���� ���� �߰�. ex) 30���� �������̸� �� �� ��� ���� �����̾����.
		while (column != 0 && column < FInGameTime::WEEK)
		{
			UCalendarDummySlot* dummySlot = CreateWidget<UCalendarDummySlot>(GetOwningPlayer(), CalendarDummySlotClass);
			if (IsValid(dummySlot))
			{
				CalendarGridPanel->AddChildToUniformGrid(dummySlot, row, column);
				column++;
			}
		}
	}

}

bool UCalendarWidget::Initialize()
{
	bool succeed = Super::Initialize();

	if (!succeed)
	{
		return false;
	}

	InitCalendar();

	return true;
}

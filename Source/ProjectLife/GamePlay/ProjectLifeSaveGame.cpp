// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectLifeSaveGame.h"

UProjectLifeSaveGame::UProjectLifeSaveGame()
{
	SaveSlotName = FString("GameSaveSlot");
	UserIndex = 0;

	////�κ��丮 ���� ���� 10, �ø��� ������ ���� �ʿ���.
	//for (int32 i = 0; i < 100; i++)
	//{
	//	FInventory inventory;
	//	Inventories.Add(i, inventory);
	//}
}
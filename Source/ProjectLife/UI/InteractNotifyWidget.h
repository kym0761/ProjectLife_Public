// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractNotifyWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTLIFE_API UInteractNotifyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UTextBlock> NameTextBlock;

private:

	//Interact ������Ʈ�� ���������� �����ϹǷ�, UObject�� ���� Ȯ��.
	UPROPERTY()
		TObjectPtr<UObject> ObjectRef;

public:

	virtual void NativeConstruct() override;

	void NotifyInteract(UObject* InObject);
	void UnnotifyInteract();

};

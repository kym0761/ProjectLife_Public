// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FireTimerWidget.generated.h"

class UProgressBar;
class AFireStand;
/**
 * 
 */
UCLASS(abstract)
class PROJECTLIFE_API UFireTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Meta = (BindWidget))
		TObjectPtr<UProgressBar> TimerProgressBar;

	//FireStand�� BeginPlay���� �ʱ�ȭ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
		TObjectPtr<AFireStand> FireRef;

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
		float FirePercent();
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterDeliveryComponent.h"

// Sets default values for this component's properties
UWaterDeliveryComponent::UWaterDeliveryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWaterDeliveryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWaterDeliveryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWaterDeliveryComponent::WaterDelivery()
{
	if (bIsActivated == false)
	{
		return;
	}


	//����� ���� ���� ����.
	for (UWaterDeliveryComponent* i : Connected)
	{
		if (i->GetActivate() == true)
		{
			continue;
		}
		else
		{
			i->SetActivate(true);
			i->Forwarding = this;
		}
	}

}

void UWaterDeliveryComponent::StopWatering()
{
	if (bIsActivated == true)
	{
		return;
	}


	for (UWaterDeliveryComponent* i : Connected)
	{
		if (i->GetActivate() == false)
		{
			continue;
		}
		else
		{
			i->SetActivate(false);
			i->Forwarding = nullptr;
		}
	}

}

void UWaterDeliveryComponent::SetActivate(bool InVal)
{
	bIsActivated = InVal;

	//���� ���� ���Դٸ� �ٸ� ������ ���� ���������� ����.
	if (bIsActivated == true)
	{
		WaterDelivery();
	}
	else //���� ���� �ȵ��Դٸ�, �ٸ� ������ ���� ���������� ����.
	{
		StopWatering();
	}
}

bool UWaterDeliveryComponent::GetActivate() const
{
	return bIsActivated;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractDetectComponent.h"
#include "InteractiveInterface.h"

UInteractDetectComponent::UInteractDetectComponent()
{
	HasInteractive = nullptr;

	//TODO : Make And Apply Interact Collision
}

void UInteractDetectComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UInteractDetectComponent::OnSphereBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractDetectComponent::OnSphereEndOverlap);
 
}

void UInteractDetectComponent::CheckInteract()
{
	//���� �������� ���͵��� ���� Get�ϰ�
	//�������� ���� �߿��� UInteractive �������̽��� ���� �ֵ鸸 �ɷ�����
	//UInteractive�� ���� Actor���� UInteractDetectComponent�� ���� ����� ������ ������ ��
	//HasInteractive�� ���� ����� Actor�� ����. Ȥ�� �ƹ��� ���ٸ� HasInteractive�� nullptr��.

	//Warning : ���� �������� ���¿��� �ȿ� �ִ� �ֵ� ��ġ�� �ٲ�ٰ� HasInteractive�� �ٲ��� ����
	//������ �ֱ�� CheckInteract�� �ϸ� �ذ�� ��������, ���� ���� ��ĥ �ʿ� ���� ��.

	TArray<AActor*> overlapActors;
	TArray<AActor*> hasInteractives;

	GetOverlappingActors(overlapActors);
	//�� ������Ʈ�� Owner�� ����
	overlapActors.Remove(GetOwner());

	for (AActor* i : overlapActors)
	{
		if (i->GetClass()->ImplementsInterface(UInteractive::StaticClass()))
		{
			hasInteractives.Add(i);
		}
	}

	hasInteractives.Sort([this](const AActor& a, const AActor& b)
		{
			FVector compLocation = this->GetComponentLocation();

			float distA = FVector::Distance(compLocation, a.GetActorLocation());
			float distB = FVector::Distance(compLocation, b.GetActorLocation());

			return distA < distB;
		});

	if (hasInteractives.Num() > 0)
	{
		HasInteractive = hasInteractives[0];
	}
	else
	{
		HasInteractive = nullptr;
	}

}

void UInteractDetectComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Overlap Begin �� ������ �ѹ��� ��ü �������� �Ѵ�.
	CheckInteract();

}

void UInteractDetectComponent::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Overlap End �� ������ �ѹ��� ��ü �������� �Ѵ�.
	CheckInteract();

}

AActor* UInteractDetectComponent::GetInteractiveActor() const
{
	//nullptr or Actor
	return HasInteractive;

}

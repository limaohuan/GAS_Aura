// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (const auto& Pair : AS->TagsToAttributesFuncPtr)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda
		(
			// ����Pair��AS��ֵ���ݶ��������ô��ݣ���ΪPair�Ǿֲ��������뿪��������������ָ��
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (const auto& Pair : AS->TagsToAttributesFuncPtr)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AS);
	AttributeInfoDelegate.Broadcast(Info);
}

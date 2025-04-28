// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// ���ý�ɫ�ƶ������Զ���ת���ƶ�����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// ����ɫ�ƶ�Լ����ƽ���� (��ֹZ���ƶ�)
	GetCharacterMovement()->bConstrainToPlane = true;
	// ��Ϸ��ʼʱ����ɫ���뵽ƽ��
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// ���ÿ�������ת�Խ�ɫ��ת��Ӱ��
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
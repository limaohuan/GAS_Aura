// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// 设置角色移动方向自动旋转到移动方向
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	// 将角色移动约束在平面上 (防止Z轴移动)
	GetCharacterMovement()->bConstrainToPlane = true;
	// 游戏开始时将角色对齐到平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 禁用控制器旋转对角色旋转的影响
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
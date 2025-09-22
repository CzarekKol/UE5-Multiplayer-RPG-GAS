// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{

	GENERATED_BODY()
public:

	AAuraEnemy();

	// Enemy Interface
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	// end Enemy Interface

	/* Combat Interface*/

	virtual int32 GetPlayerLevel() override;
	/* End Combat Interface*/

	UPROPERTY(BlueprintAssignable)
	FOnAttributeCHangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeCHangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterClassDefaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CharacterClassDefaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
private:
	float RedHighlightValue = 250.f;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxMana.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	InteligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	InteligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	InteligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(InteligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	//Gather Tags form source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Inteligence = 0.f;

	GetCapturedAttributeMagnitude(InteligenceDef, Spec, EvaluationParameters, Inteligence);
	Inteligence = FMath::Max<float>(Inteligence, 0.f);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();


	return 100.f + 2.f * Inteligence + 10.f * PlayerLevel;
}

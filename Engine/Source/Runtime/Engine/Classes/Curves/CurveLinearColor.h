// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.


#pragma once
#include "CurveLinearColor.generated.h"

UCLASS(BlueprintType)
class ENGINE_API UCurveLinearColor : public UCurveBase
{
	GENERATED_UCLASS_BODY()

	/** Keyframe data, one curve for red, green, blue, and alpha */
	UPROPERTY()
	FRichCurve FloatCurves[4];

	/** Evaluate this float curve at the specified time */
	UFUNCTION(BlueprintCallable, Category="Math|Curves")
	virtual FLinearColor GetLinearColorValue(float InTime) const;

	// Begin FCurveOwnerInterface
	virtual TArray<FRichCurveEditInfoConst> GetCurves() const override;
	virtual TArray<FRichCurveEditInfo> GetCurves() override;

	/** @return True if the curve as any alpha keys */
	bool HasAnyAlphaKeys() const { return FloatCurves[3].GetNumKeys() > 0; }

	/** Determine if Curve is the same */
	bool operator == (const UCurveLinearColor& Curve) const;
};


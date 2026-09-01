#pragma once
#include "PIXUIStructType.generated.h"

USTRUCT()
struct FOptionDataEditConditionDescription
{
	GENERATED_BODY()
	
public:	

	void SetEditCondition(TFunction<bool()> InEditConditionFunc)
	{
		// Set the edit condition function
		EditConditionFunc = InEditConditionFunc;
	}

	bool IsValid() const
	{
		// If EditConditionFunc is set, it should be valid; otherwise, it's considered valid by default
		return EditConditionFunc != nullptr;
	}

	bool IsEditContionMet() const
	{
		// If the edit condition function is set, call it to determine if the edit condition is met
		if (IsValid())
		{
			// Call the function to check if the edit condition is met
			return EditConditionFunc();
		}
		return true; // If no edit condition is set, default to editable
	}

	FString GetDisabledRichReason() const
	{
		return DisabledRichReason;
	}

	void SetDisabledRichReason(const FString& InRichReason)
	{
		DisabledRichReason = InRichReason;
	}

	bool HasForcedStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

	FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetDisabledForcedStringValue(const FString& InForcedValue)
	{
		DisabledForcedStringValue = InForcedValue;
	}


private:

	TFunction<bool()> EditConditionFunc;

	FString DisabledRichReason;

	TOptional<FString> DisabledForcedStringValue;
};

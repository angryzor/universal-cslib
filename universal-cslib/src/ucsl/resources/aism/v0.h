#pragma once

namespace ucsl::resources::aism::v0 {
    struct ConditionData {
        struct FixedBoolArgumentValue {
            bool value;
        };

        struct FixedIntArgumentValue {
            int value;
        };

        struct FixedFloatArgumentValue {
            float value;
        };

        struct RandomIntArgumentValue {
            int lowerBound;
            int upperBound;
        };

        struct RandomFloatArgumentValue {
            float lowerBound;
            float upperBound;
        };

        struct AttributeArgumentValue {
            const char* name;
            const char* propertyName;
        };

        union ArgumentValuePtr {
            FixedBoolArgumentValue* fixedBool;
            FixedIntArgumentValue* fixedInt;
            FixedFloatArgumentValue* fixedFloat;
            RandomIntArgumentValue* randomInt;
            RandomFloatArgumentValue* randomFloat;
            AttributeArgumentValue* attribute;
        };

        enum class ArgumentType : unsigned char {
            FIXED_BOOL,
            FIXED_INT,
            FIXED_FLOAT,
            RANDOM_BOOL,
            RANDOM_INT,
            RANDOM_FLOAT,
            ATTRIBUTE_BOOL,
            ATTRIBUTE_INT,
            ATTRIBUTE_FLOAT,
        };

        enum class Operator : unsigned char {
            GREATER_THAN,
            GREATER_THAN_OR_EQUAL,
            LESS_THAN,
            LESS_THAN_OR_EQUAL,
            EQUAL,
            NOT_EQUAL,
        };

        ArgumentValuePtr lhsValue;
        ArgumentType lhsType;
        ArgumentValuePtr rhsValue;
        ArgumentType rhsType;
        Operator op;
    };

    struct TriggerData {
        enum class TriggerType : unsigned char {
            NORMAL,
            ONLY_WHEN_BOTTOM,
            ONCE,
        };

        enum class ConditionType : unsigned char {
            ALL,
            ANY,
        };

        const char* targetStateName;
        TriggerType triggerType;
        ConditionType conditionType;
        ConditionData* conditions;
        unsigned int conditionCount;
    };

    struct ReactionData {
        const char* name;
        const char* targetStateName;
    };

	struct StateData {
        const char* name;
        const char* type;
        int superState;
        int entryChildState;
        TriggerData* triggers;
        unsigned int triggerCount;
        ReactionData* reactions;
        unsigned int reactionCount;
	};

	struct AIStateMachineData {
		StateData* states;
		unsigned int stateCount;
	};
}

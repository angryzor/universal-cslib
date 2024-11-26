#pragma once

#define UCSL_PACK(...) __VA_ARGS__
#define UCSL_NEWTYPE_WITH_ATTRS(Kind, ClassName, BaseClass, ConstructorName, Attrs) \
	Kind Attrs ClassName : public BaseClass { \
	public: \
		using BaseClass::ConstructorName; \
    \
		inline ClassName(const BaseClass& other) : BaseClass{ other } {} \
		inline ClassName(BaseClass&& other) : BaseClass{ std::move(other) } {} \
	}

#define UCSL_NEWTYPE_SIMPLE(ClassName, BaseClass) UCSL_NEWTYPE_WITH_ATTRS(class, ClassName, BaseClass, ClassName,)
#define UCSL_NEWTYPE(ClassName, BaseClass, ConstructorName) UCSL_NEWTYPE_WITH_ATTRS(class, ClassName, BaseClass, ConstructorName,)
#define UCSL_NEWTYPE_ALIGNED(ClassName, BaseClass, ConstructorName, Alignment) UCSL_NEWTYPE_WITH_ATTRS(class, ClassName, BaseClass, ConstructorName, alignas(Alignment))

#define UCSL_NEWTYPE_STRUCT_SIMPLE(ClassName, BaseClass) UCSL_NEWTYPE_WITH_ATTRS(struct, ClassName, BaseClass, ClassName,)
#define UCSL_NEWTYPE_STRUCT(ClassName, BaseClass, ConstructorName) UCSL_NEWTYPE_WITH_ATTRS(struct, ClassName, BaseClass, ConstructorName,)
#define UCSL_NEWTYPE_STRUCT_ALIGNED(ClassName, BaseClass, ConstructorName, Alignment) UCSL_NEWTYPE_WITH_ATTRS(struct, ClassName, BaseClass, ConstructorName, alignas(Alignment))

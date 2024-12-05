#pragma once
#include <ucsl/strings/variable-string.h>
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/traversals/types.h>
#include <ucsl-reflection/util/memory.h>

namespace ucsl::reflection::algorithms {
	using namespace ucsl::reflection::traversals;

	template<typename GameInterface>
	class Copy {
	public:
		constexpr static size_t arity = 2;
		typedef int result_type;

		template<typename T>
		result_type visit_primitive(T& obj1, T& obj2, const PrimitiveInfo<T>&info) {
			obj1 = obj2;
			return 0;
		}

		result_type visit_primitive(ucsl::strings::VariableString& obj1, ucsl::strings::VariableString& obj2, const PrimitiveInfo<ucsl::strings::VariableString>& info) {
			if (obj1.GetAllocator())
				obj1.Set(obj2.c_str());
			else
				obj1.Set(obj2.c_str(), -1, GameInterface::AllocatorSystem::get_allocator());

			return 0;
		}

		result_type visit_primitive(const char*& obj1, const char*& obj2, const PrimitiveInfo<const char*>& info) {
			assert(false && "copy currently doesn't handle pointers");
			return 0;
		}

		template<typename T, typename O>
		result_type visit_enum(T& obj1, T& obj2, const EnumInfo<O>& info) {
			return visit_primitive(obj1, obj2, PrimitiveInfo<T>{});
		}

		template<typename T, typename O>
		result_type visit_flags(T& obj1, T& obj2, const FlagsInfo<O>& info) {
			return visit_primitive(obj1, obj2, PrimitiveInfo<T>{});
		}

		template<typename F, typename A, typename C, typename D>
		result_type visit_array(A& arr1, A& arr2, const ArrayInfo& info1, const ArrayInfo& info2, C c, D d, F f) {
			arr1.clear();

			for (auto item : arr2) {
				arr1.emplace_back();
				f(arr1[arr1.size() - 1], item);
			}

			return 0;
		}

		template<typename F, typename A, typename C, typename D>
		result_type visit_tarray(A& arr1, A& arr2, const ArrayInfo& info1, const ArrayInfo& info2, C c, D d, F f) {
			arr1.clear();

			for (auto item : arr2) {
				arr1.emplace_back();
				f(arr1[arr1.size() - 1], item);
			}

			return 0;
		}

		template<typename F>
		result_type visit_carray(opaque_obj* obj1, opaque_obj* obj2, const CArrayInfo& info1, const CArrayInfo& info2, F f) {
			assert(info1.size == info2.size && info1.stride == info2.stride && "carray copy currently expects equal size arrays");

			for (size_t i = 0; i < info1.size; i++)
				f(*util::addptr(obj1, i * info1.stride), *util::addptr(obj2, i * info1.stride));
			return 0;
		}

		template<typename F>
		result_type visit_pointer(opaque_obj*& obj1, opaque_obj*& obj2, const PointerInfo& info1, const PointerInfo& info2, F f) {
			assert(false && "copy currently doesn't handle pointers");
			return 0;
		}

		template<typename F>
		result_type visit_union(opaque_obj& obj1, opaque_obj& obj2, const UnionInfo& info1, const UnionInfo& info2, F f) {
			return f(obj1, obj2);
		}

		template<typename F>
		result_type visit_type(opaque_obj& obj1, opaque_obj& obj2, const TypeInfo& info1, const TypeInfo& info2, F f) {
			return f(obj1, obj2);
		}

		template<typename F>
		result_type visit_field(opaque_obj& obj1, opaque_obj& obj2, const FieldInfo& info, F f) {
			return f(obj1, obj2);
		}

		template<typename F>
		result_type visit_base_struct(opaque_obj& obj1, opaque_obj& obj2, const StructureInfo& info, F f) {
			return f(obj1, obj2);
		}

		template<typename F>
		result_type visit_struct(opaque_obj& obj1, opaque_obj& obj2, const StructureInfo& info, F f) {
			return f(obj1, obj2);
		}

		template<typename F>
		result_type visit_root(opaque_obj& obj1, opaque_obj& obj2, const RootInfo& info1, const RootInfo& info2, F f) {
			return f(obj1, obj2);
		}
	};
}
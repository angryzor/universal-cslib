#pragma once

#include <catch2/catch_test_macros.hpp>
#include <ucsl-reflection/accessors/binary.h>
#include <ucsl-reflection/providers/simplerfl.h>
#include <ucsl-reflection/game-interfaces/standalone/game-interface.h>
#include <ucsl-reflection/traversals/fold.h>

using namespace simplerfl;
using namespace std;
using namespace ucsl::reflection;

struct Test1Repr {
	float foo;
	float baz;
	alignas(16) int barCount;
	int* bars;
};

using Bars = dynamic_carray<int, field_resolver<size_t, "barCount">>;

using Test1 = structure<Test1Repr, "Test1", void,
	field<float, "foo">,
	field<float, "baz">,
	field<aligned<16, int>, "barCount">,
	field<Bars*, "bars">
>;

struct Null {
	static constexpr size_t arity = 2;
	struct result_type {
		int v;

		result_type() : v{} {}
		result_type(int v) : v{ v } {}
		result_type(const result_type& other) : v{ other.v } {}
		result_type& operator|=(const result_type& other) { v += other.v; return *this; }
	};
	template<accessors::PrimitiveAccessor T> result_type visit_primitive(T obj1, T obj2) {
		return obj1.visit([&](auto obj1Data) {
			return obj2.visit([&](auto obj2Data) {
				std::cout << "obj1: " << obj1Data << ", obj2: " << obj2Data << std::endl;
				return static_cast<int>(obj1Data + obj2Data);
			});
		});
	}
	//template<typename T> result_type visit_enum(T obj1, T obj2) { return 0; }
	//template<typename T> result_type visit_flags(T obj1, T obj2) { return 0; }
	//template<typename T, typename F> result_type visit_array(T& obj1, T& obj2, F f) { return 0; }
	//template<typename T, typename F> result_type visit_tarray(T& obj1, T& obj2, F f) { return 0; }
	template<accessors::PointerAccessor T, typename F> result_type visit_pointer(T obj1, T obj2, F f) {
		auto obj1Target = obj1.get().value();
		auto obj2Target = obj2.get().value();

		return f(obj1Target, obj2Target);
	}
	template<accessors::CArrayAccessor T, typename F> result_type visit_carray(T obj1, T obj2, F f) {
		result_type res{};
		for (size_t i = 0; i < obj1.get_length(); i++) {
			auto obj1Item = obj1[i];
			auto obj2Item = obj2[i];

			res |= f(obj1Item, obj2Item);

			std::cout << res.v << std::endl;
		}
		return res;
	}
	//template<typename T, typename F> result_type visit_union(T& obj1, T& obj2, F f) { return 0; }
	template<typename T, typename F> result_type visit_type(T obj1, T obj2, F f) { return f(obj1, obj2); }
	template<accessors::ValueAccessor T, typename F> result_type visit_field(T obj1, T obj2, const traversals::FieldInfo& info, F f) { return f(obj1, obj2); }
	template<accessors::StructureAccessor T, typename F> result_type visit_base_struct(T obj1, T obj2, F f) { return f(obj1, obj2); }
	template<accessors::StructureAccessor T, typename F> result_type visit_struct(T obj1, T obj2, F f) { return f(obj1, obj2); }
	template<accessors::ValueAccessor T, typename F> result_type visit_root(T obj1, T obj2, F f) { return f(obj1, obj2); }
};

using GI = game_interfaces::standalone::StandaloneGameInterface;

TEST_CASE( "null traversal", "traversals/fold") {
	int bars[]{ 1, 2, 3, 4, 5 };
	Test1Repr t1{ 4.0f, 5.0f, 5, bars };
	int bars2[]{ 6, 7, 8, 9, 0 };
	Test1Repr t2{ 8.0f, 2.0f, 1, bars2 };
	using Refl = providers::simplerfl<GI>::Type<Test1>;
	accessors::binary<GI>::ValueBinaryAccessor<Refl, Refl, Refl, int, int> acc1{ &t1 };
	accessors::binary<GI>::ValueBinaryAccessor<Refl, Refl, Refl, int, int> acc2{ &t2 };
	traversals::traversal<Null> t{};

	REQUIRE(t.call(Refl{}, acc1, acc2).v == 70);
}
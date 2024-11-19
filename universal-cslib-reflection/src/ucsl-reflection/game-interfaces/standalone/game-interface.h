#pragma once
#include <cstdlib>
#include <iostream>
#include <ucsl/memory/iallocator.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/rfl/rflclass.h>
#include "rflsystem.h"
#include "reflection-db.h"

namespace ucsl::reflection::game_interfaces::standalone {
	template<typename TypeSet>
	struct StandaloneGameInterface {
		class NativeCAllocator : public memory::IAllocator {
		public:
			void* Alloc(size_t size, size_t alignment) override { return _aligned_malloc(size, alignment); }
			void Free(void* ptr) override { _aligned_free(ptr); }
		};

		inline static NativeCAllocator nativeCAllocator{};

		inline static memory::IAllocator* get_fallback_allocator() { return &nativeCAllocator; }
		inline static memory::IAllocator* get_fallback_allocator_for_tarray(void* arr) { return get_fallback_allocator(); }

		template<typename T> struct TArray : public containers::arrays::TArray<T, get_fallback_allocator_for_tarray> {};

		using RflSystem = StandaloneRflSystem<TypeSet>;

		inline static ReflectionDB<RflSystem> reflectionDB;

		class RflClassNameRegistry {
			inline static RflClassNameRegistry* instance{};

		public:
			static void Initialize() {
				instance = new RflClassNameRegistry{};
			}

			static RflClassNameRegistry* GetInstance() {
				return instance;
			}

			typename const RflSystem::RflClass* GetClass(const char* name) {
				return &*reflectionDB.rflClasses[name];
			}
		};

		class RflTypeInfoRegistry {
			inline static RflTypeInfoRegistry* instance{};

		public:
			static void Initialize() {
				instance = new RflTypeInfoRegistry{};
			}

			static RflTypeInfoRegistry* GetInstance() {
				return instance;
			}

			void* ConstructObject(memory::IAllocator* allocator, void* instance, const char* rflClassName) {
				return instance;
			}

			void CleanupLoadedObject(void* instance, const char* rflClassName) {
			}
		};

		static const char* get_spawner_data_class(const char* objectName) {
			return reflectionDB.spawnerDataRflClasses[objectName].c_str();
		}
	};
}

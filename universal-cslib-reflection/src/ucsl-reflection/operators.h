#pragma once
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>

namespace ucsl::reflection {
    static constexpr size_t MOD_ERASED = 0x4552415345443030;
    template<typename Type> struct erased : simplerfl::mod<MOD_ERASED> {
        using type = simplerfl::resolve_decl_t<Type>;
    };

    static constexpr size_t DESCTYPE_CONSTANT = 0x434f4e5354414e54;
    template<typename Repr, Repr value> struct constant : simplerfl::decl<DESCTYPE_CONSTANT> {
        using repr = Repr;
        static constexpr Repr value = value;
    };

    static constexpr size_t DESCTYPE_RFLCLASS = 0x52464c434c415353;
    template<typename Parent> using rflclass_resolver = const char* (*)(const Parent& parent);
    template<typename Parent, rflclass_resolver<Parent> resolver> struct rflclass : simplerfl::decl<DESCTYPE_RFLCLASS> {
        using parent = Parent;
        static constexpr rflclass_resolver<Parent> resolver = resolver;
    };

    static constexpr size_t DESCTYPE_SPAWNER_DATA_RFLCLASS = 0x5350574e44415441;
    template<typename Parent> using game_object_class_resolver = const char* (*)(const Parent& parent);
    template<typename Parent, game_object_class_resolver<Parent> resolver> struct spawner_data_rflclass : simplerfl::decl<DESCTYPE_SPAWNER_DATA_RFLCLASS> {
        using parent = Parent;
        static constexpr rflclass_resolver<Parent> resolver = resolver;
    };

    static constexpr size_t DESCTYPE_COMPONENT_DATA = 0x434f4d5044415441;
    template<typename Parent> using component_data_resolver = const char* (*)(const Parent& parent);
    template<typename Parent, component_data_resolver<Parent> resolver> struct component_data_rflclass : simplerfl::decl<DESCTYPE_COMPONENT_DATA> {
        using parent = Parent;
        static constexpr rflclass_resolver<Parent> resolver = resolver;
    };

    static constexpr size_t DESCTYPE_ARRAY = 0x4152524159303030;
    template<typename Type, typename AllocatorSystem> struct array : simplerfl::decl<DESCTYPE_ARRAY> {
        using type = simplerfl::resolve_decl_t<Type>;
        using allocator_system = AllocatorSystem;
    };

    static constexpr size_t DESCTYPE_TARRAY = 0x5441525241593030;
    template<typename Type, typename AllocatorSystem> struct tarray : simplerfl::decl<DESCTYPE_TARRAY> {
        using type = simplerfl::resolve_decl_t<Type>;
        using allocator_system = AllocatorSystem;
    };

    //template<typename Type> struct is_erased { static constexpr bool value = is_erased<Type::type>::value; };
    //template<typename T> struct is_erased<simplerfl::primitive<T>> { static constexpr bool value = false; };
    //template<typename Repr, Repr value> struct is_erased<constant<Repr, value>> { static constexpr bool value = false; };
    //template<typename Repr, simplerfl::strlit name, typename Underlying, typename... Options> struct is_erased<simplerfl::enumeration<Repr, name, Underlying, Options...>> { static constexpr bool value = false; };
    //template<typename T> struct is_erased<simplerfl::pointer<T>> { static constexpr bool value = false; };
    //template<typename Repr, simplerfl::strlit name, typename Base, typename... Fields> struct is_erased<simplerfl::structure<Repr, name, Base, Fields...>> { static constexpr bool value = false; };
    //template<typename Repr, simplerfl::strlit name, typename Parent, simplerfl::union_resolver<Parent> resolver, typename... Fields> struct is_erased<simplerfl::unionof<Repr, name, Parent, resolver, Fields...>> { static constexpr bool value = false; };
    //template<typename Parent, rflclass_resolver<Parent> resolver> struct is_erased<rflclass<Parent, resolver>> { static constexpr bool value = false; };
    //template<typename Type> struct is_erased<erased<Type>> { static constexpr bool value = true; };
    template<typename Type> static constexpr bool is_erased_v = simplerfl::has_modifier_v<MOD_ERASED, Type>;
}

namespace simplerfl {
    template<typename Repr, Repr value> struct is_realigned<ucsl::reflection::constant<Repr, value>> { static constexpr bool value = false; };
    template<typename Parent, ucsl::reflection::rflclass_resolver<Parent> resolver> struct is_realigned<ucsl::reflection::rflclass<Parent, resolver>> { static constexpr bool value = false; };
    template<typename Parent, ucsl::reflection::game_object_class_resolver<Parent> resolver> struct is_realigned<ucsl::reflection::spawner_data_rflclass<Parent, resolver>> { static constexpr bool value = false; };
    template<typename Parent, ucsl::reflection::component_data_resolver<Parent> resolver> struct is_realigned<ucsl::reflection::component_data_rflclass<Parent, resolver>> { static constexpr bool value = false; };
    template<typename Type, typename AllocatorSystem> struct is_realigned<ucsl::reflection::array<Type, AllocatorSystem>> { static constexpr bool value = false; };
    template<typename Type, typename AllocatorSystem> struct is_realigned<ucsl::reflection::tarray<Type, AllocatorSystem>> { static constexpr bool value = false; };

    template<typename Repr, long long value> struct representation<ucsl::reflection::constant<Repr, value>> { using type = Repr; };
    template<typename Parent, ucsl::reflection::rflclass_resolver<Parent> resolver> struct representation<ucsl::reflection::rflclass<Parent, resolver>> { static_assert("Cannot get representation of dynamic type."); using type = void; };
    template<typename Parent, ucsl::reflection::game_object_class_resolver<Parent> resolver> struct representation<ucsl::reflection::spawner_data_rflclass<Parent, resolver>> { static_assert("Cannot get representation of dynamic type."); using type = void; };
    template<typename Parent, ucsl::reflection::component_data_resolver<Parent> resolver> struct representation<ucsl::reflection::component_data_rflclass<Parent, resolver>> { static_assert("Cannot get representation of dynamic type."); using type = void; };
    template<typename Type, typename AllocatorSystem> struct representation<ucsl::reflection::array<Type, AllocatorSystem>> { using type = ucsl::containers::arrays::Array<typename representation<typename ucsl::reflection::array<Type, AllocatorSystem>::type>::type, AllocatorSystem>; };
    template<typename Type, typename AllocatorSystem> struct representation<ucsl::reflection::tarray<Type, AllocatorSystem>> { using type = ucsl::containers::arrays::TArray<typename representation<typename ucsl::reflection::tarray<Type, AllocatorSystem>::type>::type, AllocatorSystem>; };
}

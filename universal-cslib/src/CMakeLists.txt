target_sources(universal-cslib
    INTERFACE FILE_SET HEADERS FILES
        "ucsl/bitset.h"
        "ucsl/memory/iallocator.h"
        "ucsl/containers/arrays/array-base.h"
        "ucsl/containers/arrays/array.h"
        "ucsl/containers/arrays/tarray.h"
        "ucsl/strings/string-ops.h"
        "ucsl/strings/variable-string.h"
        "ucsl/colors.h"
        "ucsl/magic.h"
        "ucsl/math.h"
        "ucsl/object-id.h"
        "ucsl/rfl/rflclass.h"
        "ucsl/rfl/ranges.h"
        "ucsl/resources/asm/v103.h"
        "ucsl/resources/fxcol/v1.h"
        "ucsl/resources/map/v1.h"
        "ucsl/resources/object-world/v2.h"
        "ucsl/resources/object-world/v3.h"
        "ucsl/resources/rfl/v1.h"
        "ucsl/resources/rfl/v2.h"
        "ucsl/resources/sobj/v1.h"
        "ucsl/resources/swif/v6.h"
        "ucsl/resources/vertex-animation-texture/v1-miller.h"
        "ucsl/resources/vertex-animation-texture/v1-rangers.h"
)

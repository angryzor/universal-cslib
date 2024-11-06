target_sources(universal-cslib
    INTERFACE FILE_SET HEADERS FILES
        "ucsl/memory/iallocator.h"
        "ucsl/containers/arrays/array-base.h"
        "ucsl/containers/arrays/array.h"
        "ucsl/containers/arrays/tarray.h"
        "ucsl/strings/string-ops.h"
        "ucsl/strings/variable-string.h"
        "ucsl/colors.h"
        "ucsl/math.h"
        "ucsl/object-id.h"
)

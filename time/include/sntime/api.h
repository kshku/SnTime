#pragma once

#include <sncore/api_common.h>

#if defined(SN_TIME_STATIC)
    #define SN_TIME_API
#elif defined(SN_EXPORT)
    #define SN_TIME_API SN_API_HELPER_EXPORT
#else
    #define SN_TIME_API SN_API_HELPER_IMPORT
#endif

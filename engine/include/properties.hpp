//
// Created by flori on 3/27/2022.
//

#pragma once

// Define found on internet : https://www.codeproject.com/articles/118921/c-properties

#define PROPERTY(t, n) __declspec( property \
(put = property_set_##n, get = property_get_##n )) t n; \
typedef t property_tmp_type_##n
#define READONLY_PROPERTY(t, n) __declspec( property (get = property_get_##n )) t n; \
typedef t property_tmp_type_##n
#define WRITEONLY_PROPERTY(t, n) __declspec( property (put = property_set_##n )) t n; \
typedef t property_tmp_type_##n
#define GET(n) property_tmp_type_##n property_get_##n() const
#define SET(n) void property_set_##n(const property_tmp_type_##n& value)
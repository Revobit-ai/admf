//
//  4ddat_to_admf.hpp
//  4ddat_to_admf
//
//  Created by yushihang on 2021/3/26.
//

#ifndef _datestring_to_int_hpp
#define _datestring_to_int_hpp

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "stdint.h"

namespace admfExport {
    

    
    typedef struct _bson_error_t {
        uint32_t domain;
        uint32_t code;
        char message[504];
    } bson_error_t;
    
    bool
    _bson_iso8601_date_parse (const char *str,
                              int32_t len,
                              int64_t *out,
                              bson_error_t *error);
}

#endif /* _datestring_to_int_hpp */





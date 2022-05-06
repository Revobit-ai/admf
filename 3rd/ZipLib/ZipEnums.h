//
//  ZipEnums.h
//  ziptest
//
//  Created by yushihang on 2021/3/21.
//

#ifndef ZipEnums_h
#define ZipEnums_h
namespace  zip_helper{
enum class ZIPResult{
    SUCCESS = 0,
    INPUT_FILE_OPEN_FAILED = -1,
    OUTPUT_FILE_OPEN_FAILED = -2,
    NO_ENTRY = -3,
    GET_ENTRY_STREAM_FAILED = -4,
    BUFFER_SIZE_NOT_ENOUGH = -5,
    WRONG_PASSWORD = -6,
    BUFFER_READ_FAILED = -7,
};
}


#endif /* ZipEnums_h */

//
//  exportadmf.h
//  exportadmf
//
//  Created by yushihang on 2021/7/1.
//

#ifndef exportadmf_h
#define exportadmf_h

namespace CHANGE_COLOR{
    struct Result;
}

#ifdef __cplusplus


extern "C" {
#endif
    bool extractAdmf(const char* admfFilePath, const char* dir_);
    //free the return char
    const char* exportChangeColorToJson(const CHANGE_COLOR::Result& result);
#ifdef __cplusplus
}
#endif
#endif /* exportadmf_h */

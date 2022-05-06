//
//  admf_custom.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef ADMF_INCLUDED
#error "DO NOT include the header directly, use #include \"admf_cpp.h\" instead"
#endif

#ifndef admf_custom_h
#define admf_custom_h
#include <string>
#include <unordered_map>
namespace admf
{
    /// 用户自定义数据
    class Custom_
    {
    public:
#ifdef ADMF_EDIT
        virtual std::unordered_map<std::string, std::string>& getValueMap() = 0;
#else
        virtual const std::unordered_map<std::string, std::string>& getValueMap() = 0;
#endif
    };

}
#endif /* admf_custom_h */

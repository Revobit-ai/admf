//
//  admf_pipeline_internal.hpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#ifndef admf_pipeline_internal_hpp
#define admf_pipeline_internal_hpp

#include <unordered_map>
#include "admf_internal.h"
namespace admf_internal {
    class Custom_internal : public admf::Custom_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Custom_internal);

    private:
        std::unordered_map<std::string, std::string> valueMap_;
        
    public:
#ifdef ADMF_EDIT
        std::unordered_map<std::string, std::string>& getValueMap() override {return valueMap_;};
#else
        const std::unordered_map<std::string, std::string>& getValueMap() override {return valueMap_;};
#endif
    
    };
}
#endif /* admf_pipeline_internal_hpp */

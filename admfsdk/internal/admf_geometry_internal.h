//
//  admf_geometry_internal.hpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#ifndef admf_geometry_internal_hpp
#define admf_geometry_internal_hpp

#include "admf_internal.h"
namespace admf_internal {
    class Geometry_internal : public admf::Geometry_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Geometry_internal);

    };
}
#endif /* admf_geometry_internal_hpp */

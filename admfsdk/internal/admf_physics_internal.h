//
//  admf_physics_internal.hpp
//  admfsdk
//
//  Created by yushihang on 2021/3/22.
//

#ifndef admf_physics_internal_hpp
#define admf_physics_internal_hpp

#include "admf_internal.h"
#include <vector>
#include <memory>
namespace admf_internal {
    class String_internal;
    class BinaryData_internal;

    
    class PhysicsComposition_internal : public admf::PhysicsComposition_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(PhysicsComposition_internal);
    public:

        virtual admf::String getName() override; //"Cotton"
        virtual admf::ADMF_UINT getPercentage() override; //万分比的整数值 例如30.1% => 3010
    private:
        std::shared_ptr<String_internal> name_;
        admf::ADMF_UINT percentage_ = 0;
        
#ifdef ADMF_EDIT
    public:
        virtual void setPercentage(admf::ADMF_UINT percentage) override {percentage_ = percentage;};
#endif

    };
    
    class Physics_internal : public admf::Physics_, public Base_internal
    {
        ADMF_INTERNAL_CLASS_CONSTRUCTOR(Physics_internal);
        
    public:

        
        virtual admf::String getDevice() override; //"fab"
        
        virtual admf::BinaryData getBinaryData() override; //content of "/browzwear_physics_example.json"

        virtual admf::Array<admf::PhysicsComposition> getCompositionArray() override;

        
        virtual admf::ADMF_FLOAT getThinkness() override; //0.20203900337219239
    private:
        std::shared_ptr<String_internal> deviceString_;
        admf::ADMF_FLOAT thickness_ = 0;
        std::shared_ptr<BinaryData_internal> binary_; //u3m的文件内容
        std::shared_ptr<Array_internal<admf::PhysicsComposition>> compositionArray_;
        
#ifdef ADMF_EDIT
    public:
        virtual void setThinkness(admf::ADMF_FLOAT thinkness) override {thickness_ = thinkness;};
#endif
        
    };
}
#endif /* admf_physics_internal_hpp */

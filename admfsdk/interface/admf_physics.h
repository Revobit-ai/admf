//
//  admf_physics.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef ADMF_INCLUDED
#error "DO NOT include the header directly, use #include \"admf_cpp.h\" instead"
#endif

#ifndef admf_physics_h
#define admf_physics_h

namespace admf
{

        /// 物理数据构成类
        class PhysicsComposition_
        {
        public:
                ///     获取物理信息名字
                ///    @return    返回物理信息名字
                virtual String getName() = 0;         //"Cotton"
                                                      ///     获取百分率
                                                      ///    @return    返回百分率
                virtual ADMF_UINT getPercentage() = 0; //万分比的整数值 例如30.1% => 3010
#ifdef ADMF_EDIT
        public:
                /// 设置百分率
                /// @param percentage 百分值
                virtual void setPercentage(ADMF_UINT percentage) = 0;
#endif
        };

        ///物理参数类
        class Physics_
        {
        public:
                ///     获取设备
                ///    @return    返回设备
                virtual String getDevice() = 0; //"fab"

                ///     获取物理二进制数据
                ///    @return    返回物理二进制数据
                virtual BinaryData getBinaryData() = 0; //content of "/browzwear_physics_example.json"

                ///     获取物理组成集合
                ///    @return    返回物理组成集合
                virtual Array<PhysicsComposition> getCompositionArray() = 0;

                ///     获取物理厚度
                ///    @return    返回物理厚度
                virtual ADMF_FLOAT getThinkness() = 0; //0.20203900337219239

#ifdef ADMF_EDIT
        public:
                ///     设置物理厚度
                ///     @param thinkness 厚度值
                virtual void setThinkness(ADMF_FLOAT thinkness) = 0;
#endif
        };
}
#endif /* admf_physics_h */

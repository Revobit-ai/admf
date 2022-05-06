//
//  admf_material.h
//  admfsdk
//
//  Created by yushihang on 2021/3/20.
//

#ifndef ADMF_INCLUDED
#error "DO NOT include the header directly, use #include \"admf_cpp.h\" instead"
#endif

#ifndef admf_material_h
#define admf_material_h

namespace admf
{

  /// 材质类
  class Material_
  {
  public:
    ///   获取所有材质层级
    ///    @return    返回所有材质层级
    virtual Array<MaterialLayer> getLayerArray() = 0;
    ///   获取单个材质层级
    ///    @return    返回单个材质层级
    ///    @see MaterialLayer
    virtual MaterialLayer getSideLayer() = 0;
    ///   获取创建材质时间
    ///    @return    返回创建材质时间
    ///    @see ADMFDATE
    virtual ADMF_DATE getCreatedTime() = 0; //"2018-11-08T12:35:37",
                                           ///   获取修改材质时间
    ///    @return    返回修改材质时间
    ///    @see ADMFDATE
    virtual ADMF_DATE getModifiedTime() = 0; //"2020-11-03T07:59:55Z",
    ///获取设备
    virtual MaterialDevice getDevice() = 0;

    ///   获取材质Id
    ///    @return    返回材质Id
    virtual String getId() = 0; //"0001"
                                ///   获取材质名称
    ///    @return    返回材质名称
    virtual String getName() = 0; //"PANTONE 20-0060TPM",
                                  ///   获取meta数据
    ///    @return    返回meta数据
    ///    @see MaterialMetaData
    virtual MaterialMetaData getMetaData() = 0;
      
      
      
#ifdef ADMF_EDIT
    ///设置创建时间
    virtual void setCreatedTime(ADMF_DATE date) = 0;  //"2018-11-08T12:35:37",
                                                     ///设置修改时间
    virtual void setModifiedTime(ADMF_DATE date) = 0; //"2020-11-03T07:59:55Z",
#endif
      
  
  };

  /// 材质采集设备类
  class MaterialDevice_
  {
  public:
    ///   获取材质采集类型
    ///    @return    返回材质采集类型
    virtual String getType() = 0;         //"material_scan",
                                          ///   获取材质采集设备
                                          ///    @return    返回材质采集设备
    virtual String getModel() = 0;        //"Idemera",
                                          ///   获取材质采集设备版本号
                                          ///    @return    返回材质采集设备版本号
    virtual String getRevision() = 0;     //"1.0",
                                          ///   获取材质采集设备序列号
                                          ///    @return    返回材质采集设备序列号
    virtual String getSerialNumber() = 0; //"1000"
  };

  /// 原材质数据类
  class MaterialMetaData_
  {
  public:
    ///   获取二进制数据
    ///    @return    返回二进制数据
    ///    @see BinaryData
    virtual BinaryData getSource() = 0; // source material file
                                        ///获取类型
    virtual String getType() = 0;       //"4ddat",
      
    virtual String getVersion() = 0;    //"1.0"
  };
}
#endif /* admf_material_h */

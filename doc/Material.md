# Material

代码示例

```cpp
admf::ADMF_RESULT result;
admf::ADMF::ADMFJsons admfJsons;
auto admf = admf::ADMF::loadFromFile(admfFilePath, result, &admfJsons);
//admfJsons.material 为如下的json文本内容

auto material = admf->getMaterial();
//material为如下内容对应的c++对象
```

Material 格式 <font color="gray">(部分字段有注释)</font>

```jsonc
{
  //layers为多层材质数组,每个元素为一层材质
  "layers": [
    {
      "type": "Fabric",
      "shader": "", //shader model
      "preview": "150_preview.png",
      "basic": {
        "emissive": {
          "texture": {
            "path": "237_Emissive.png",
            "colorSpace": "",
            "dpi": {
              "x": 0.0,
              "y": 0.0
            },
            "width": 0.0,
            "height": 0.0,
            "physicalWidth": 0.0,
            "physicalHeight": 0.0,
            "channels": 0,
            "elementSize": 0
          },
          "value": 0.0,
          "color": {
            "r": 0.039215687662363052368,
            "g": 0.039215687662363052368,
            "b": 0.039215687662363052368,
            "colorSpace": ""
          }
        },
        "basecolor": {
          //diffuse
          "texture": {
            "path": "156_Base.png", //admf zip包里的文件名
            "colorSpace": "srgb",
            "dpi": { //dpi以此为准， 忽略图片格式本身的dpi
              "x": 156.0576019287109375,
              "y": 156.0576019287109375
            },
            "width": 2048.0, 】
            //贴图像素，如果path里的文件内容（156_Base.png）可以通过文件头识别为png/jpg等格式，以图片里保存的为准，忽略这里的数据
            "height": 2048.0,
            "physicalWidth": 333.333343505859375, //单位为mm
            "physicalHeight": 333.333343505859375,
            "channels": 3, //同图片像素字段，如果图片里包含了channel信息， 以图片为准
            "elementSize": 1 //同图片像素字段，如果图片里包含了channel信息， 以图片为准
          },
          //color:色卡信息
          "color": {
            /*
            * 注意: 在存在多个色卡定义时， 例如同时存在单色或者多色色卡， 或者单色/多色色卡数组里有组元素时
            * 默认显示的改色信息，应该由type+index共同决定，
            * 如type+index对应的色卡数组下标不存在，按照下列顺序进行匹配
            * 1. 对应type(solid/multi)的第一个色卡,即认为index=0
            * 2. solid类型的第一个色卡 即 type = "solid", index=0
            * 3. 原色色卡，即认为默认色卡为type = "solid"且isOriginal为1
            */
            "type": "solid", //当前色卡类型，solid为纯色色卡，multi为多色改色色卡(非multi的都认为是solid)
            "index": 0, //和type一起生效，下面solid或者multi数组里的第index个色卡为当前色卡(业务上未指定色卡时，使用此色卡进行渲染)
            "solid": {
              //纯色色卡数组，本例子里有两个色卡，第一个为原色色卡，第二个为红色
              "block": [
                {
                  "name": "",
                  "type": "",
                  "value": "",
                  "isOriginal": 1 //该值为不为0时, type/value被忽略, 表示不叠加改色信息
                },
                {
                  "name": "(255,0,0)",
                  "type": "RGB",
                  "value": "255,0,0", //红色色卡
                  "isOriginal": 0
                }
              ]
            },
            "multi": {
              //多色色卡(改色)
              "block": [
                //block里的每一个元素都是一个多色改色方案
                {
                  "name": "这是多色改色方案名字", //改色方案名字
                  "mask": [
                    //mask数组里每一个元素是一个多色改色区域+颜色的组合
                    //多个组合一起叠加形成一个改色方案，实现对原材质不同区域的改色
                    {
                      "texture": {
                        //此处为一张纹理
                        "path": "binary_273", //标识改色区域(灰度图)
                        "colorSpace": "",
                        "dpi": {
                          "x": 0.0,
                          "y": 0.0
                        },
                        "width": 0.0,
                        "height": 0.0,
                        "physicalWidth": 0.0,
                        "physicalHeight": 0.0,
                        "channels": 0,
                        "elementSize": 0
                      },
                      "value": "255,255,0" //改色的颜色信息
                    },
                    {
                      "texture": {
                        "path": "binary_281",
                        "colorSpace": "",
                        "dpi": {
                          "x": 0.0,
                          "y": 0.0
                        },
                        "width": 0.0,
                        "height": 0.0,
                        "physicalWidth": 0.0,
                        "physicalHeight": 0.0,
                        "channels": 0,
                        "elementSize": 0
                      },
                      "value": "0,255,0"
                    }
                  ]
                }
              ]
            }
          }
        },
        "normal": {
          "texture": {
            "path": "171_Normal.png",
            "colorSpace": "linear",
            "dpi": {
              "x": 156.0576019287109375,
              "y": 156.0576019287109375
            },
            "width": 2048.0,
            "height": 2048.0,
            "physicalWidth": 333.333343505859375,
            "physicalHeight": 333.333343505859375,
            "channels": 3,
            "elementSize": 1
          },
          "value": 1.2000000476837158203
        },
        "alpha": {
          "texture": {
            "path": "179_Alpha.png",
            "colorSpace": "",
            "dpi": {
              "x": 0.0,
              "y": 0.0
            },
            "width": 0.0,
            "height": 0.0,
            "physicalWidth": 0.0,
            "physicalHeight": 0.0,
            "channels": 0,
            "elementSize": 0
          },
          "value": 1.0
        },
        "metalness": {
          "texture": {
            "path": "187_Metalness.png",
            "colorSpace": "",
            "dpi": {
              "x": 0.0,
              "y": 0.0
            },
            "width": 0.0,
            "height": 0.0,
            "physicalWidth": 0.0,
            "physicalHeight": 0.0,
            "channels": 0,
            "elementSize": 0
          },
          "value": 0.0
        },
        "roughness": {
          "texture": {
            "path": "195_Roughness.png",
            "colorSpace": "",
            "dpi": {
              "x": 0.0,
              "y": 0.0
            },
            "width": 0.0,
            "height": 0.0,
            "physicalWidth": 0.0,
            "physicalHeight": 0.0,
            "channels": 0,
            "elementSize": 0
          },
          "value": 0.0
        },
        "specular": {
          "texture": {
            "path": "203_Specular.png",
            "colorSpace": "srgb",
            "dpi": {
              "x": 156.0576019287109375,
              "y": 156.0576019287109375
            },
            "width": 2048.0,
            "height": 2048.0,
            "physicalWidth": 333.333343505859375,
            "physicalHeight": 333.333343505859375,
            "channels": 3,
            "elementSize": 1
          },
          "color": {
            "r": 0.039215687662363052368,
            "g": 0.039215687662363052368,
            "b": 0.039215687662363052368,
            "colorSpace": ""
          }
        },
        "glossiness": {
          "texture": {
            "path": "213_Glossiness.png",
            "colorSpace": "linear",
            "dpi": {
              "x": 156.0576019287109375,
              "y": 156.0576019287109375
            },
            "width": 2048.0,
            "height": 2048.0,
            "physicalWidth": 333.333343505859375,
            "physicalHeight": 333.333343505859375,
            "channels": 3,
            "elementSize": 1
          },
          "value": 1.0
        },
        "anisotropy": {
          "texture": {
            "path": "221_Anisotropy.png",
            "colorSpace": "",
            "dpi": {
              "x": 0.0,
              "y": 0.0
            },
            "width": 0.0,
            "height": 0.0,
            "physicalWidth": 0.0,
            "physicalHeight": 0.0,
            "channels": 0,
            "elementSize": 0
          },
          "value": 0.0
        },
        "anisotropyRotation": {
          "texture": {
            "path": "229_AnisotropyRotation.png",
            "colorSpace": "",
            "dpi": {
              "x": 0.0,
              "y": 0.0
            },
            "width": 0.0,
            "height": 0.0,
            "physicalWidth": 0.0,
            "physicalHeight": 0.0,
            "channels": 0,
            "elementSize": 0
          },
          "value": 0.0
        },
        "ambientOcclusion": {
          "texture": {
            "path": "247_AO.png",
            "colorSpace": "",
            "dpi": {
              "x": 0.0,
              "y": 0.0
            },
            "width": 0.0,
            "height": 0.0,
            "physicalWidth": 0.0,
            "physicalHeight": 0.0,
            "channels": 0,
            "elementSize": 0
          }
        },
        "height": {
          //heightmap
          "texture": {
            "path": "255_Height.png",
            "colorSpace": "",
            "dpi": {
              "x": 0.0,
              "y": 0.0
            },
            "width": 0.0,
            "height": 0.0,
            "physicalWidth": 0.0,
            "physicalHeight": 0.0,
            "channels": 0,
            "elementSize": 0
          },
          "value": 1.0,
          "level": 1.0
        },
        "clearCoatNormal": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            }
        },
        "clearCoatRoughness": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "value": 0.0
        },
        "clearCoatValue": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "value": 0.0
        },
        "sheenTint": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "value": 0.0
        },
        "sheenValue": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "value": 0.0
        },
        "specularTint": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "value": 0.0
        },
        "subSurfaceColor": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "color": {
                "r": 0.0,
                "g": 0.0,
                "b": 0.0,
                "colorSpace": ""
            }
        },
        "subSurfaceRadius": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "value": 0.0
        },
        "subSurfaceValue": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "value": 0.0
        },
        "transmission": {
            "texture": {
                "path": "",
                "colorSpace": "",
                "dpi": {
                    "x": 0.0,
                    "y": 0.0
                },
                "width": 0.0,
                "height": 0.0,
                "physicalWidth": 0.0,
                "physicalHeight": 0.0,
                "channels": 0,
                "elementSize": 0
            },
            "value": 0.0
          },

        "transform": {
          "wrapping": "",
          "offset": {
            "x": 0.0,
            "y": 0.0
          },
          "scale": { // 1.0/physicalsize
            "x": 0.0030000000260770320892,
            "y": 0.0030000000260770320892
          },
          "rotation": 0.0
        }
      },
      "spec": {
        "refraction": {
          "color": {  //对应4ddat的 refractionColor
            "r": 0.0,
            "g": 0.0,
            "b": 0.0,
            "colorSpace": ""
          },
          "glossiness": 1.0, //对应4ddat的 refractionGlossiness
          "texture": {   //对应u3m的IOR
              "path": "",
              "colorSpace": "",
              "dpi": {
                  "x": 0.0,
                  "y": 0.0
              },
              "width": 0.0,
              "height": 0.0,
              "physicalWidth": 0.0,
              "physicalHeight": 0.0,
              "channels": 0,
              "elementSize": 0
          },
          "value": 1.3999999761581420898  //对应u3m的IOR
        }
      },
      "enabled": 1 //layer是否启用
    }
  ],
  "side": {
    //侧面材质信息，与layers数组里的单个元素格式一致
    ...
  },
  "device": {
    //设备信息(例如扫描仪)
    "type": "",
    "model": "",
    "revision": "",
    "serialNumber": ""
  },
  "created": {
    //创建时间
    "$date": "2021-10-13T08:10:18.592Z"
  },
  "modified": {
    //修改时间
    "$date": "2021-10-13T08:10:18.592Z"
  },
  "id": "",
  "name": "",
  "metadata": {
    //从其他格式转换过来时, 原格式的相关信息
    "source": "135_PANTONE 20-0056TPM.4Ddat",
    "type": "4ddat",
    "version": "1.9.0"
  }
}
```

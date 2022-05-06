# Version

代码示例

```cpp
admf::ADMF_RESULT result;
admf::ADMF::ADMFJsons admfJsons;
auto admf = admf::ADMF::loadFromFile(admfFilePath, result, &admfJsons);

auto sdkVersion = admf->getSDKVersion();
auto schema = admf->getSchema();

```

Version

```jsonc
{
  "sdkVersion": "1.1",
  //sdkVersion主要用于标识SDK版本的修改，例如增加了字段定义(例如增加了贴图)等，当前值为1.1

  "schema": "1.0"
  //schema字段用于标识ADMF的格式本身是否有修改(例如增加了新的信息存储格式或者修改了bson格式，简单的增加字段不会涉及这个值的改动)，当前值为1.0
}
```

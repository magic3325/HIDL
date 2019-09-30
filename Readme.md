# HIDL
 添加vendor下的hidl
hidl-gen工具介绍

系统定义的所有的.hal接口，都是通过hidl-gen工具转换成对应的代码。hidl-gen源码路径：system/tools/hidl，是在ubuntu上可执行的二进制文件。
路径　out/host/linux-x86/bin/hidl-gen
使用方法：hidl-gen -o output-path -L language (-r interface-root) fqname

在vendor/tinno/interfaces/目录下新建test/1.0目录，并在1.0目录中创建接口ITest.hal。目录结构如下：

.
└── test
    └── 1.0
        └── ITest.hal
ITest.hal文件里面只有一个方法helloWorld(string name)，具体实现如下：

```
package vendor.tinno.hardware.test@1.0;

interface ITest{
   helloWorld(string name) generates (string result);
};

```

2、执行下面三条命令会自动生成对应的c++文件；
```
 PACKAGE=vendor.tinno.hardware.test@1.0
 LOC=vendor/tinno/interfaces/test/1.0/default/
 out/host/linux-x86/bin/hidl-gen -o $LOC -Lc++-impl -r vendor.tinno.hardware.test:vendor/tinno/interfaces/test -r android.hidl:system/libhidl/transport $PACKAGE
```

执行命令后的目录结构如下：

```
└── 1.0
    ├── default
    │   ├── Test.cpp
    │   └── Test.h
    └── ITest.hal
```
default 是新生成的目录，Test.cpp和Test.h是新生成的两个文件，打开Gunder.h文件，去掉
//  extern "C" ITest* HIDL_FETCH_ITest(const char* name);
前面的注释，使用直通式HAL（Passthrough 模式）来通信。
Test.cpp文件也要进行对应的修改，
```
ITest* HIDL_FETCH_ITest(const char* /* name */) {
    return new Test();
}
```

3、执行下面命令

  out/host/linux-x86/bin/hidl-gen -o $LOC -Landroidbp-impl -r vendor.tinno.hardware.test:vendor/tinno/interfaces/test -r android.hidl:system/libhidl/transport $PACKAGE
  会在vendor/tinno/interfaces/test$ 1.0/default/目录生成Android.bp文件。
  out/host/linux-x86/bin/hidl-gen -Landroidbp -r vendor.tinno.hardware.test:vendor/tinno/interfaces/test -r android.hidl:system/libhidl/transport $PACKAGE

    会在vendor/tinno/interfaces/test$ 1.0/目录生成Android.bp文件
```
.
└── 1.0
    ├── Android.bp
    ├── default
    │   ├── Android.bp
    │   ├── Test.cpp
    │   └── Test.h
    └── ITest.hal
```


4、在vendor/tinno/interfaces/test/1.0/default目录下新建
service.cpp
android.hardware.gunder@1.0-service.rc，
其中vendor.tinno.hardware.test@1.0-service.rc是程序的入口函数。
```
service.cpp内容

#define LOG_TAG "vendor.tinno.hardware.test@1.0-service"
#include <hidl/LegacySupport.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>
#include "Test.h"

using android::hardware::defaultPassthroughServiceImplementation;
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;
using ::vendor::tinno::hardware::test::V1_0::ITest;
using ::vendor::tinno::hardware::test::V1_0::implementation::Test;
using ::android::sp;

int main() {
     return defaultPassthroughServiceImplementation<ITest>();
}

vendor.tinno.hardware.test@1.0-service.rc　内容
service test_server-1-0 /vendor/bin/hw/vendor.tinno.hardware.test@1.0-service
    class hal
    user system
    group system
```

5.打开vendor/tinno/interfaces/test/1.0/default目录下的Android.bp，添加编译service.cpp成为可执行文件的代码。具体添加内容如下：

```
cc_binary {
    name: "vendor.tinno.hardware.test@1.0-service",
    defaults: ["hidl_defaults"],
    relative_install_path: "hw",
    vendor: true,
    init_rc: ["vendor.tinno.hardware.test@1.0-service.rc"],
    srcs: ["service.cpp",
            "Test.cpp",
    ],

    shared_libs: [
        "liblog",
        "libcutils",
        "libdl",
        "libbase",
        "libutils",
        "libhardware",
        "libhidlbase",
        "libhidltransport",
        "vendor.tinno.hardware.test@1.0",
    ],
}
```
打开vendor/tinno/interfaces/test/1.0/目录下的Android.bp，添加编译
```
hidl_package_root {
    name: "vendor.tinno.hardware.test",
    path: "vendor/tinno/interfaces/test",
}

```

mmm vendor/tinno/interfaces/test/1.0/
执行后会生成下面的文件：
out/target/product/c330ae/vendor/lib64/vendor.tinno.hardware.test@1.0.so
out/target/product/c330ae/vendor/lib64/vendor.tinno.hardware.test@1.0-adapter-helper.so
out/target/product/c330ae/vendor/lib/hw/vendor.tinno.hardware.test@1.0-impl.so
out/target/product/c330ae/vendor/bin/hw/vendor.tinno.hardware.test@1.0-service
out/target/product/c330ae/vendor/etc/init/vendor.tinno.hardware.test@1.0-service.rc

6、在manifest文件里添加vendor接口的定义编译device/fsl/mek_8q/manifest.xml文件（不同厂家路径可能不一样），添加以下内容，不然在client端是没法拿到service的。如下：
```
<manifest version="1.0" type="device" target-level="3">
<hal format="hidl">
    <name>vendor.tinno.hardware.test</name>
    <transport>hwbinder</transport>
    <version>1.0</version>
    <interface>
        <name>ITest</name>
        <instance>default</instance>
    </interface>
</hal>
</manifest>
```
7、使用C++实现客户端调用

在vendor/tinno/interfaces/test/1.0/目录下新建test目录，并且在test目录下新建Android.bp跟testtest.cpp文件，这两个文件的内容如下：
```
cc_binary {
    relative_install_path: "hw",
    defaults: ["hidl_defaults"],
    name: "test_test",
    proprietary: true,
    srcs: ["testtest.cpp"],

    shared_libs: [
        "liblog",
        "libhardware",
        "libhidlbase",
        "libhidltransport",
        "libutils",
        "vendor.tinno.hardware.test@1.0",
    ],

}
```
```
#include <vendor/tinno/hardware/test/1.0/ITest.h>
#include <hidl/Status.h>
#include <hidl/LegacySupport.h>
#include <utils/misc.h>
#include <hidl/HidlSupport.h>
#include <stdio.h>

using ::android::hardware::hidl_string;
using ::android::sp;
using vendor::tinno::hardware::test::V1_0::ITest;

int main(){
   android::sp<ITest> service =  ITest::getService();
   if (service == nullptr){
        printf("Failed to get service");
        return -1;
   }
   service->helloWorld("helloworld",[&](hidl_string result){
     printf("%s\n",result.c_str());
   });

   return 0;

}
```

到此代码部分已经可以了，接下来就要添加权限，

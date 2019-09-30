#include "Test.h"

namespace vendor {
namespace tinno {
namespace hardware {
namespace test {
namespace V1_0 {
namespace implementation {

  Test::Test() {

      const hw_module_t *hw_module = nullptr;
      ALOGD("%s() :%d  enter.\n", __func__, __LINE__);
      int ret = hw_get_module(TEST_HARDWARE_MODULE_ID, &hw_module);
      if (ret == 0) {
          ret = test_open(hw_module, &mDevice);
          if (ret != 0) {
              ALOGE("faceunlock_open failed: %d", ret);
          }
      }
      else {
          ALOGE("hw_get_module %s failed: %d", TEST_HARDWARE_MODULE_ID, ret);
      }
  }


  Test::~Test() {

  }


// Methods from ::vendor::tinno::hardware::test::V1_0::ITest follow.
Return<void> Test::helloWorld(const hidl_string& name, helloWorld_cb _hidl_cb) {
    // TODO implement
    char buf[100];
    ::memset(buf, 0x00, 100);
    ::snprintf(buf, 100, "Hello World, %s", name.c_str());
    hidl_string result(buf);
    _hidl_cb(result);
    return Void();

}


// Methods from ::android::hidl::base::V1_0::IBase follow.

Test::Test(test_device_t *device) : mDevice(device) {}

//
//
// ITest* HIDL_FETCH_ITest(const char* /* name */) {
//     return new Test();
//
//     test_device_t *test_device;
//     const hw_module_t *hw_module = nullptr;
//
//     int ret = hw_get_module(TEST_HARDWARE_MODULE_ID, &hw_module);
//     if (ret == 0) {
//         ret = test_open(hw_module, &test_device);
//         if (ret != 0) {
//             ALOGE("test_open failed: %d", ret);
//         }
//     } else {
//         ALOGE("hw_get_module %s failed: %d", TEST_HARDWARE_MODULE_ID, ret);
//     }
//
//     if (ret == 0) {
//         return new Test(test_device);
//     } else {
//         ALOGE("Passthrough failed to open legacy HAL.");
//         return nullptr;
//     }
//   return nullptr;
//
//
// }





//
}  // namespace implementation
}  // namespace V1_0
}  // namespace test
}  // namespace hardware
}  // namespace tinno
}  // namespace vendor

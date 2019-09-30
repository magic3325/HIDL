#ifndef VENDOR_TINNO_HARDWARE_TEST_V1_0_TEST_H
#define VENDOR_TINNO_HARDWARE_TEST_V1_0_TEST_H

#include <vendor/tinno/hardware/test/1.0/ITest.h>
#include <hidl/MQDescriptor.h>
//#include <hidl/Status.h>
#include "test_interface.h"

namespace vendor {
namespace tinno {
namespace hardware {
namespace test {
namespace V1_0 {
namespace implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Test : public ITest {
    // Methods from ::vendor::tinno::hardware::test::V1_0::ITest follow.
    //Return<void> helloWorld(const hidl_string& name, helloWorld_cb _hidl_cb) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.
  public:
      Test();
      ~Test();
    Test(test_device_t *device);
    // Return<Status> on(uint32_t timeoutMs)  override;
    // Return<Status> off()  override;
    Return<void> helloWorld(const hidl_string& name, helloWorld_cb _hidl_cb) override;

  private:
    test_device_t    *mDevice;

};

// FIXME: most likely delete, this is only for passthrough implementations
 //extern "C" ITest* HIDL_FETCH_ITest(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace test
}  // namespace hardware
}  // namespace tinno
}  // namespace vendor

#endif  // VENDOR_TINNO_HARDWARE_TEST_V1_0_TEST_H

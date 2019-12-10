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

   #if 0
     return defaultPassthroughServiceImplementation<ITest>();
   #else
       sp <ITest> service = new Test();
       configureRpcThreadpool(1, true /*callerWillJoin*/);
       if(android::OK !=  service->registerAsService()){
         ALOGE("%s() failed.\n",  __func__);
         return -1;
       }
       joinRpcThreadpool();
         return 0;
  #endif

}

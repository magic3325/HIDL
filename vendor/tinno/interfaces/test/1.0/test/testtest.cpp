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

 #!/bin/bash

mydir=$(dirname $0 | sed -e 's/^\.\///')
default=$mydir/1.0/default

ROOT="vendor/tinno/interfaces/test"
OEMFMHIDL_COMPONENT="vendor.tinno.hardware.test:$ROOT"
OEMFM_INTERFACE_LIB="vendor.tinno.hardware.test@1.0"
LIBHIDL_COMPONENT="android.hidl:system/libhidl/transport"

hidl_gen=$ANDROID_BUILD_TOP/out/host/linux-x86/bin/hidl-gen

$hidl_gen -Landroidbp -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB
$hidl_gen -o "$ROOT"  -Ljava -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB
$hidl_gen -o "$default"  -Lc++-impl -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB
$hidl_gen -o "$default"  -Landroidbp-impl -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB
$hidl_gen -o "$default"  -Lhash -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB >$mydir/current.txt

#$hidl_gen -o "$default"  -Lc++-headers -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB
#$hidl_gen -o "$default"  -Lc++-sources -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB
#$hidl_gen -o "$default"  -Lvts -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB
#$hidl_gen -o "$default"  -Ljava -r $OEMFMHIDL_COMPONENT -r $LIBHIDL_COMPONENT $OEMFM_INTERFACE_LIB

# 修正编译报错问题.
echo 'hidl_package_root {
    name: "vendor.tinno.hardware.test",
    path: "vendor/tinno/interfaces/test",
}' >>$mydir/1.0/Android.bp


echo "hidl-gen ok.->$mydir"

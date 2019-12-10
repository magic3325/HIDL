#/bin/bash
echo "service start!"
adb root
adb remount
adb push out/target/product/c330ae/vendor/lib/vendor.tinno.hardware.test@1.0.so vendor/lib/
adb push out/target/product/c330ae/vendor/lib/vendor.tinno.hardware.test@1.0-adapter-helper.so vendor/lib/

adb push out/target/product/c330ae/vendor/lib64/vendor.tinno.hardware.test@1.0.so vendor/lib64/
adb push out/target/product/c330ae/vendor/lib64/vendor.tinno.hardware.test@1.0-adapter-helper.so vendor/lib64/


adb push out/target/product/c330ae/vendor/bin/hw/vendor.tinno.hardware.test@1.0-service vendor/bin/hw/

adb push out/target/product/c330ae/vendor/lib/hw/vendor.tinno.hardware.test@1.0-impl.so vendor/lib/hw/
adb push out/target/product/c330ae/vendor/lib64/hw/vendor.tinno.hardware.test@1.0-impl.so vendor/lib64/hw/

sleep 3s
adb reboot
echo "service end!"

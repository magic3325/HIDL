
Memory Leak :

1: malloc Leak;

Heap memory Allocate Way:
malloc \ realloc ---- free
new \ new[]      ---- delete \ delete[]
GlobalAlloc      ---- GlobalFree

数组对象的内存泄露：
```
MyPointEx *pointArray = new MyPointEx[100];
删除形为：
delete [] pointArray;

```


```
int main(){
    memObj *array;
    array = new [10] memObj();
    delete array;

}

Modify:
int main(){
    memObj *array;
    array = new [10] memObj();
    delete[] array; //modify delete array;

}
```
2: Fd Leak
```
 int file_operations(){
    char path[32];
    char line[32];
    int fd;
    snprintf(path, sizeof(path),"/proc/self/status");
    fd = open(path , sizeof(patch),O_RDONLY);
    if(fd>0){
      read(fd,line,sizeof(line));
      line[31]='\0';
      if(strstr(&(line[6]),"ABenchMark")!=null){
        close(fd);
        return -1;
      }
    }
 }

 int file_operations(){
    char path[32];
    char line[32];
    int fd;
    snprintf(path, sizeof(path),"/proc/self/status");
    fd = open(path , sizeof(patch),O_RDONLY);
    if(fd>0){
      read(fd,line,sizeof(line));
      line[31]='\0';
      if(strstr(&(line[6]),"ABenchMark")!=null){
        close(fd);
        return -1;
      }
        close(fd);  //modify add close fd;
    }
 }
//只有陪陪的才会close(fd),不匹配的没有close(fd),存在fd 泄露，
//需要考虑不同情况下对FD进行关闭，防止覆盖不全的情况。

```

Analysis method

step1: 先检查ION确认ION Memory Usage 是否合理；
setp2:  再查malloc,DB中的PROCESS_MAPS看看有没有在VA大量分配malloc 的情况；

1.malloc Analysis
一般使用monkey进行压测获取对应的Process的dumpsys meminfo 信息，
看是否有持续增长无法回落的情况来判定是否有内存泄露。

2.dumpsys meminfo;

```
Applications Memory Usage (in Kilobytes):
Uptime: 149866597 Realtime: 149866597

Total PSS by process:
    113,249K: system (pid 1086)
     79,221K: camerahalserver (pid 721)
     70,367K: com.google.android.gms (pid 1687)
     69,463K: com.google.android.gms.persistent (pid 1308)
     66,615K: com.android.systemui (pid 1267)
     56,430K: com.google.android.googlequicksearchbox:search (pid 2469)
     52,660K: com.google.android.inputmethod.latin (pid 1570)
     41,979K: com.google.android.apps.messaging (pid 13734)
     39,480K: surfaceflinger (pid 571)

```
一般而言，Native heap 是衡量malloc泄露的主要方式，目前客户一般关注的是total的数据，
这个就同时包含ION申请的memory 信息.

3.procrank & showmap

我们还可以使用procrank 和showmap来定位泄露点。

```
android@c0469:~$ adb shell procrank -u
  PID       Vss      Rss      Pss      Uss     Swap    PSwap    USwap    ZSwap  cmdline
 1086  2163492K  184904K   59021K   47296K   46168K   22469K    6556K    6636K  system_server
 1570  1398360K  147044K   52940K   46872K       4K       0K       0K       0K  com.google.android.inputmethod.latin
13734  1385848K  120088K   30738K   26216K   11532K    4796K     496K    1416K  com.google.android.apps.messaging
14316  1364664K  112692K   26210K   21896K    6268K    2593K     152K     766K  com.google.android.apps.tachyon
  839   117044K   22380K   18981K   18912K    1132K     806K     548K     238K  /system/bin/emdlogger1
 2320  1929648K  135500K   34537K   15016K       4K       0K       0K       0K  com.google.android.googlequicksearchbox:interactor
 2469  1974716K  147136K   33478K   11924K   23240K    9385K     864K    2772K  com.google.android.googlequicksearchbox:search
14574  1320396K  104368K   16531K   11200K    1616K     541K      12K     159K  com.google.android.contacts
 1388  1396184K  110512K   18229K   11104K    4304K    2043K     368K     603K  com.android.phone
14132  1332156K   86088K   12723K    9364K    5424K    2113K      64K     624K  com.google.android.apps.wellbeing
14539  1307256K   86716K   12313K    9244K    2288K     904K      36K     267K  android.process.acore
 3505  1337268K   93616K   12134K    8928K       4K       0K       0K       0K  com.debug.loggerui
14054  1321804K  100584K   11976K    7772K    4876K    2026K      80K     598K  com.google.android.apps.turbo:aab
 1267  1449744K  134192K   16738K    7168K   28732K   13131K    2316K    3878K  com.android.systemui

```

```
W-V830:/ # showmap 1267
 virtual                     shared   shared  private  private
    size      RSS      PSS    clean    dirty    clean    dirty     swap  swapPSS   #  object
-------- -------- -------- -------- -------- -------- -------- -------- -------- ---- ------------------------------
     428      280        9      280        0        0        0        0        0    2 /apex/com.android.conscrypt/javalib/conscrypt.jar
     556       24        0        0       24        0        0        0        0    4 /apex/com.android.conscrypt/lib/libc++.so
     856       44        1        0       44        0        0        0        0    4 /apex/com.android.conscrypt/lib/libcrypto.so
     204        8        0        0        8        0        0        0        0    4 /apex/com.android.conscrypt/lib/libjavacrypto.so
     224       12        0        0       12        0        0        0        0    4 /apex/com.android.conscrypt/lib/libssl.so
      64       60        1       60        0        0        0        0        0    2 /apex/com.android.media/javalib/updatable-media.jar
     832      556        7      536       16        0        4        0        0    4 /apex/com.android.runtime/bin/linker
   47496     1008      113     1008        0        0        0        0        0    2 /apex/com.android.runtime/etc/icu/icudt63l.dat
    1188        0        0        0        0        0        0        0        0    2 /apex/com.android.runtime/javalib/apache-xml.jar
    1372      208        8      208        0        0        0        0        0    2 /apex/com.android.runtime/javalib/bouncycastle.jar
    3232     1804      131     1804        0        0        0        0        0    2 /apex/com.android.runtime/javalib/core-libart.jar
    4844     3056      184     3056        0        0        0        0        0    2 /apex/com.android.runtime/javalib/core-oj.jar
     404      128        4      128        0        0        0        0        0    2 /apex/com.android.runtime/javalib/okhttp.jar
     884      880       14      856       16        0        8        0        0    4 /apex/com.android.runtime/lib/bionic/libc.so
      12       12        0        8        4        0        0        0        0    3 /apex/com.android.runtime/lib/bionic/libdl.so
     136      136        1      128        8        0        0        0        0    4 /apex/com.android.runtime/lib/bionic/libm.so
      48       48        0       40        8        0        0        0        0    4 /apex/com.android.runtime/lib/libadbconnection.so
     140      108        3       96       12        0        0        0        0    4 /apex/com.android.runtime/lib/libandroidicu.so

```

我们一般通过PSS和object 的数量信息来判断那个块发生了泄露。

PSS & USS
Android内存主要有四种形式：
一般来说内存占用大小有如下规律：
VSS>=RSS>=PSS>=USS

VSS - Virtual Set Size 虚拟耗用内存（包含共享库占用的内存）
RSS - Resident Set Size 实际使用物理内存（包含共享库占用的内存）
PSS - Proportional Set Size 实际使用的物理内存（比例分配共享库占用的内存）
USS - Unique Set Size 进程独自占用的物理内存（不包含共享库占用的内存）


VSS - Virtual Set Size （用处不大）
虚拟耗用内存（包含共享库占用的全部内存，以及分配但未使用内存）。其大小还包括了可能不在RAM中的内存（比如虽然malloc分配了空间，但尚未写入）。VSS 很少被用于判断一个进程的真实内存使用量。

RSS - Resident Set Size （用处不大）
实际使用物理内存（包含共享库占用的全部内存）。但是RSS还是可能会造成误导，因为它仅仅表示该进程所使用的所有共享库的大小，它不管有多少个进程使用该共享库，该共享库仅被加载到内存一次。所以RSS并不能准确反映单进程的内存占用情况

PSS - Proportional Set Size （仅供参考）
实际使用的物理内存（比例分配共享库占用的内存，按照进程数等比例划分）。例如：如果有三个进程都使用了一个共享库，共占用了30页内存。那么PSS将认为每个进程分别占用该共享库10页的大小。 PSS是非常有用的数据，因为系统中所有进程的PSS都相加的话，就刚好反映了系统中的 总共占用的内存。 而当一个进程被销毁之后， 其占用的共享库那部分比例的PSS，将会再次按比例分配给余下使用该库的进程。这样PSS可能会造成一点的误导，因为当一个进程被销毁后， PSS不能准确地表示返回给全局系统的内存。

USS - Unique Set Size （非常有用）
进程独自占用的物理内存（不包含共享库占用的内存）。USS是非常非常有用的数据，因为它反映了运行一个特定进程真实的边际成本（增量成本）。当一个进程被销毁后，USS是真实返回给系统的内存。当进程中存在一个可疑的内存泄露时，USS是最佳观察数据。


Memory Leak Tools-Malloc debug

从Android O1开始，使用Malloc debug 机制来进行memory Leak的分析；
Malloc debug机制的主要功能，
1.memory corruption
2.momory leaks
分析工具，GAT,NE/KE Analyzer(SP_offlineDebugSuite)
说明：
   原声的code一般都是在eng版本上默认打开malloc debug.
   为了暴增手机的performance，客户一般使用userdebug版本进行memory leak的分析


Enable Malloc debug:
adb shell "echo libc.debug.malloc.option="$DEBUG_OPTIONS">>data/local.prop"  
adb shell "echo libc.debug.malloc.program="$PROGRAM_NAME">>data/local.prop"
adb shell "chmod 644 /data/local.prop"
adb reboot

Disable Malloc debug:
adb shell "echo libc.debug.malloc.option=disable-malloc_debug>>data/local.prop"
adb shell "chmod 644 /data/local.prop"
adb reboot

Example:
adb shell "echo libc.debug.malloc.options="backtrace=16 guard=8 free_track">>data/local.prop"
adb shell "echo libc.debug.malloc.program=/vendor/bin/hw/camerahalserver>>data/local.prop"
一般来说　memory leak问题只需要保证backtrace有写入options即可。随着backtrae的增加会导致系统Loding增加。

# cdev 开发知识储备；
Makefile  
obj-y := tvibrator_drv.o
obj-m := tvibrator_drv.o

obj-t　vibrator_drv.c  文件编译得到tvibrator_drv.o 并连接进内核.
obj-m: 则表示该文件作为模块编译，生成tvibrator_drv.ko

misc设备其实也就是特殊的字符设备，可自动生成设备节点。misc_register就是用主标号10调用register_chrdev()的。
杂项设备作为字符设备的封装，为字符设备提供的简单的编程接口，如果编写新的字符驱动，可以考虑使用杂项设备接口，方便简单，只需要初始化一个miscdevice的结构，调用misc_register就可以了。系统最多有255个杂项设备.

insmod /向内核加载模块
rmmod  命令用于删除模块。

vmalloc 用于申请较大的内存空间，虚拟内存是连续的,物理内存不连续；
vfree

kmalloc  用于申请较小的、连续的物理内存；
kfree


mutex_lock　获得互斥体
mutex_unlock　释放互斥体

memcpy    从 src 复制 n 个字符到 dest。
memset    复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符。
memmove  另一个用于从 src 复制 n 个字符到 dest 的函数。

kmap
kunmap

* kstrtoint 将字符串转换为int

  int kstrtoint(const char *s, unsigned int base, int *res)

* kstrtouint   将字符串转换为无符合型整整

      int kstrtouint(const char *s, unsigned int base, unsigned int *res)

      s是输入字符串,base可以是10(10进制)或16(16进制),或者是0自动识别,res存放转换后的整形值.
      当没有错误时返回值是0;

* kstrtol　将字符串转换为长整型数　

　　　　　long int strtol (const char* str, char** endptr, int base);
          str 为要转换的字符串，endstr 为第一个不能转
          换的字符的指针，base 为字符串 str 所采用的进制。
* kstrtoul　　将字符串转换为无符号长整型数

    unsigned long kstrtoul (const char* str, char** endptr, int base);
*  kstrtoull　　将字符串转换为长长整型数

      int kstrtoll(const char *s, unsigned int base, long long *res)

* sprintf　把格式化的数据写入某个字符串缓冲区
      int sprintf (char * szBuffer, const char * szFormat, ...);

      第一个参数是字符串缓冲区，后面是一个格式字串。sprintf不是将格式化结果标准输出，而是将其存入szBuffer。该函数返回该字符串的长度。　

* snprintf　将可变个参数(...)按照format格式化成字符串，然后将其复制到str中

* vsprintf　vsprintf(char *buffer, char *format, va_list param);
          vsprintf是sprintf的一个变形，它只有三个参数。vsprintf用於执行有多个参数的自订函式，类似printf格式。vsprintf的前两个参数与sprintf相同：一个用於保存结果的字元缓冲区和一个格式字串。第三个参数是指向格式化参数阵列的指标。实际上，该指标指向在堆叠中供函式呼叫的变数。va_list、va_start和va_end巨集（在STDARG.H中定义）帮助我们处理堆叠指标。

* vsnprintf

* memcpy　由src指向地址为起始地址的连续n个字节的数据复制到以dest指向地址为起始地址的空间内。
        void *memcpy(void *dest,const void *src,size_t n);
        src和dest所指内存区域不能重叠，函数返回指向dest的指针。
        与strcpy相比，memcpy并不是遇到’\0’就结束，而是一定会拷贝完n个字节。
        如果目标数组dest本身已经有数据了，执行memcpy()后，将覆盖原有数据（最多覆盖n）。如果要追加数据，则每次执行memcpy后，要将目标数组地址追加到你要追加数据的地址。
        注意：src和dest都不一定是数组，任意的可读写的空间均可。

* strcpy　strcpy是一种C语言的标准库函数，strcpy把含有’\0’结束符的字符串复制到另一个地址空间，返回值为char。
　　　　　char *strncpy(char *desc,const char *src,size_t n);
        src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。
        注意：sizeof(指针)，sizeof（数组）的区别。
* strncpy

　　　　　char *strncpy(char *desc,const char *src,size_t n);  
　　　　　  函数strncpy从src指向地址为起始地址的空间中复制n个字符（不复制空字符后面的字符）
          到以desc指向的地址为起始地址的空间中。
　　　　　 如果未抵达n而src结束，则写入额外的空字符到dest，直至写入总共count个字符。

          strcpy是依据’\0’作为结束判断的，如果dest的空间不够，则会引起buffer overflow。
          memcpy用来在内存中复制数据，由于字符串是以’\0’结尾的，所以对于在数据中包含’\0’的数据只能用memcpy。
          strncpy和memcpy很相似，只不过他在一个终止的空字符处停止。当n>strlen(src)时，给dest不够数的空间里填充终止字符，但也不是’\0’,是’?’（在visual studio中）；当n<=strlen(src)时，dest是没有结束符’\0’的（经过测试，无论复制多少个当n<dest的空间长度时都会自动分配’\0’,当等于时没有’\0’,大于时报错；而memcpy基本一样，不过memcpy当n大于dest的长度时不会报错，在visual studio中有绿线警告提示）。
* strlcpy　　是更加安全版本的strcpy函数，在已知目的地址空间大小的情况下，把从src地址开始且含有'\0'结束符的字符串
          复制到以dest开始的地址空间,并不会造成缓冲区溢出

* strnlen　获取字符串s中实际字符个数，不包括结尾的'\0'

* sscanf　　从字符串读取格式化输入

down_read
up


seq_puts
seq_printf




*************************************************************
                 GFP_KERNEL的作用
*************************************************************
GFP_KERNEL是linux内存分配器的标志，标识着内存分配器将要采取的行为。
分配器标志分为行为修饰符，区修饰符及类型。行为修饰符表示内核应当如何分配所需的内存。
区修饰符表示内存区应当从何处分配。类型就是行为修饰符和区修饰符的合体。

在include/linux/gfp.h中定义,GFP_KERNEL是内核内存分配时最常用的，无内存可用时可引起休眠.

```
#define GFP_KERNEL(__GFP_WAIT | __GFP_IO | __GFP_FS)
__GFP_WAIT ： 缺内存页的时候可以睡眠;
__GFP_IO ： 允许启动磁盘IO；
__GFP_FS ： 允许启动文件系统IO。

```

1:在/dev/目录创建cdev;

alloc_chrdev_region 是让内核分配给我们一个尚未使用的主设备号，动态分配主设备和从设备号
cdev_init  将cdev和file_operations关联起来;
cdev_add  将cdev和设备号关联起来;
class_create  创建class并将class　注册到内核中，返回值为class机构体指针;
              用于动态创建设备的逻辑类，并完成字段的初始化，并在/sys/class 下新建一个目录。
              会在sys/class目录下生成test_class文件夹，test_class可以随意命名；           
device_create();会在/dev目录下生成xxx的设备文件，major是主设备号，可以在前面宏定义指定,也可以直接指定为0~254中的没被使用的一个
file_operations 对cdev操作函数；


```
    int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count,const char *name){}
    void cdev_init(struct cdev *cdev, const struct file_operations *fops){}
    class_create(THIS_MODULE, tvib_dev->name){};
    struct class * class_create(struct module *owner, const char *name){};
      static const struct file_operations tvibrator_file_operation
```
2: 在/sys/class/tvibrator/tvibrator目录下创建属性文件tvibrator_val
    device_create_file  函数可以在/sys/class/下创建对应的属性文件，从而通过对该文件的读写实现特定的数据操作。
    dev_set_drvdata     函数用来设置device 的私有数据，把设备的相关信息放到设备结构里作为私有数据存起来。
    dev_get_drvdata     函数用来获取device 的私有数据。

  ```
    DEVICE_ATTR(_name, _mode, _show, _store)
    _name：名称，也就是将在sys fs中生成的文件名称。
    _mode：上述文件的访问权限，与普通文件相同，UGO的格式。
    _show：显示函数，cat该文件时，此函数被调用。
    _store：写函数，echo内容到该文件时，此函数被调用。
  ```

```
    device_create_file(tvib_dev->device, &dev_attr_tvibrator_val);
    static ssize_t tvibrator_val_show(struct device* dev, struct device_attribute* attr, char* buf) {};
    static ssize_t tvibrator_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) {}
    static DEVICE_ATTR(tvibrator_val, 0660, tvibrator_val_show, tvibrator_val_store);
          tvibrator_val 属性文件名称，
          0660　　　　　　属性文件操作权限；
    　    tvibrator_val_show　　文件读操作函数；
　　　　　　tvibrator_val_store　文件写操作函数；
    dev_set_drvdata()；函数用来设置device 的私有数据
```

3:在proc目录创建文件；
　 proc_mkdir 在proc目录创建文件夹;
   proc_create 创建文件；

```
struct proc_dir_entry *proc_mkdir(const char *name, struct proc_dir_entry *parent);
static inline struct proc_dir_entry *proc_create(const char *name, mode_t mode,
        struct proc_dir_entry *parent, const struct file_operations *proc_fops);

        static const struct file_operations tvibrator_proc_ops = {
        	.open = tvibrator_proc_open,
          .read =  seq_read,
        	//.read = tvibrator_proc_read,
        	.write = tvibrator_proc_write,
        	.release = single_release,
        };

```



 参考文献：　https://blog.csdn.net/yanleizhouqing/article/details/47105073   
 　　　　　　https://blog.csdn.net/weixin_39821531/article/details/86582838



１.字符设备是 3 大类设备(字符设备、块设备和网络设备)中较简单的一类设备,提供连续的数据流，应用程序可以顺序读取，通常不支持随机存取。相反，此类设备支持按字节/字符来读写数据。

其驱动程序中完成的主要工作是初始化、添加和删除 cdev 结构体,申请和释放设备号,以及填充file_operations 结构体中的操作函数,实现file_operations 结构体中的read()、write()和ioctl()等函数是驱动设计的主体工作。

字符设备的重要的数据结构
```
struct tvibrator_device {
	struct cdev	cdev;
	struct class	*class;
	struct device	*device;
	dev_t		dev_no;
	char		*name;
  int      val;
  char mem[GLOBALFIFO_SIZE];
	struct semaphore sem;
	char kbuf[BUF_SIZE];
	u8 led;
};
```


在内核中，用32位的dev_t类型来保存设备编号，高12位为主设备号，低20位为次设备号。
要获取dev_t的主次设备，应使用：

MAJOR(dev_t dev);   //主设备号
MINOR(dev_t dev);   //次设备号
如果要将主次设备号转为dev_t类型，应使用：
MKDEV(int major, int minor);


#### file_operations结构就是用来建立这种连接的。
```
struct file_operations {
	struct module *owner;
	loff_t (*llseek) (struct file *, loff_t, int);
	ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
	ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
	ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);
	ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
	int (*iterate) (struct file *, struct dir_context *);//
	int (*iterate_shared) (struct file *, struct dir_context *);//
	unsigned int (*poll) (struct file *, struct poll_table_struct *);
	long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
	long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
	int (*mmap) (struct file *, struct vm_area_struct *);
	int (*open) (struct inode *, struct file *);
	int (*flush) (struct file *, fl_owner_t id);
	int (*release) (struct inode *, struct file *);
	int (*fsync) (struct file *, loff_t, loff_t, int datasync);
	int (*fasync) (int, struct file *, int);
	int (*lock) (struct file *, int, struct file_lock *);
	ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
	unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
	int (*check_flags)(int);
	int (*flock) (struct file *, int, struct file_lock *);
	ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
	ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int);
	int (*setlease)(struct file *, long, struct file_lock **, void **);
	long (*fallocate)(struct file *file, int mode, loff_t offset,
			  loff_t len);
	void (*show_fdinfo)(struct seq_file *m, struct file *f);
#ifndef CONFIG_MMU
	unsigned (*mmap_capabilities)(struct file *);
#endif
	ssize_t (*copy_file_range)(struct file *, loff_t, struct file *,
			loff_t, size_t, unsigned int);
	int (*clone_file_range)(struct file *, loff_t, struct file *, loff_t,
			u64);
	ssize_t (*dedupe_file_range)(struct file *, u64, u64, struct file *,
			u64);
};
```
它其实是一种形式的文档而已，表明指针是一个用户空间地址，因此不能被直接引用。对通常的编译来将，__user没有任何效果，但是可由外部检查软件使用，用来寻找对用户空间的错误使用。


struct module *owner;

第一个file_operations字段并不是一个操作，相反，它是一个指向拥有该结构的模块的指针。内核使用这个字段以避免在模块的操作正在被使用时卸载该模块。几乎在所有情况下，该成员都会被初始化为THIS_MODULE，它是定义在<linux/module.h>中的一个宏。
loff_t (*llseek) (struct file *, loff_t, int);

llseek 方法用作改变文件中的当前读/写位置, 并且新位置作为(正的)返回值。

(指针参数struct file *为进行读取信息的目标文件结构体指针；参数 loff_t 为文件定位的目标偏移量；参数int为对文件定位的起始地址，这个值可以为文件开头（SEEK_SET，0,当前位置(SEEK_CUR,1)，文件末尾(SEEK_END,2)）

loff_t 参数是一个"long offset", 并且就算在 32位平台上也至少 64 位宽. 错误由一个负返回值指示；如果这个函数指针是 NULL, seek 调用会以潜在地无法预知的方式修改 file结构中的位置计数器。
ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);

用来从设备中读取数据。该函数指针被赋值为NULL值时，将导致read系统调用出错并返回-EINVAL（"Invalid argument，非法参数"）。函数返回非负值表示成功读取的字节数（返回值为"signed size"数据类型，通常就是目标平台上的固有整数类型）。
ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);

向设备发送数据。如果没有这个函数，write系统调用会向程序返回一个-EINVAL。如果返回值非负，则表示成功写入的字节数。
ssize_t (*read_iter) (struct kiocb *, struct iov_iter *);

异步的读取操作，在函数返回之前可能不会完成的读取操作。如果该函数为NULL，所有的操作将通过read（同步）处理。

linux 4.5版本前为：
``
ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
ssize_t (*write_iter) (struct kiocb *, struct iov_iter *);
```
异步写入操作。

linux 4.5版本前为：
```
ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);
unsigned int (*poll) (struct file *, struct poll_table_struct *);
```
poll方法是poll、epoll和select这三个系统调用的后端实现。这三个系统调用可用来查询某个或多个文件描述符上的读取或写入是否被阻塞。poll方法应该返回一个位掩码，用来指出非阻塞的读取或写入是否可能，并且也会向内核提供将调用进程置于休眠状态直到I/O变为可能时的信息。如果驱动程序将poll方法定义为NULL，则设备会被认为即可读也可写，并且不会被阻塞。
```
long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
```
ioctl提供了一种执行设备特定命令的方法（如格式化软盘的某个磁道，这即不是读操作也不是写操作）。另外，内核还能识别一部分ioctl命令，而不必调用fops表中的ioctl。如果设备不提供ioctl入口点。这对于任何内核未预先定义的请求，ioctl系统调用将返回错误（-ENOTTY，“No such ioctl for device，该设备无此ioctl命令”）。

1、compat_ioctl：支持64bit的driver必须要实现的ioctl，当有32bit的userspace application call 64bit kernel的IOCTL的时候，这个callback会被调用到。如果没有实现compat_ioctl，那么32位的用户程序在64位的kernel上执行ioctl时会返回错误：Not a typewriter

2、如果是64位的用户程序运行在64位的kernel上，调用的是unlocked_ioctl，如果是32位的APP运行在32位的kernel上，调用的也是unlocked_ioctl
```
int (*mmap) (struct file *, struct vm_area_struct *);
```
mmap用于请求将设备内存映射到进程地址空间。如果设备没有实现这个方法，那么mmap系统调用将返回-ENODEV。
```
int (*open) (struct inode *, struct file *);
```
尽管这始终是对设备文件执行的第一个操作，然而却并不要求驱动程序一定要声明一个相应的方法。如果这个入口为NULL，设备的打开操作永远成功，但系统不会通知驱动程序。
```
int (*flush) (struct file *, fl_owner_t id);
```
对flush操作的调用发生在进程关闭设备文件描述符的时候，它应该执行（并等待）设备上尚未完结的操作。请不要将它同用户程序使用的fsync操作相混淆。目前，flush仅仅用于少数几个驱动程序，比如SCSI磁带驱动程序用它来确保设备被关闭之前所有的数据都被写入磁带中。如果flush被置为NULL，内核将简单地忽略用户应用程序的请求。
```
int (*release) (struct inode *, struct file *);
```
当file结构被释放时，将调用这个操作。与open相仿，也可以将release置为NULL。

注意：release并不是在进程每次调用close时都会被调用。只要file结构被共享（如在fork或dup调用之后），release就会等到所有的副本都关闭之后才会得到调动。如果需要在关闭任意一个副本时刷新那些待处理的数据，则实现flush方法。
```
int (*fsync) (struct file *, loff_t, loff_t, int datasync);
```
该方法是fsync系统调用的后端实现，用户调用它来刷新待处理的数据。如果驱动程序没有实现这一方法，fsync系统调用返回-EINVAL。
```
int (*fasync) (int, struct file *, int);

```
这个操作用来通知设备起FASYNC标志位发生了变化。如果设备不支持异步通知，该字段可以是NULL。
```
int (*lock) (struct file *, int, struct file_lock *);
```
lock方法用于实现文件锁定，锁定是常规文件不可缺少的特性，但设备驱动程序几乎从来不会实现这个方法。
```
ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int);
```
由内核调用，将数据发送到对应的文件，每次一个数据页。设备驱动程序通常也不需要实现sendpage。
```
unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long);
```
该方法的目的是在进程的地址空间中找到一个合适的位置，以便将底层驱动设备中的内存段映射到该位置。该任务通常由内存管理代码完成，但该方法的存在可允许驱动程序强制满足特定设备需要的任何对齐需求。大部分驱动程序可设置该方法为NULL。
int (*check_flags)(int);

该方法允许模块检查传递给fcntl（F_SETFL...）调用的标志。



设备入口初始化函数；
```
static int __init tvibrator_drv_init(void){
  	return ０;
  ｝
```
设备卸载函数；
```
static void __exit tvibrator_drv_exit(void){}
```
module_init和module_exit，这两个函数分别在加载和卸载驱动时被调用，即调用insmod和rmmod命令的时候
```
module_init(tvibrator_drv_init);　　
module_exit(tvibrator_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu.tao <liutao3325@gmail.com>");
MODULE_DESCRIPTION("Tinno Tvibrator Core Driver");
MODULE_ALIAS("Tvibrator");
```

编写模块必须先声明下面两句：

    #include <linux/module.h>               //这个头文件包含了许多符号与函数的定义，这些符号与函数多与加载模块有关
    #include <linux/init.h>                      //这个头文件包含了你的模块初始化与清除的函数

    MODULE_LICENSE("GPL");          // "GPL" 是指明了 这是GNU General Public License的任意版本
                                    // “GPL v2” 是指明 这仅声明为GPL的第二版本
                                    // "GPL and addtional"
                                    // "Dual BSD/GPL"
                                    // "Dual MPL/GPL"
                                    // "Proprietary"  私有的
                                    // 除非你的模块显式地声明一个开源版本，否则内核会默认你这是一个私有的模块(Proprietary)。
    MODULE_AUTHOR                        // 声明作者
    MODULE_DESCRIPTION              // 对这个模块作一个简单的描述，这个描述是"human-readable"的
    MODULE_VERSION                        // 这个模块的版本
    MODULE_ALIAS                               // 这个模块的别名
    MODULE_DEVICE_TABLE            // 告诉用户空间这个模块支持什么样的设备


MODULE_声明可以写在模块的任何地方（但必须在函数外面），但是惯例是写在模块最后。

/*
 * Copyright (C) 2016 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/sysfs.h>
#include <linux/device.h>       /* needed by device_* */
#include <linux/vmalloc.h>      /* needed by kmalloc */
#include <linux/uaccess.h>      /* needed by copy_to_user */
#include <linux/slab.h>         /* needed by kmalloc */
#include <linux/poll.h>         /* needed by poll */
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/suspend.h>
#include <linux/timer.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/of_fdt.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/semaphore.h>
#include <linux/init.h>
#define TVIBRATOR_DEVNAME    "tvibrator"
#define TVIBRATOR_SYSFS              "tvibrator"
DEFINE_SEMAPHORE(tvib_sem);

// static dev_t tvibrator_devno;
// static struct cdev *tvibrator_cdev;
// static struct class *tvibrator_class;
// static struct device *tvibrator_device;
//static struct kobject tvibrator_kobj;

#define GLOBALFIFO_SIZE 64
#define NAME_SIZE 16
#define GLOBALFIFO_NUM 3
#define BUF_SIZE	10
struct tvibrator_device {
	unsigned int      val;
	char		name[NAME_SIZE];
	struct cdev	cdev;
	struct class	*class;
	struct device	*device;
	dev_t		dev_no;


};

static struct tvibrator_device *tvib_dev;
static struct proc_dir_entry *tvib_proc_dir;
//static  struct proc_dir_entry *tvib_proc_file;
static int tvibrator_open(struct inode *inode, struct file *filp){

     int major = 0;
     int minor = 0;
     unsigned char *devname = NULL;

    struct tvibrator_device *dev =  container_of(inode->i_cdev, struct tvibrator_device,cdev);
     filp->private_data = dev;
     major = MAJOR(inode->i_rdev);
     minor = MINOR(inode->i_rdev);
      devname = filp->f_path.dentry->d_iname;
     pr_info("%s:%d:%d\n", __func__, major, minor);
     pr_info("tvibrator open \n");

		 if (tvib_dev == NULL)
			 return -EINVAL;

		 if (filp == NULL)
			 return -EINVAL;

   	return 0;
}


static ssize_t tvibrator_read(struct file *filp, char __user *buf,size_t size, loff_t *ppos){
	//struct tvibrator_device* dev = tvib_dev;
 	struct tvibrator_device *dev = filp->private_data;
		 pr_info("tvibrator read size = %d val = %d\n",sizeof(dev->val),dev->val);
		 pr_info("size=%ld ppos=%d",(unsigned long)size, (int)(*ppos));



#if 0
  /*将寄存器name的值拷贝到用户提供的缓冲区*/
		int 	len = sizeof(dev->name);
		if (*ppos)
			return 0;

  	if(copy_to_user(buf, &dev->name, len)) {
  		pr_err("Copy to user failed\n");
				return -EFAULT;
  	}
    pr_info("tvibrator read succeed\n");

  	*ppos += len;

  	return len;

#else

  /*将寄存器val的值拷贝到用户提供的缓冲区*/
	//buf为字符型指针，必须先将val转换为char;
		char buffer[BUF_SIZE];
		int len =sizeof(buffer);
		memset(buffer, '\0', BUF_SIZE);
    sprintf(buffer,"%d\n",dev->val);

   pr_info("tvibrator  val = %s\n",buffer);

		if (*ppos) //只进行一次read
			return 0;
		if(copy_to_user(buf,&buffer , BUF_SIZE)!= 0) {
			pr_err("Copy to user failed\n");
				return -EFAULT;
		}
	    *ppos += len;
			return len; // 0 不返回数据，len　返回所读取的char长度；
# endif

}

static ssize_t tvibrator_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos){
	signed char tmp_buf[50] = { 0 };

  //struct tvibrator_device* dev = tvib_dev;
  struct tvibrator_device *dev = filp->private_data;
  pr_info("tvibrator write size %d\n",size);
	if (size <= 0 || buf == NULL) {
		pr_info( "tvibrator failed parameter not accept\n");
		return -EFAULT;
	}
  	/*将用户提供的缓冲区的值写到设备寄存器去*/
  	if(copy_from_user(tmp_buf, buf, size)) {
     	pr_info("tvibrator failed copy_from_user\n");
  		return -EFAULT;
  	}
		if (kstrtouint(tmp_buf, 0, &dev->val)) {
			pr_info( "tvibrator Not a valid dbg_level: %s\n", tmp_buf);
			return -EFAULT;
		}
   pr_info("tvibrator write size = %d val = %d\n",sizeof(dev->val),dev->val);


  pr_info("tvibrator write succeed\n");
	return size;

}
static int tvibrator_release(struct inode *inode, struct file *file){
    pr_info("tvibrator release \n");
	return 0;
}

// /*读取寄存器val的值到缓冲区buf中，内部使用*/
// static ssize_t __tvibrator_get_val(struct tvibrator_device *dev, char *buf) {
// 	int val = 0;
//
// 	val = dev->val;
// 	return snprintf(buf, PAGE_SIZE, "%d\n", val);
// }
//
// /*把缓冲区buf的值写到设备寄存器val中去，内部使用*/
// static ssize_t __tvibrator_set_val(struct tvibrator_device* dev, const char* buf, size_t count) {
// 	int val = 0;
// 	/*将字符串转换成数字*/
// 	val = simple_strtol(buf, NULL, 10);
// 	dev->val = val;
// 	return count;
// }

static struct tvibrator_device *to_tvibrator_device(struct device *dev){
	return dev_get_drvdata(dev);
}


/*读取设备属性val*/
static ssize_t tvibrator_val_show(struct device* dev, struct device_attribute* attr, char* buf) {
	struct tvibrator_device *tdev = to_tvibrator_device(dev);
	  pr_info("tvibrator_val_show %d\n",tdev->val);
	return sprintf(buf, "%d\n", tdev->val);
}

/*写设备属性val*/
static ssize_t tvibrator_val_store(struct device* dev, struct device_attribute* attr, const char* buf, size_t count) {
	struct tvibrator_device *tdev =  to_tvibrator_device(dev);
//struct tvibrator_device *tdev	= container_of(dev, struct flashlight_device, dev);
	int val = 0;

	if (kstrtoint(buf, 10, &val))
		return -EINVAL;
 	  tdev->val = val;
  pr_info("tvibrator_val_store %d\n",&tdev->val);
	return count;
}

static DEVICE_ATTR(tvibrator_val, 0660, tvibrator_val_show, tvibrator_val_store);






ssize_t tvibrator_proc_read(struct file *filp, char __user *buf, size_t count, loff_t *ppos){


	struct tvibrator_device *dev = filp->private_data;
	 pr_info("tvibrator read size = %d val = %d\n",sizeof(dev->val),dev->val);
	 pr_info("size=%ld ppos=%d",(unsigned long)count, (int)(*ppos));

 /*将寄存器val的值拷贝到用户提供的缓冲区*/
//buf为字符型指针，必须先将val转换为char;
	char buffer[BUF_SIZE];
	int len =sizeof(buffer);
	memset(buffer, '\0', BUF_SIZE);
	sprintf(buffer,"%d\n",dev->val);

	pr_info("tvibrator  val = %s\n",buffer);

	if (*ppos) //只进行一次read
		return 0;
	if(copy_to_user(buf,&buffer , BUF_SIZE)!= 0) {
		pr_err("Copy to user failed\n");
			return -EFAULT;
	}
		*ppos += len;
		return len; // 0 不返回数据，len　返回所读取的char长度；
}

ssize_t tvibrator_proc_write(struct file *filp, const char __user *buffer, size_t count, loff_t *f_pos){
	signed char tmp_buf[50] = { 0 };

  //struct tvibrator_device* dev = tvib_dev;
  struct tvibrator_device *dev = filp->private_data;
  pr_info("tvibrator write size %d\n",count);
	if (count <= 0 || buffer == NULL) {
		pr_info( "tvibrator failed parameter not accept\n");
		return -EFAULT;
	}
  	/*将用户提供的缓冲区的值写到设备寄存器去*/
  	if(copy_from_user(tmp_buf, buffer, count)) {
     	pr_info("tvibrator failed copy_from_user\n");
  		return -EFAULT;
  	}
		if (kstrtouint(tmp_buf, 0, &dev->val)) {
			pr_info( "tvibrator Not a valid dbg_level: %s\n", tmp_buf);
			return -EFAULT;
		}
   pr_info("tvibrator write size = %d val = %d\n",sizeof(dev->val),dev->val);


  pr_info("tvibrator write succeed\n");
	return count;

}



static int monitor_hang_show(struct seq_file *m, void *v){
	seq_printf(m, "[Hang_Detect] show Hang_info size %d\n ",555);
//	SEQ_printf(m, "%s", Hang_Info);
	return 0;
}

static int tvibrator_proc_open(struct inode *inode, struct file *file){
	return single_open(file, monitor_hang_show, inode->i_private);
}



static const struct file_operations tvibrator_proc_ops = {
	.open = tvibrator_proc_open,
  .read =  seq_read,
	//.read = tvibrator_proc_read,
	.write = tvibrator_proc_write,
	.llseek	 = seq_lseek,
	.release = single_release,
};

// /*创建/proc/hello文件*/
// static void tvibrator_create_proc(void) {
// //	struct proc_dir_entry* entry;
//  proc_create(TVIBRATOR_DEVNAME, 0444, NULL, &tvibrator_proc_ops);
// 	//entry = create_proc_entry(TVIBRATOR_DEVNAME, 0, NULL);
// 	// if(entry) {
// 	// 	entry->owner = THIS_MODULE;
// 	// 	entry->read_proc = tvibrator_proc_read;
// 	// 	entry->write_proc = tvibrator_proc_write;
// 	// }
// }
//
// /*删除/proc/hello文件*/
// static void tvibrator_remove_proc(void) {
// 	remove_proc_entry(TVIBRATOR_DEVNAME, NULL);
// 	//remove_proc_entry(TVIBRATOR_DEVNAME, NULL);
// }




#if 1



static const struct file_operations tvibrator_file_operation = {
  .open = tvibrator_open,
  .write = tvibrator_write,
  .read =  tvibrator_read,
  .release = tvibrator_release,
};

static int __init tvibrator_drv_init(void){
	int err = -1;
	tvib_dev = kmalloc(sizeof(struct tvibrator_device), GFP_KERNEL);
	 memset(tvib_dev, 0, sizeof(struct tvibrator_device));

  if (!tvib_dev){
      pr_err("tvibrator Failed to kzalloc\n");
      return -ENOMEM;
  }
	sprintf(tvib_dev->name,TVIBRATOR_DEVNAME);
  /*动态分配主设备和从设备号*/
  if (alloc_chrdev_region(&(tvib_dev->dev_no),0,1,tvib_dev->name)) {
    pr_err("tvibrator Failed to allocate char device region\n");
    return -1;
  }
   pr_info("tvibrator MAJOR Number is %d\n",MAJOR(tvib_dev->dev_no));
   pr_info("tvibrator MINOR Number is %d\n",MINOR(tvib_dev->dev_no));

   cdev_init(&tvib_dev->cdev, &tvibrator_file_operation);
	 tvib_dev->cdev.owner = THIS_MODULE;

  //  tvib_dev->cdev = cdev_alloc();
  //  tvib_dev->cdev->owner = THIS_MODULE;
  //  tvib_dev->cdev->ops = &tvibrator_file_operation;

   /*注册字符设备*/
    if (cdev_add(&tvib_dev->cdev, tvib_dev->dev_no, 1)) {
  		pr_info("tvibrator Failed to add cdev\n");
      	unregister_chrdev_region(tvib_dev->dev_no, 1);
	    return -1;
  	}
	  pr_info("tvibrator  add cdev  succeed\n");

   /*在/sys/class/目录下创建设备类tvibrator别目录test*/

    tvib_dev->class = class_create(THIS_MODULE, tvib_dev->name);
    if (IS_ERR(tvib_dev->class)) {
      pr_info("tvibrator fail to create class\n");
      return -1;
    }
    pr_info("tvibrator create class  succeed\n");

   /*在/dev/目录和/sys/class/test目录下分别创建设备文件test*/
    tvib_dev->device = device_create(tvib_dev->class, NULL, tvib_dev->dev_no, NULL,tvib_dev->name);
    if (IS_ERR(tvib_dev->device)) {
      pr_info("tvibrator fail to create device\n");
      return -1;
    }
    pr_info("tvibrator create device  succeed\n");


		/*在/sys/class/tvibrator/tvibrator目录下创建属性文件tvibrator_val*/
			err = device_create_file(tvib_dev->device, &dev_attr_tvibrator_val);
			if(err < 0) {
			pr_info("Failed to create attribute val");
			}

			dev_set_drvdata(tvib_dev->device, tvib_dev);

			//在proc创建tvibrator；
			tvib_proc_dir = proc_mkdir("tvibrator", NULL);
			if (tvib_proc_dir == NULL) {
				pr_info("aed tvib_proc_dir failed\n");
				return -1;
			}

		tvib_proc_dir =	proc_create(TVIBRATOR_DEVNAME, 0664, tvib_proc_dir, &tvibrator_proc_ops);

		if (tvib_proc_dir == NULL) {
			pr_info("tvib_proc_file proc_create failed at tvib_proc_file\n");
			return -1;
		}

   return 0;
}

static void __exit tvibrator_drv_exit(void){
	  pr_debug("Exit start\n");
    remove_proc_entry(TVIBRATOR_DEVNAME, NULL);
		//remove_proc_entry(TVIBRATOR_DEVNAME, tvib_proc_dir);
    device_destroy(tvib_dev->class, tvib_dev->dev_no);
  	//device_del(tvibrator_device);
  	class_destroy(tvib_dev->class);
  	cdev_del(&tvib_dev->cdev);
		kfree(tvib_dev);
  	unregister_chrdev_region(tvib_dev->dev_no, 1);

	  pr_debug("Exit done\n");
}


#else



static const struct file_operations tvibrator_file_operation =
{
  .owner = THIS_MODULE,
  .open = tvibrator_open,
  .write = tvibrator_write,
  .read =  tvibrator_read,
  .release = tvibrator_release,
};

static struct miscdevice tvibrator_dev =
{
	.minor = MISC_DYNAMIC_MINOR,
	.name = "tvibrator",
	.fops = &tvibrator_file_operation,
};

static int __init tvibrator_drv_init(void)
{
	int ret = 0;

	ret = misc_register(&tvibrator_dev);
	if (unlikely(ret != 0)) {
		pr_debug("[tvibrator] misc register failed\n");
		return ret;
	}

	return ret;
}


static void __exit tvibrator_drv_exit(void)
{
	misc_deregister(&tvibrator_dev);
}
#endif

module_init(tvibrator_drv_init);
module_exit(tvibrator_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("liu.tao <liutao3325@gmail.com>");
MODULE_DESCRIPTION("Tinno Tvibrator Core Driver");
MODULE_ALIAS("Tvibrator");

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/ioctl.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/delay.h>

// Define the GPIO pins used to control the seven segment display
#define SEG_A 17
#define SEG_B 27
#define SEG_C 22
#define SEG_D 10
#define SEG_E 9
#define SEG_F 11
#define SEG_G 5
#define SEG_DP 4

#define SEG_A2 14
#define SEG_B2 15
#define SEG_C2 18
#define SEG_D2 23
#define SEG_E2 24
#define SEG_F2 25
#define SEG_G2 8
#define SEG_DP2 7

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
 
int32_t value = 0;
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev sevenseg_cdev;

// Function Prototypes
static int      __init sevenseg_driver_init(void);
static void     __exit sevenseg_driver_exit(void);
static int      sevenseg_open(struct inode *inode, struct file *file);
static int      sevenseg_release(struct inode *inode, struct file *file);
static ssize_t  sevenseg_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  sevenseg_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     sevenseg_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// File operation sturcture

static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = sevenseg_read,
        .write          = sevenseg_write,
        .open           = sevenseg_open,
        .unlocked_ioctl = sevenseg_ioctl,
        .release        = sevenseg_release,
};

// This function will be called when we open the Device file

static int sevenseg_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}

// This function will be called when we close the Device file

static int sevenseg_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}

// This function will be called when we read the Device file

static ssize_t sevenseg_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read Function\n");
        return 0;
}

// This function will be called when we write the Device file
static ssize_t sevenseg_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write function\n");
        return len;
}

// Set the GPIOs to output mode
void cal(int i,int j){
// 10's Place
    switch(i){
        case 0:
        // Set 0
        gpio_direction_output(SEG_A, 1);
        gpio_direction_output(SEG_B, 1);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 1);
        gpio_direction_output(SEG_E, 1);
        gpio_direction_output(SEG_F, 1);
        gpio_direction_output(SEG_G, 0);
        gpio_direction_output(SEG_DP, 0);
        break;

        case 1:
        // Set 1 
        gpio_direction_output(SEG_A, 0);
        gpio_direction_output(SEG_B, 1);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 0);
        gpio_direction_output(SEG_E, 0);
        gpio_direction_output(SEG_F, 0);
        gpio_direction_output(SEG_G, 0);
        gpio_direction_output(SEG_DP, 0);
        break;

        // Set 2 
        case 2:
        gpio_direction_output(SEG_A, 1);
        gpio_direction_output(SEG_B, 1);
        gpio_direction_output(SEG_C, 0);
        gpio_direction_output(SEG_D, 1);
        gpio_direction_output(SEG_E, 1);
        gpio_direction_output(SEG_F, 0);
        gpio_direction_output(SEG_G, 1);
        gpio_direction_output(SEG_DP, 0);
        break;

        // Set 3
        case 3:
        gpio_direction_output(SEG_A, 1);
        gpio_direction_output(SEG_B, 1);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 1);
        gpio_direction_output(SEG_E, 0);
        gpio_direction_output(SEG_F, 0);
        gpio_direction_output(SEG_G, 1);
        gpio_direction_output(SEG_DP, 0);
        break; 

        // Set 4
        case 4:
        gpio_direction_output(SEG_A, 0);
        gpio_direction_output(SEG_B, 1);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 0);
        gpio_direction_output(SEG_E, 0);
        gpio_direction_output(SEG_F, 1);
        gpio_direction_output(SEG_G, 1);
        gpio_direction_output(SEG_DP, 0);
        break;

        // Set 5
        case 5:
        gpio_direction_output(SEG_A, 1);
        gpio_direction_output(SEG_B, 0);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 1);
        gpio_direction_output(SEG_E, 0);
        gpio_direction_output(SEG_F, 1);
        gpio_direction_output(SEG_G, 1);
        gpio_direction_output(SEG_DP, 0);
        break;

        // Set 6
        case 6:
        gpio_direction_output(SEG_A, 1);
        gpio_direction_output(SEG_B, 0);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 1);
        gpio_direction_output(SEG_E, 1);
        gpio_direction_output(SEG_F, 1);
        gpio_direction_output(SEG_G, 1);
        gpio_direction_output(SEG_DP, 0);
        break;

        // Set 7
        case 7:
        gpio_direction_output(SEG_A, 1);
        gpio_direction_output(SEG_B, 1);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 0);
        gpio_direction_output(SEG_E, 0);
        gpio_direction_output(SEG_F, 0);
        gpio_direction_output(SEG_G, 0);
        gpio_direction_output(SEG_DP, 0);
        break;

        // Set 8
        case 8:
        gpio_direction_output(SEG_A, 1);
        gpio_direction_output(SEG_B, 1);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 1);
        gpio_direction_output(SEG_E, 1);
        gpio_direction_output(SEG_F, 1);
        gpio_direction_output(SEG_G, 1);
        gpio_direction_output(SEG_DP, 0);
        break;

        // Set 9
        case 9:
        gpio_direction_output(SEG_A, 1);
        gpio_direction_output(SEG_B, 1);
        gpio_direction_output(SEG_C, 1);
        gpio_direction_output(SEG_D, 1);
        gpio_direction_output(SEG_E, 0);
        gpio_direction_output(SEG_F, 1);
        gpio_direction_output(SEG_G, 1);
        gpio_direction_output(SEG_DP, 0);
        break;
    }
// Unit's Place
    switch(j){
        // Set 0
        case 0:
        gpio_direction_output(SEG_A2, 1);
        gpio_direction_output(SEG_B2, 1);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 1);
        gpio_direction_output(SEG_E2, 1);
        gpio_direction_output(SEG_F2, 1);
        gpio_direction_output(SEG_G2, 0);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 1
        case 1:
        gpio_direction_output(SEG_A2, 0);
        gpio_direction_output(SEG_B2, 1);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 0);
        gpio_direction_output(SEG_E2, 0);
        gpio_direction_output(SEG_F2, 0);
        gpio_direction_output(SEG_G2, 0);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 2 
        case 2:
        gpio_direction_output(SEG_A2, 1);
        gpio_direction_output(SEG_B2, 1);
        gpio_direction_output(SEG_C2, 0);
        gpio_direction_output(SEG_D2, 1);
        gpio_direction_output(SEG_E2, 1);
        gpio_direction_output(SEG_F2, 0);
        gpio_direction_output(SEG_G2, 1);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 3
        case 3:
        gpio_direction_output(SEG_A2, 1);
        gpio_direction_output(SEG_B2, 1);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 1);
        gpio_direction_output(SEG_E2, 0);
        gpio_direction_output(SEG_F2, 0);
        gpio_direction_output(SEG_G2, 1);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 4
        case 4:
        gpio_direction_output(SEG_A2, 0);
        gpio_direction_output(SEG_B2, 1);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 0);
        gpio_direction_output(SEG_E2, 0);
        gpio_direction_output(SEG_F2, 1);
        gpio_direction_output(SEG_G2, 1);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 5
        case 5:
        gpio_direction_output(SEG_A2, 1);
        gpio_direction_output(SEG_B2, 0);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 1);
        gpio_direction_output(SEG_E2, 0);
        gpio_direction_output(SEG_F2, 1);
        gpio_direction_output(SEG_G2, 1);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 6
        case 6:
        gpio_direction_output(SEG_A2, 1);
        gpio_direction_output(SEG_B2, 0);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 1);
        gpio_direction_output(SEG_E2, 1);
        gpio_direction_output(SEG_F2, 1);
        gpio_direction_output(SEG_G2, 1);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 7
        case 7:
        gpio_direction_output(SEG_A2, 1);
        gpio_direction_output(SEG_B2, 1);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 0);
        gpio_direction_output(SEG_E2, 0);
        gpio_direction_output(SEG_F2, 0);
        gpio_direction_output(SEG_G2, 0);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 8
        case 8:
        gpio_direction_output(SEG_A2, 1);
        gpio_direction_output(SEG_B2, 1);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 1);
        gpio_direction_output(SEG_E2, 1);
        gpio_direction_output(SEG_F2, 1);
        gpio_direction_output(SEG_G2, 1);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;

        // Set 9
        case 9:
        gpio_direction_output(SEG_A2, 1);
        gpio_direction_output(SEG_B2, 1);
        gpio_direction_output(SEG_C2, 1);
        gpio_direction_output(SEG_D2, 1);
        gpio_direction_output(SEG_E2, 0);
        gpio_direction_output(SEG_F2, 1);
        gpio_direction_output(SEG_G2, 1);
        gpio_direction_output(SEG_DP2, 0);
        msleep(300);
        break;
    }
}
void display(int value){
	  int i=value/10;
      int j=value%10;
      cal(i,j);
  }
/*
** This function will be called when we write IOCTL on the Device file
*/
static long sevenseg_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
         switch(cmd) {
                case WR_VALUE:
                        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                        {
                                pr_err("Data Write : Err!\n");
                        }
                        display(value);
                        pr_info("Value = %d\n", value);
                        break;
                case RD_VALUE:
                        if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Data Read : Err!\n");
                        }
                        break;
                default:
                        pr_info("Default\n");
                        break;
        }
        return 0;
}
 
// Module Init function
static int __init sevenseg_driver_init(void)
{
  // Request access to the GPIO pins
  if (gpio_request(SEG_A, "SEG_A")  ||
      gpio_request(SEG_B, "SEG_B")  ||
      gpio_request(SEG_C, "SEG_C")  ||
      gpio_request(SEG_D, "SEG_D")  ||
      gpio_request(SEG_E, "SEG_E")  ||
      gpio_request(SEG_F, "SEG_F")  ||
      gpio_request(SEG_G, "SEG_G")  ||
      gpio_request(SEG_DP, "SEG_DP")||
      gpio_request(SEG_A2, "SEG_A2") ||
      gpio_request(SEG_B2, "SEG_B2") ||
      gpio_request(SEG_C2, "SEG_C2") ||
      gpio_request(SEG_D2, "SEG_D2") ||
      gpio_request(SEG_E2, "SEG_E2") ||
      gpio_request(SEG_F2, "SEG_F2") ||
      gpio_request(SEG_G2, "SEG_G2") ||
      gpio_request(SEG_DP2, "SEG_DP2"))
  {
    printk(KERN_ERR "Error requesting GPIOs for seven segment display\n");
    return -1;
  }

    // Allocating Major number
    if((alloc_chrdev_region(&dev, 0, 1, "sevenseg_Dev")) <0){
            pr_err("Cannot allocate major number\n");
            return -1;
    }
    pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));

    // Creating cdev structure
    cdev_init(&sevenseg_cdev,&fops);

    /*Adding character device to the system*/
    if((cdev_add(&sevenseg_cdev,dev,1)) < 0){
        pr_err("Cannot add the device to the system\n");
        goto r_class;
    }

    // Creating struct class
    if(IS_ERR(dev_class = class_create(THIS_MODULE,"sevenseg_class"))){
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }

    // Creating device
    if(IS_ERR(device_create(dev_class,NULL,dev,NULL,"sevenseg_device"))){
        pr_err("Cannot create the Device 1\n");
        goto r_device;
    }
    pr_info("Device Driver Insert...Done!!!\n");
    return 0;
 
    r_device:
        class_destroy(dev_class);
    r_class:
        unregister_chrdev_region(dev,1);
        return -1;
}


// Module exit function
static void __exit sevenseg_driver_exit(void)
{
      // Release access to the GPIO pins
        gpio_free(SEG_A);
        gpio_free(SEG_B);
        gpio_free(SEG_C);
        gpio_free(SEG_D);
        gpio_free(SEG_E);
        gpio_free(SEG_F);
        gpio_free(SEG_G);
        gpio_free(SEG_DP);

	    gpio_free(SEG_A2);
        gpio_free(SEG_B2);
        gpio_free(SEG_C2);
        gpio_free(SEG_D2);
        gpio_free(SEG_E2);
        gpio_free(SEG_F2);
        gpio_free(SEG_G2);
        gpio_free(SEG_DP2);

        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&sevenseg_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Remove...Done!!!\n");

}
 
module_init(sevenseg_driver_init);
module_exit(sevenseg_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sharan Pai");    

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <asm/irq.h>
#include <mach/map.h>
#include <mach/gpio.h>
#include <asm/gpio.h>
#include <mach/regs-gpio.h>
#include <plat/gpio-cfg.h>

MODULE_LICENSE("GPL");

#define DEVICE_NAME "dipsw"
#define DEVICE_MAJOR    235

// Device Address
#define DIP		0x0E

#define ADDR_PIN_L_0(x)           EXYNOS4212_GPM0(x+1)
#define ADDR_PIN_L_1(x)           EXYNOS4212_GPM1(x)

#define ADDR_PIN_H_1(x)           EXYNOS4212_GPM1(x+1)

#define DATA_PIN_2(x)             EXYNOS4212_GPM2(x)
#define DATA_PIN_3(x)             EXYNOS4212_GPM3(x)
#define DATA_PIN_4(x)             EXYNOS4212_GPM4(x)
#define DATA_PIN_0(x)             EXYNOS4212_GPM0(x)

static char sw_data;

static void gpio_init(void){
	int i;

	//fpga data gpio init - gpm2_0~2
        for(i=0; i<3; i++){
                gpio_request(DATA_PIN_2(i), "GPM2_0");
                s3c_gpio_cfgpin(DATA_PIN_2(i), S3C_GPIO_SFN(1));
                gpio_direction_output(DATA_PIN_2(i), 1);
        }
        //fpga data gpio init - gpm0_0
        for(i=0; i<1; i++){
                gpio_request(DATA_PIN_0(i), "GPM0_0");
                s3c_gpio_cfgpin(DATA_PIN_0(i), S3C_GPIO_SFN(1));
                gpio_direction_output(DATA_PIN_0(i), 1);
        }
        //fpga data gpio init - gpm2_3~4
        for(i=3; i<5; i++){
                gpio_request(DATA_PIN_2(i), "GPM2_0");
                s3c_gpio_cfgpin(DATA_PIN_2(i), S3C_GPIO_SFN(1));
                gpio_direction_output(DATA_PIN_2(i), 1);
        }
        //fpga data gpio init - gpm3_0~7
        for(i=0; i<2; i++){
                gpio_request(DATA_PIN_3(i), "GPM3_0");
                s3c_gpio_cfgpin(DATA_PIN_3(i), S3C_GPIO_SFN(1));
                gpio_direction_output(DATA_PIN_3(i), 1);
        }
     	//fpga address gpio init - gpm0_1~7 <= (0~6+1) //add 1
        for(i=0; i<7; i++){
                gpio_request(ADDR_PIN_L_0(i), "GPM0_0");
                s3c_gpio_cfgpin(ADDR_PIN_L_0(i), S3C_GPIO_SFN(1));
                gpio_direction_output(ADDR_PIN_L_0(i), 0);
                gpio_set_value(ADDR_PIN_L_0(i), 0);
        }
        //fpga address gpio init - gpm1_0~0
        for(i=0; i<1; i++){
                gpio_request(ADDR_PIN_L_1(i), "GPM0_0");
                s3c_gpio_cfgpin(ADDR_PIN_L_1(i), S3C_GPIO_SFN(1));
                gpio_direction_output(ADDR_PIN_L_1(i), 0);
                gpio_set_value(ADDR_PIN_L_1(i), 0);
        }
        //fpga address gpio init - gpm1_1~6
        for(i=0; i<6; i++){
                gpio_request(ADDR_PIN_H_1(i), "GPM1_0");
                s3c_gpio_cfgpin(ADDR_PIN_H_1(i), S3C_GPIO_SFN(1));
                gpio_direction_output(ADDR_PIN_H_1(i), 0);
                gpio_set_value(ADDR_PIN_H_1(i), 0); 
	}
}

static void write_addr_l(unsigned char val){
	int i;
	for(i=0; i<7; i++){
                if(val & (1<<i)){
                        gpio_set_value(ADDR_PIN_L_0(i), 1);
                }
                else{
                        gpio_set_value(ADDR_PIN_L_0(i), 0);
                }
        }
        if(val & (1 << 7))
                gpio_set_value(ADDR_PIN_L_1(0), 1);
        else
                gpio_set_value(ADDR_PIN_L_1(0), 0);
}

static unsigned char DIPSW_DATA(void)
{
	int i;
	unsigned char buf=0;
	for(i=0; i<3; i++){
		sw_data |= ((gpio_get_value(DATA_PIN_2(i)))<<i);
	}       	
		sw_data |= (gpio_get_value(DATA_PIN_0(0)))<<3;
        	sw_data |= ((gpio_get_value(DATA_PIN_2(3)))<<4);
        	sw_data |= ((gpio_get_value(DATA_PIN_2(4)))<<5);
        	sw_data |= ((gpio_get_value(DATA_PIN_3(0)))<<6);
        	sw_data |= ((gpio_get_value(DATA_PIN_3(1)))<<7);

	return buf;
}

static int dip_ioctl(struct inode *inode, struct file *file, unsigned int  cmd, unsigned long arg)
{
	return 0;
}

static int dip_open(struct inode *minode, struct file *mfile)
{
	gpio_init();

	return 0;
}
static ssize_t dip_read (struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	gpio_init();
	write_addr_l(DIP);

	DIPSW_DATA();

	copy_to_user(buf, &sw_data, sizeof(sw_data));
	sw_data = 0;
	return 0;
}

static int dip_release(struct inode *minode, struct file *mfile)
{
	gpio_init();
	return 0;
}

static ssize_t dip_write (struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	return 0;
}

static struct file_operations dip_fops = {
   .write      =   dip_write,
   .read	   =   dip_read,
   .open       =   dip_open,
   .release    =   dip_release,
   .unlocked_ioctl      =   dip_ioctl,
};

static int __init dip_init(void){
	int result;
	struct class *dipsw_class;
        struct device *dev;

	result = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &dip_fops);
	if (result < 0) {
		printk("unable to register device %s\n", DEVICE_NAME);
		return result;
	}
	printk("H-Smart4412TKU IEB: register device %s\n", DEVICE_NAME);

        dipsw_class = class_create(THIS_MODULE, DEVICE_NAME);
        dev = device_create(dipsw_class, NULL, MKDEV(DEVICE_MAJOR,0), NULL, DEVICE_NAME);

	gpio_init();

	return 0;        
}

static void __exit dip_exit(void){
	unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
	gpio_init();
}

module_init(dip_init);
module_exit(dip_exit);

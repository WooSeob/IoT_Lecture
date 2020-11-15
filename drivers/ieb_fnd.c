/*
 * H-Smart4412TKU IEB Board - FND
 *
 * by yoonmk
 *
 */

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

// Device Address
#define LED     0x0A
#define FND     0x0B
#define CLCD    0x0C

#define ADDR_PIN_L_0(x)           EXYNOS4212_GPM0(x+1)
#define ADDR_PIN_L_1(x)           EXYNOS4212_GPM1(x)

#define ADDR_PIN_H_1(x)           EXYNOS4212_GPM1(x+1)

#define DATA_PIN_2(x)             EXYNOS4212_GPM2(x)
#define DATA_PIN_3(x)             EXYNOS4212_GPM3(x)
#define DATA_PIN_4(x)             EXYNOS4212_GPM4(x)
#define DATA_PIN_0(x)             EXYNOS4212_GPM0(x)

#define SEL(x)           ADDR_PIN_H_1(x)

#define DEVICE_NAME 	"fnd"
#define DEVICE_MAJOR    231

#define     MAX_FND  4

static char disp[MAX_FND] = {0};

static int n = 0;

unsigned char FND_DATA_TBL[]={
	0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,
	0x83,0xC6,0xA1,0x86,0x8E,0xC0,0xF9,0xA4,0xB0,0x99,0x89
};

static struct timer_list timer_str;
static void timer_function( unsigned long ptr);
static void init_add_timer(void);
static void remove_timer(void);

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
                gpio_request(DATA_PIN_2(i), "GPM2_3");
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
		gpio_request(ADDR_PIN_L_0(i), "GPM0_1");
	        s3c_gpio_cfgpin(ADDR_PIN_L_0(i), S3C_GPIO_SFN(1));
		gpio_direction_output(ADDR_PIN_L_0(i), 0);
		gpio_set_value(ADDR_PIN_L_0(i), 0);
	}
	//fpga address gpio init - gpm1_0~0
        for(i=0; i<1; i++){
                gpio_request(ADDR_PIN_L_1(i), "GPM1_0");
                s3c_gpio_cfgpin(ADDR_PIN_L_1(i), S3C_GPIO_SFN(1));
                gpio_direction_output(ADDR_PIN_L_1(i), 0);
                gpio_set_value(ADDR_PIN_L_1(i), 0);
        }
	//fpga address gpio init - gpm1_1~6
	for(i=0; i<6; i++){
		gpio_request(ADDR_PIN_H_1(i), "GPM1_1");
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
void FND_DATA(unsigned char val)
{

	if(val & (1<<0))    gpio_set_value(DATA_PIN_2(0), 1);
    	else         	    gpio_set_value(DATA_PIN_2(0), 0);
    	if(val & (1<<1))    gpio_set_value(DATA_PIN_2(1), 1);
    	else                gpio_set_value(DATA_PIN_2(1), 0);
   	if(val & (1<<2))    gpio_set_value(DATA_PIN_2(2), 1);
    	else                gpio_set_value(DATA_PIN_2(2), 0);
    	
	if(val & (1<<3))    gpio_set_value(DATA_PIN_0(0), 1);
    	else                gpio_set_value(DATA_PIN_0(0), 0);
    	
	if(val & (1<<4))    gpio_set_value(DATA_PIN_2(3), 1);
    	else                gpio_set_value(DATA_PIN_2(3), 0);
 	if(val & (1<<5))    gpio_set_value(DATA_PIN_2(4), 1);
    	else                gpio_set_value(DATA_PIN_2(4), 0);
	
	if(val & (1<<6))    gpio_set_value(DATA_PIN_3(0), 1);
        else                gpio_set_value(DATA_PIN_3(0), 0);
	if(val & (1<<7))    gpio_set_value(DATA_PIN_3(1), 1);
        else                gpio_set_value(DATA_PIN_3(1), 0);
}
void FND_SEL(unsigned char val)
{
	int i;
	for(i=0; i<4; i++){
		if(val & (1<<i)){
		    	gpio_set_value(SEL(i), 1);
		}
		else{              
		    	gpio_set_value(SEL(i), 0);
		}
	}
}

void fnd_disp(int num, unsigned char val)
{
	write_addr_l(FND);
	switch(num)
	{
		case 1:
			FND_SEL(~0x01);
			FND_DATA(val);	
			break;
		case 2:
			FND_SEL(~0x02);
			FND_DATA(val);	
			break;
		case 3:
			FND_SEL(~0x04);
			FND_DATA(val);	
			break;
		case 4:
			FND_SEL(~0x08);
			FND_DATA(val);	
			break;
		default :   
			FND_SEL(~0x00);
			break;
	}
}
static void fnd_clear(void)
{
	fnd_disp(1, 0xff);
	fnd_disp(2, 0xff);
	fnd_disp(3, 0xff);
	fnd_disp(4, 0xff);
}

void init_add_timer(void)
{
	init_timer(&timer_str); // timer

	timer_str.function = &timer_function;
	timer_str.data = (unsigned long)n;
	timer_str.expires = jiffies + (HZ/100);

	add_timer(&timer_str);
}

void remove_timer(void)
{
	del_timer(&timer_str);
}

void timer_function(unsigned long ptr)
{
	++n;
	if( n > (MAX_FND))
		n = 1;

	switch(n)
	{
		case  1:    fnd_disp(1, disp[0]);  break;
		case  2:    fnd_disp(2, disp[1]);  break;
		case  3:    fnd_disp(3, disp[2]);  break;
		case  4:    fnd_disp(4, disp[3]);  break;
		default :   break;
	}

	init_add_timer();
}

static int fnd_ioctl(struct inode *inode, struct file *file, unsigned int  cmd, unsigned long arg)
{
	switch(cmd)
	{
		case 1:
			{
				break;
			}
		case 2:
			{
				break;
			}
		
		default:
			return 0;
	}

   return 0;
}

static int fnd_open(struct inode *minode, struct file *mfile)
{
	gpio_init();	

	init_add_timer();

	return 0;
}
static int fnd_release(struct inode *minode, struct file *mfile)
{
	remove_timer();
	gpio_init();

	fnd_clear();
	return 0;
}

static ssize_t fnd_write (struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	write_addr_l(FND);
	memset(disp, 0, 4);
    	copy_from_user(disp, buf, count);
}

static ssize_t fnd_read (struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	return 0;
}

static struct file_operations fnd_fops = {
   .read       =   fnd_read,
   .write      =   fnd_write,
   .open       =   fnd_open,
   .release    =   fnd_release,
   .unlocked_ioctl      =   fnd_ioctl,
};
static int __init fnd_init(void){
	int result;
	struct class *fnd_class;
  	struct device *dev;

	result = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fnd_fops);
	if (result < 0) {
		printk("unable to register device %s\n", DEVICE_NAME);
		return result;
	}
	printk("H-Smart4412TKU IEB: register device %s\n", DEVICE_NAME);
	
	fnd_class = class_create(THIS_MODULE, DEVICE_NAME);
 	dev = device_create(fnd_class, NULL, MKDEV(DEVICE_MAJOR,0), NULL, DEVICE_NAME);
	
	gpio_init();
	return 0;        
}

static void __exit fnd_exit(void){
	unregister_chrdev(DEVICE_MAJOR, DEVICE_NAME);
	remove_timer();
	gpio_init();
}
module_init(fnd_init);
module_exit(fnd_exit);

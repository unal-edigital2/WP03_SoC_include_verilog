#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <irq.h>
#include <uart.h>
#include <console.h>
#include <generated/csr.h>

#include "delay.h"

#define INFRAROJO_1 1
#define INFRAROJO_2 2
#define INFRAROJO_3 4
#define INFRAROJO_4 8



static char *readstr(void)
{
	char c[2];
	static char s[64];
	static int ptr = 0;

	if(readchar_nonblock()) {
		c[0] = readchar();
		c[1] = 0;
		switch(c[0]) {
			case 0x7f:
			case 0x08:
				if(ptr > 0) {
					ptr--;
					putsnonl("\x08 \x08");
				}
				break;
			case 0x07:
				break;
			case '\r':
			case '\n':
				s[ptr] = 0x00;
				putsnonl("\n");
				ptr = 0;
				return s;
			default:
				if(ptr >= (sizeof(s) - 1))
					break;
				putsnonl(c);
				s[ptr] = c[0];
				ptr++;
				break;
		}
	}
	return NULL;
}

static char *get_token(char **str)
{
	char *c, *d;

	c = (char *)strchr(*str, ' ');
	if(c == NULL) {
		d = *str;
		*str = *str+strlen(*str);
		return d;
	}
	*c = 0;
	d = *str;
	*str = c+1;
	return d;
}

static void prompt(void)
{
	printf("RUNTIME>");
}

static void help(void)
{
	puts("Available commands:");
	puts("help                            - this command");
	puts("reboot                          - reboot CPU");
	puts("led                             - led test");
	puts("switch                          - switch test");
	puts("display                         - display test");
	puts("rgbled                          - rgb led test");
	puts("infra                             - infra test");
}

static void reboot(void)
{
	ctrl_reset_write(1);
}

static void display_test(void)
{
//	int i;
	printf("display_test...\n");
/*	for(i=0; i<6; i++) {
		display_sel_write(i);
		display_value_write(7);
		display_write_write(1);
	}
*/
}
static void GPIO_infra_test(void){

	unsigned short valor_infrarrojos;

	valor_infrarrojos=GPIO_infra_in_read();
	printf("el valor de los infrarrojos es : %i \n", valor_infrarrojos);
	if (valor_infrarrojos & INFRAROJO_1)
		printf ("el infrarrojo 1 esta en ON\n");
	else
		printf ("el infrarrojo 1 esta en OFF\n");
	if (valor_infrarrojos & INFRAROJO_2)
		printf ("el infrarrojo 2 esta en ON\n");
	else
		printf ("el infrarrojo 2 esta en OFF\n");
	if (valor_infrarrojos & INFRAROJO_3)
		printf ("el infrarrojo 3 esta en ON\n");
	else
		printf ("el infrarrojo 3 esta en OFF\n");
	if (valor_infrarrojos & INFRAROJO_4)
		printf ("el infrarrojo 4 esta en ON\n");
	else
		printf ("el infrarrojo 4 esta en OFF\n");
	

}
static void led_test(void)
{
	unsigned int i;
//./l		printf("Test del los leds... se interrumpe con el botton 1\n");
	while(!(buttons_in_read()&1)) {

	for(i=1; i<65536; i=i*2) {
		leds_out_write(i);
		delay_ms(50);
	}
	for(i=32768;i>1; i=i/2) {
		leds_out_write(i);
		delay_ms(50);
	}
	}
	
}


static void switch_test(void)
{
	unsigned short temp2 =0;
	printf("Test del los interruptores... se interrumpe con el botton 1\n");
	while(!(buttons_in_read()&1)) {
		unsigned short temp = switchs_in_read();
		if (temp2 != temp){
			printf("switch bus : %i\n", temp);
			leds_out_write(temp);
			temp2 = temp;
		}
	}
}

static void rgbled_test(void)
{
	unsigned int T = 128;
	
	ledRGB_1_r_period_write(T);
	ledRGB_1_g_period_write(T);
	ledRGB_1_b_period_write(T);

	ledRGB_1_r_enable_write(1);
	ledRGB_1_g_enable_write(1);
	ledRGB_1_b_enable_write(1);

	
	
	for (unsigned int j=0; j<100; j++){
		ledRGB_1_g_width_write(j);
		for (unsigned int i=0; i<100; i++){
			ledRGB_1_r_width_write(100-i);
			ledRGB_1_b_width_write(i);	
			delay_ms(20);
		}	
	}
	
}

/*
static void vga_test(void)
{
	int x,y;
	
	for(y=0; y<480; y++) {
		for(x=0; x<640; x++) {
			vga_cntrl_mem_we_write(0);
			vga_cntrl_mem_adr_write(y*640+x);
			if(x<640/3)	
				vga_cntrl_mem_data_w_write(((int)(x/10)%2^(int)(y/10)%2)*15);
			else if(x<2*640/3) 
				vga_cntrl_mem_data_w_write((((int)(x/10)%2^(int)(y/10)%2)*15)<<4);
			else 
				vga_cntrl_mem_data_w_write((((int)(x/10)%2^(int)(y/10)%2)*15)<<8);
			vga_cntrl_mem_we_write(1);
		}
	}
}
*/
static void console_service(void)
{
	char *str;
	char *token;

	str = readstr();
	if(str == NULL) return;
	token = get_token(&str);
	if(strcmp(token, "help") == 0)
		help();
	else if(strcmp(token, "reboot") == 0)
		reboot();
	else if(strcmp(token, "led") == 0)
		led_test();
	else if(strcmp(token, "switch") == 0)
		switch_test();
	else if(strcmp(token, "display") == 0)
		display_test();
	else if(strcmp(token, "rgbled") == 0)
		rgbled_test();
	else if(strcmp(token, "infra") == 0)
		GPIO_infra_test();
/*	else if(strcmp(token, "vga") == 0)
		vga_test();
*/
	prompt();
}

int main(void)
{
	#ifdef CONFIG_CPU_HAS_INTERRUPT
	irq_setmask(0);
	irq_setie(1);
	#endif
	uart_init();

	puts("\nVerilog SoC - RiscV project UNAL 2022-2"__DATE__" "__TIME__"\n");
	help();
	prompt();
	GPIOverilog_cntrl_valor_led_write(1);
	delay_ms(1000);
	GPIOverilog_cntrl_valor_led_write(0);
	delay_ms(1000);
	GPIOverilog_cntrl_valor_led_write(1);
	delay_ms(1000);
	int time=20000;
	PWM_enable_write(1);
	PWM_period_write(time);
	PWM_width_write(time/2);

	while(1) {
		console_service();
		// se le asigna dos les prendidos  y dos apagados al bus 
		GPIOverilog_cntrl_bus_valor_leds_write(0xA);
  		// se lee el bus de sw 
		uint32_t valorsw= GPIOverilog_cntrl_bus_valor_sw_read();
		printf("switch bus verilog: %i\n", valorsw);

	/*	leds_out_write(15);
		delay_ms(100);
		leds_out_write(240);
		delay_ms(100);
		printf("El programa \n");*/
	}

	return 0;
}

#!/usr/bin/env python3

from migen import *
from migen.genlib.io import CRG
from migen.genlib.cdc import MultiReg

## debe dejar solo una tarjeta
# import tarjetas.digilent_nexys4 as tarjeta # si usa tarjeta nexy 4 
import tarjetas.nexys4ddr as tarjeta # si usa tarjeta nexy 4 4DRR
# import tarjetas.digilent_zybo_z7 as tarjeta # si usa tarjeta zybo z7
# import tarjetas.c4e6e10 as tarjeta

from litex.soc.integration.soc_core import *
from litex.soc.integration.builder import *
from litex.soc.interconnect.csr import *

from litex.soc.cores import gpio
from litex.soc.cores import pwm
from module import rgbled
from module import vgacontroller
from module.display import SevenSegmentDisplay
from module import module_verilog

# BaseSoC ------------------------------------------------------------------------------------------

class BaseSoC(SoCCore):
	def __init__(self):
		platform = tarjeta.Platform()
		sys_clk_freq = int(1e9/platform.default_clk_period)
		
		## add source verilog

		platform.add_source("module/verilog/GPIOv.v")


		# SoC with CPU
		SoCCore.__init__(self, platform,
# 			cpu_type="picorv32",
			cpu_type="vexriscv",
			clk_freq=1e9/platform.default_clk_period, 
			integrated_rom_size=0x8000,
			integrated_sram_size=0x1000,
			integrated_main_ram_size=20*1024)

		# Clock Reset Generation
		self.submodules.crg = CRG(platform.request("clk"), ~platform.request("cpu_reset"))

		# Leds
		SoCCore.add_csr(self,"leds")
		user_leds = Cat(*[platform.request("led", i) for i in range(3)])
		self.submodules.leds = gpio.GPIOOut(user_leds)
		
		# Switchs
		SoCCore.add_csr(self,"switchs")
		user_switchs = Cat(*[platform.request("sw", i) for i in range(4)])
		self.submodules.switchs = gpio.GPIOIn(user_switchs)
		
		# Buttons 

		SoCCore.add_csr(self,"buttons")
		user_buttons = Cat(*[platform.request("btn%c" %c) for c in ['c','d','u' ]])
		self.submodules.buttons = gpio.GPIOIn(user_buttons)
		

		# RGB leds
		SoCCore.add_csr(self,"ledRGB_1")
		self.submodules.ledRGB_1 = rgbled.RGBLed(platform.request("ledRGB",1))
		
		SoCCore.add_csr(self,"GPIO_infra")
		pads_infra = Cat(*[platform.request("infrarrojo", i) for i in range(3)])
		self.submodules.GPIO_infra = gpio.GPIOIn(pads_infra)


		SoCCore.add_csr(self,"PWM")
		self.submodules.PWM = pwm.PWM(platform.request("pwm__",1))

		
		# 7segments Display para zybo z7 comentar 
  
#		self.submodules.display = SevenSegmentDisplay(sys_clk_freq)
#		self.add_csr("display")
#		self.comb += [
#          platform.request("display_cs_n").eq(~self.display.cs),
#           platform.request("display_abcdefg").eq(~self.display.abcdefg)
#   	]				
		
		#add driver in verilog
		SoCCore.add_csr(self,"GPIOverilog_cntrl")
		self.submodules.GPIOverilog_cntrl = module_verilog.GPIOverilog(platform.request("mod_pin_verilog",1))
	#	SoCCore.add_interrupt(self,"GPIOverilog_cntrl")
#		self.submodules.GPIOverilog_cntrl = module_verilog.GPIOverilog(platform.request("cam_xclk"),platform.request("cam_pclk"),cam_data_in)

# Build --------------------------------------------------------------------------------------------
if __name__ == "__main__":
	builder = Builder(BaseSoC(),output_dir="build", csr_csv="csr.csv")
	builder.build(build_name="top")



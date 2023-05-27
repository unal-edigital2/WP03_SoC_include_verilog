from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

# Modulo Principal
class GPIOverilog(Module,AutoCSR):
    def __init__(self, pads):
        self.clk = ClockSignal()   
        self.rst = ResetSignal()  
        self.xclk = pads.xclk
        self.salida_led=pads.salida_led

        self.bus_leds=pads.bus_leds
        self.bus_sw=pads.bus_sw

        self.done= CSRStatus()
        self.valor_led= CSRStorage()
        
        self.bus_valor_sw= CSRStatus(4)
        self.bus_valor_leds= CSRStorage(4)

        self.specials +=Instance("GPIOv",
            i_clk = self.clk,
            i_rst = self.rst,
            o_xclk = self.xclk,
            o_salida_led =self.salida_led,
            i_valor_led =self.valor_led.storage,
            o_done =self.done.status,
            o_bus_leds= self.bus_leds,
            i_bus_valor_leds=self.bus_valor_leds.storage,
            i_bus_sw= self.bus_sw,
            o_bus_valor_sw=self.bus_valor_sw.status
            


        )
        
        
       
        self.submodules.ev = EventManager()
        self.ev.ok = EventSourceProcess()
        self.ev.finalize()
 
        self.ev.ok.trigger.eq(self.done.status)

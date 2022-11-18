from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

# Modulo Principal
class GPIOverilog(Module,AutoCSR):
    def __init__(self, data):
        self.clk = ClockSignal()   
        self.rst = ResetSignal()  
        self.xclk = data.xclk
        self.salida_led=data.salida_led

        self.done= CSRStatus()
        self.valor_led= CSRStorage()
        
        self.specials +=Instance("GPIOv",
            i_clk = self.clk,
            i_rst = self.rst,
            o_xclk = self.xclk,
            o_salida_led =self.salida_led,
           i_valor_led =self.valor_led.storage,
            o_done =self.done.status,
        )
        
        
       
        self.submodules.ev = EventManager()
        self.ev.ok = EventSourceProcess()
        self.ev.finalize()
 
        self.ev.ok.trigger.eq(self.done.status)

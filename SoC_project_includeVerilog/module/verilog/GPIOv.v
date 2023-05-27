`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:14:22 12/02/2019 
// Design Name: 
// Module Name:   GPIOverilog 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module GPIOv (
		input clk,
		input rst,
		output xclk,			// ejemplo de reloj de salida
		output done,			// usado para la interrupción
		output [3:0] bus_leds,			// ejemplo de bus de salida 
		input  [3:0] bus_valor_leds,	// ejemplo de bus de entrada 
		input  [3:0] bus_sw,
		output  [3:0] bus_valor_sw,
		
		input valor_led,				// ejemplo entrada 
		output salida_led
		/* incluir las señales necesarias*/
   );
	

/******************************************************************
HDL EJEMPLO  SE DEBE MODIFICAR 
******************************************************************/
	
	reg [26:0] cfreq=0;
	always @(posedge clk) begin
  		if(rst==1) begin
			cfreq <= 0;
		end else begin
			cfreq <=cfreq+1;
		end
	end
	assign xclk = cfreq[25];
	assign salida_led =valor_led;
	assign bus_leds=bus_valor_leds;
	assign bus_valor_sw=bus_sw;
//	assign bus_leds=bus_sw;

	assign done = xclk;

endmodule

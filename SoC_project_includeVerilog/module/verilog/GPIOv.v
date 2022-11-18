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
		output done,
		input valor_led,			// ejemplo dato que llega al bloque
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
	assign done = cfreq[25];;
	assign salida_led =valor_led;


endmodule

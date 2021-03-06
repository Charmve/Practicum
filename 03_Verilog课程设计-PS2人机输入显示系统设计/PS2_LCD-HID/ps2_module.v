module Ps2_Module
(
	CLK_20M,RST_N,PS2_CLK,PS2_DATA,
	o_ps2_data
);


//---------------------------------------------------------------------------
input	CLK_20M;				
input	RST_N;				
input	PS2_CLK;				
input	PS2_DATA;			
output reg	[15:0] 	o_ps2_data;			


//---------------------------------------------------------------------------
reg			[ 1:0]	detect_edge;		
wire		[ 1:0]	detect_edge_n;		
reg			[ 3:0]	bit_cnt;			
reg			[ 3:0]	bit_cnt_n;			
reg			[10:0]	bit_shift;		
reg			[10:0]	bit_shift_n;	
reg			[39:0]	data_shift;			
reg			[39:0]	data_shift_n;		//data_shift
reg			[15:0]	o_ps2_data_n;		//o_ps2_data
reg			negedge_reg;	
wire		negedge_reg_n;		//negedge_reg

//---------------------------------------------------------------------------
 
always @ (posedge CLK_20M or negedge RST_N)
begin
	if(!RST_N)									 
		detect_edge	<= 2'b11;			 
	else
		detect_edge <= detect_edge_n;		 
end

 
assign detect_edge_n = {detect_edge[0] , PS2_CLK};	 
 
always @ (posedge CLK_20M or negedge RST_N)
begin
	if(!RST_N)									 
		negedge_reg	<= 1'b0;					 
	else
		negedge_reg <= negedge_reg_n;		 
end


assign negedge_reg_n = (detect_edge == 2'b10) ? 1'b1 : 1'b0; 


always @ (posedge CLK_20M or negedge RST_N)
begin
	if(!RST_N)									
		bit_cnt <= 4'b0;						
	else
		bit_cnt <= bit_cnt_n;				
end


always @ (*)
begin
	if(bit_cnt == 4'd11)						
		bit_cnt_n = 4'b0;						
	else if(negedge_reg)						
		bit_cnt_n = bit_cnt + 4'b1;		
	else							
		bit_cnt_n = bit_cnt;					 
end


always @ (posedge CLK_20M or negedge RST_N)
begin
	if(!RST_N)									
		bit_shift <= 11'b0;					
	else
		bit_shift <= bit_shift_n;			
end


always @ (*)
begin
	if(negedge_reg)							
		bit_shift_n = {PS2_DATA , bit_shift[10:1]};	
	else
		bit_shift_n = bit_shift;			
end


always @ (posedge CLK_20M or negedge RST_N)
begin
	if(!RST_N)									
		data_shift <= 40'b0;					
	else
		data_shift <= data_shift_n;		
end

always @ (*)
begin
	if(bit_cnt == 4'd11)						
		data_shift_n = {data_shift[31:0] , bit_shift[8:1]}; 
	else
		data_shift_n = data_shift;			
end


always @ (posedge CLK_20M or negedge RST_N)
begin
	if(!RST_N)									
		o_ps2_data <= 16'b0;					
	else
		o_ps2_data <= o_ps2_data_n;		
end


always @ (*)
begin
	if((data_shift_n[15:8] == 8'hF0) && (data_shift_n[23:16] == 8'hE0)) 
		o_ps2_data_n = {8'hE0 , data_shift_n[7:0]}; 
	else if((data_shift_n[15:8] == 8'hF0) && (data_shift_n[23:16] != 8'hE0))  
		o_ps2_data_n = {8'h0, data_shift_n[7:0]};	 
	else
		o_ps2_data_n = o_ps2_data;		 
end

endmodule



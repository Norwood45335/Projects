module Lab9(SW, LEDR,  CLOCK_50 ,KEY, HEX3, HEX2, HEX1 ,HEX0);
  input	CLOCK_50;			// input Clock (50 MHz)	
  input  [5:0] SW;			// Counter Max. Value
  input  [1:0] KEY;			// KEY[1]: Reset, KEY[0]: Pause/Resume 

  output [2:0] LEDR;			// LEDR[2:1]: your status (for debugging), LEDR[0]: 1Hz Clock
  output [6:0] HEX3,HEX2; 	// Max counter value (Two 7-segment display) HEX3: tens position, HEX2: ones position
  output [6:0] HEX1,HEX0;	// Current counter value (Two 7-segment display) HEX1: tens position, HEX0: ones position
  
  reg CLK_1Hz;					// generated 1Hz Clock signal
  wire RST;						// active high reset signal
  wire PR;						// active high pause/resume signal
  wire cnt_en;					// counter enable signal
  wire [5:0] max_CNT;		// maximum counter value (from user input - switch)
  wire [5:0] CNT_out;		// Current counter value (6-bit)
  
  wire [3:0] max_tens_digit;	// BCD coded tens position digit of the max counter value
  wire [3:0] max_ones_digit;	// BCD coded ones position digit of the max counter value
  wire [3:0] cur_tens_digit;	// BCD coded tens position digit of the current counter value
  wire [3:0] cur_ones_digit;	// BCD coded ones position digit of the current counter value
  
  assign RST = ~KEY[1];	// reset (active-high)
  assign PR = ~KEY[0];	// pause/resume (active-high)
  
  ///////////////////////////////////////////////////////////////////////
  // 1Hz clock generation (DO NOT TOUCH)
  // period of clock in is 1/(50MHz) = 0.02 * 10^-6
  // period of clock out is 1/1Hz = 1
  integer clk_count = 0;
  parameter th = 50000000;	// complete the code with your calculated th value
  parameter half_th = th/2;
  always@(posedge CLOCK_50 or posedge RST)
  begin
    if (RST == 1)
		begin
			clk_count = 0;
			CLK_1Hz = 0;
		end 
	 else
	   if(CLOCK_50 == 1)
			if(clk_count < half_th)
				begin
					CLK_1Hz = 1;
					clk_count = clk_count + 1;
				end
			else if(clk_count < th-1)
				begin
					CLK_1Hz = 0;
					clk_count = clk_count + 1;
				end
			else if(clk_count == th-1)
				begin
					CLK_1Hz = 0;
					clk_count = 0;
				end
  end
  
  assign LEDR[0] = CLK_1Hz;	// 1Hz clock (approximate)
  // end of clock generation 
  ///////////////////////////////////////////////////////////////////////

  
  //////////////////////////////////////////////////////////////////////  
  // A 6-bit Up Counter - DO NOT TOUCH
  UpCounter_6bit	u1 (CLK_1Hz, RST, cnt_en, CNT_out);
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // FSM (Mealy machine) - NEED TO COMPLETE THIS PART
  assign max_CNT = SW[5:0];
  // instantiate your FSM here
  lab9_fsm		u_fsm (CLK_1Hz, RST, PR, CNT_out, max_CNT, cnt_en, LEDR[2:1]); // uncomment this line
  //assign cnt_en = 1'b1; // comment out this line aftter completting lab9_fsm.value
  ///////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////
  // display the counter value - DO NOT TOUCH
  BCD_encoder_2_digit	u2(max_CNT,max_tens_digit,max_ones_digit);
  bin_7seg			u3 (max_tens_digit,HEX3);
  bin_7seg			u4 (max_ones_digit,HEX2);
  BCD_encoder_2_digit	u5(CNT_out,cur_tens_digit,cur_ones_digit);
  bin_7seg			u6 (cur_tens_digit,HEX1);
  bin_7seg			u7 (cur_ones_digit,HEX0);
  ///////////////////////////////////////////////////////////////////////

endmodule


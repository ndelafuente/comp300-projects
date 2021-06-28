////////
// 
//	p4starter/firmware.c
//
//	Project 4 Starter
//	
//	Hardware multiplexed 7-segment display 
//  on the breadboard using this C program on a 
//  PICOSOC RISC-V processor instantiated on the FPGA.
//
//  P3 solution with modified gpio output:
//       reg_gpio =  ((second_timer & 0xFFFF) << 0); // debug LEDs
//
//  The second count is provided as a 16 bit binary number
//  in 4 hex digits.  Note that 4 decimal digits could be
//  sent to the display by sending 4 BCD digits instead of hex.
//
//  reg_gpio = ...      //GPIO output to display circuit
//  var = reg_gpio ...  // input from display cirtuit GPIO to program
//
//	Chuck Pateros
//	University of San Diego
//	26-Jun-2021
//
////////

#include <stdint.h>
#include <stdbool.h>


// a pointer to this is a null pointer, but the compiler does not
// know that because "sram" is a linker symbol from sections.lds.
extern uint32_t sram;

#define reg_spictrl (*(volatile uint32_t*)0x02000000)
#define reg_uart_clkdiv (*(volatile uint32_t*)0x02000004)
#define reg_uart_data (*(volatile uint32_t*)0x02000008)
#define reg_gpio (*(volatile uint32_t*)0x03000000)

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss,_heap_start;

uint32_t set_irq_mask(uint32_t mask); asm (
    ".global set_irq_mask\n"
    "set_irq_mask:\n"
    ".word 0x0605650b\n"
    "ret\n"
);

void spin_wait_ms(int wait_ms){
    int cycles_to_wait = 25 * wait_ms;
    while (cycles_to_wait >= 0)
      cycles_to_wait -= 1;
  }

int segment_decode(int hex){

  switch(hex){
    case 0x00 : return 0b0111111;
    case 0x01 : return 0b0110000;
    case 0x02 : return 0b1011011;
    case 0x03 : return 0b1111001;
    case 0x04 : return 0b1110100;
    case 0x05 : return 0b1101101;
    case 0x06 : return 0b1101111;
    case 0x07 : return 0b0111000;
    case 0x08 : return 0b1111111;
    case 0x09 : return 0b1111100;
    case 0x0A : return 0b1111110;
    case 0x0B : return 0b1100111;
    case 0x0C : return 0b0001111;
    case 0x0D : return 0b1110011;
    case 0x0E : return 0b1001111;
    case 0x0F : return 0b1001110;
    default   : return 0b0000000;
  } // end switch

} //end function segment_decode

void main() {
    set_irq_mask(0xff);

    // zero out .bss section
    for (uint32_t *dest = &_sbss; dest < &_ebss;) {
        *dest++ = 0;
    }

    // switch to dual IO mode
    reg_spictrl = (reg_spictrl & ~0x007F0000) | 0x00400000;

    uint32_t led_timer = 0;
    uint32_t second_timer = 0;
    uint32_t ms_timer = 0;
    uint32_t display_digit = 0;
    uint32_t comm = 0b1110;
    uint32_t segments = 0b1111111;
    uint32_t hex_to_display = 0x0;
    uint32_t second_toggle = 0;
    uint32_t new_second_toggle = 0;
    

    while (1) {
      spin_wait_ms(1);
      ms_timer += 1;
      segments = segment_decode(hex_to_display);

      // read values from hardware
      //      assign read_data = ((second_toggle & 32'b1)); // from top.v
      new_second_toggle = reg_gpio & 0x1;



      // Debug LED outputs tied to to GPIO lines (in Verilog top.v)
      //    assign DBG = gpio[3:0];

      reg_gpio =  ((second_timer & 0xFFFF) << 0); // debug LEDs in 4 LSBs

        // do display updates here
        switch(display_digit) {
            case 0 : {
              display_digit = 1;
              comm = 0b0111;
              hex_to_display = second_timer & 0x0F;
              break;
            }
            case 1 : {
              display_digit = 2;
              comm = 0b1011;
              hex_to_display = (second_timer >> 4) & 0x0F;
              break;
            }
            case 2 : {
              display_digit = 3;
              comm = 0b1101;
              hex_to_display = (second_timer >> 8) & 0x0F;
              break;
            }
            case 3 : {
              display_digit = 0;
              comm = 0b1110;
              hex_to_display = (second_timer >> 12) & 0x0F;
              break;
          }
            default : {
              display_digit = 0;
              comm = 0b1111;
              break;
          }
        } // end switch

      if(ms_timer > 999){
      // get here once a second
      ms_timer = 0;
      second_timer += 1;
    } // end of if
  } // end of while(1)
} // end of main program

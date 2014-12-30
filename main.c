/* ===================================================
======================================================
======== WYKORZYSTANIE TECHNIKI BIT-BANGING ==========
============== DO ODBIORU KODU RC5 ===================
======================================================
================================ by: Krzysztof Maicher
======================================================
=================================================== */

#include <stdio.h>
#include <io430x42x.h>
#include <intrinsics.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned char RC5_address, RC5_data;
bool t_flag = 0, RC5_error = 0, RC5_error2 = 0;

#include "LCD_lib.c"
#include "timer.c"
#include "rc5_lib.c"

/* ===================================================
======================================================
==================== M A I N =========================
======================================================
=================================================== */

void main (void)
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  P1DIR = 0xFF;   // wszystkie linie P1 wyjsciowe
  P1OUT = 0xFF;   // wstepne ustawienie lini
  RC5_init(); 
  lcd_init();
  lcd_clear();
  
  __low_power_mode_3();  // wejdz w stan uspienia
  
  while (1)  // petla glowna
  {
    if (RC5_data != 0)
    {
      LCDCTL_bit.LCDON = 1;  //wlacz LCD 
      lcd_put_byte (RC5_data);  // wyswietl odebrane rozkaz
      //P1IFG = 0;  // czysc flagi
      RC5_data = 0;
      __low_power_mode_3();  // wejdz w stan uspienia
    }
    delay (0xFFFF);  // czekaj
    LCDCTL_bit.LCDON = 0;  //wylacz LCD   
  }
}

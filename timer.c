/* ===================================================
==================== T I M E R   A ===================
====================================================*/


/* opoznienie nie uzywajace przerwania o 'ticks' * 30.518us
*/
void delay (unsigned int ticks)
{
  TACTL_bit.TACLR = 1;  //kasowanie 
  TACCR0 = ticks;  // ustaw ilosc taktow
  TACTL_bit.TASSEL = 1;  //zrodlo taktowania ACLK
  TACTL_bit.TAMC = 1;  // start, zliczanie do wartosci TACCR0
  
  while (TACTL_bit.TAIFG == 0) {} //czekaj az zliczy do konca
  
  TACTL_bit.TAMC = 0; //stop
  TACTL_bit. TAIFG; // kasowanie flagi
}
 


/* procedura obslugi przerwania TIMERa A
*/
#pragma vector = TIMERA1_VECTOR  // wektor przerwania dla TIMERa A
__interrupt void timerA_interrupt(void)
{
  TACTL &= ~(TAIFG | TAIE); // czysc flage, wylacz przerwanie timera
  if (RC5_error == 1)    
    RC5_error2 = 1;  //blad
  else
    t_flag = 1;
    
}

/* wlacz TIMER A
*/
void timerA_start (void)
{
  RC5_error = 0; 
  TACTL_bit.TACLR = 1;  //kasowanie  
  TACCR0 = 34;

  TACTL_bit.TASSEL = 1;  // zrodlo taktowania ACLK
  TACTL_bit.TAMC = 1;  // start, zliczanie do wartosci TACCR0
  TACTL_bit.TAIE = 1; // wlcz prerwanie timera
}

/* wlacz TIMER A, oczekuj na error
*/
void timerA_start_error (void)
{
  RC5_error = 1;
  TACTL_bit.TACLR = 1;  //kasowanie  
  TACCR0 = 60;
  TACTL_bit.TASSEL = 1;
  TACTL_bit.TAMC = 1;
  TACTL_bit.TAIE = 1; //zrodlo taktowania ACLK, start, zliczanie do wartosci TACCR0, wlcz prerwanie timera
}


/* czekaj na zgloszenie przerwania
*/
void timerA_wait (void)
{
  while (t_flag == 0) {} //czekaj az zliczy do konca
  t_flag = 0;
  TACTL_bit.TAMC = 0;  // timer stop 
}



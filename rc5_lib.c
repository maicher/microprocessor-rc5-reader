/* ===================================================
====================== R C   5 =======================
====================================================*/

#define RC5_line 0x10  // linia wejscia sygnalu RC5

/* inicjalizacja lini odbiornika RC5 i ustawienie przerwania
*/
void RC5_init (void)
{
  P1DIR &= ~RC5_line;   // ustawienie linii RC5 jako wejscia
  P1IES |= RC5_line;   // reakcja na zbocze opadajace
  P1OUT &= ~RC5_line;  // 
  P1IFG = 0;  // czysc flagi
  P1IE |= RC5_line;   // przerwanie wlaczone
  P1IFG = 0;  // czysc flagi
}

/*  koniec obslugi linii RC5
*/
void RC5_end (void)
{
  if (RC5_error == 1)
  {
    RC5_data = 0;
    RC5_address = 0;
    RC5_error = 0;
    RC5_error2 = 0;
  }
 // delay (0x1000);
  P1IFG = 0;  // czysc flagi
  TACCTL0 = 0;
  TACCTL1 = 0;
  TACCTL2 = 0;
  __disable_interrupt();
  P1IE |= RC5_line;   // przerwanie wlaczone 
  P1IFG = 0;  // czysc flagi
  TACTL_bit.TAIE = 0; //przerwanie z timera wyl
  TACTL_bit.TAIFG = 1;  // ustawienie flagi timera
  TACTL_bit.TACLR = 1;  //kasowanie 
  TACCR0 = 0xFFFF;
  __enable_interrupt();
}









/* oczekiwanie na zmiane bitu na lini RC5, odbior bitu i zapisanie go do zmiennej 
'byte' na miejscu  okreslonym przez 'pos', zwraca zmodyfikowana zmienna 'byte'
*/
unsigned char get_RC5_bit (unsigned char byte, unsigned char pos)
{
  timerA_start_error(); // wlacz timer i zglos przerwanie jesli zmiana bitu nie nastapi
  if ((P1IN & RC5_line) == 0) // jesli linia jest w stanie niskim
    while ((P1IN & RC5_line) == 0) {if (RC5_error2 == 1) break;} // czekaj na zmiane stanu na wysoki
  else  // jesli linia jest w stanie wysokim
  {
    while ((P1IN & RC5_line) != 0) {if (RC5_error2 == 1) break;} // czekaj na zmiane stanu na niski
    byte |= (1 << pos);  // wstaw 1 na odpowiednim miejscu
  } 
  return byte;
}


/* odbior RC5
*/
#pragma vector = PORT1_VECTOR  // wektor przerwania dla P1
__interrupt void RC5_interrupt(void)
{
  ////////////////////////////////////////////////////////////// bity startu ///
  
  __low_power_mode_off_on_exit(); 
  // jestesmy na poczatku S1
  timerA_start(); // wlacz timer
  unsigned char i; // licznik petli
  P1IFG = 0;  // czysc flagi
  P1IE &= ~RC5_line;   // przerwanie wylaczone
  __enable_interrupt(); //wlacz przerwaniem, dla timera
  timerA_wait(); //czekaj 0.75 czasu trwania bitu
  // jestesmy w 1/4 S2
  get_RC5_bit (0,0);  // czekamy 2 na bit startu
  if (RC5_error2 == 1) goto end;
  
  // jestesmy w 1/2 S2
  //////////////////////////////////////////////////////////////////// bit T ///
  timerA_start(); //wlacz timer
  timerA_wait(); //czekaj 0.75 czasu trwania bitu
  // jestesmy w 1/4 T
  get_RC5_bit (0,0);  // ignorujemy bit T
  // jestesmy w 1/2 T
  if (RC5_error2 == 1) goto end;
  ////////////////////////////////////////////////////////////// bity adresu ///
  timerA_start(); //wlacz timer
  i = 5;  // 5 bitow adresu
  RC5_address = 0;
  while (i--)
  {
    timerA_wait(); //czekaj 0.75 czasu trwania bitu
    // jestesmy w 1/4 kolejnego bitu address
    RC5_address = get_RC5_bit (RC5_address, i);  // odbior bitu
    if (RC5_error2 == 1) goto end;
    // jestesmy w 1/2 kolejnego bitu address
    timerA_start(); //wlacz timer
  }
  ////////////////////////////////////////////////////////////// bity danych ///
  i = 6;  // 6 bitow danych
  RC5_data = 0;
  while (i--)
  {
    timerA_wait(); //czekaj 0.75 czasu trwania bitu
    // jestesmy w 1/4 kolejnego bitu data
    RC5_data = get_RC5_bit (RC5_data, i);  // odbior bitu
    if (RC5_error2 == 1) goto end;
    // jestesmy w 1/2 kolejnego bitu data
    timerA_start(); //wlacz timer
  } 
end:
  RC5_end ();
}

/* ===================================================
======================================================
=========== ZBIOR FUNKCJI DO OBSLUGI LCD =============
======================================================
================================ by: Krzysztof Maicher
======================================================
=================================================== */


#define d   0x0001
#define e   0x0002
#define f   0x0004
#define x   0x0008
#define m   0x0010
#define n   0x0020
#define g1  0x0040
#define h   0x0080
#define dp  0x0100
#define c   0x0200
#define b   0x0400
#define a   0x0800
#define l   0x1000
#define g2  0x2000
#define k   0x4000
#define j   0x8000

/* talbica znakow wyswietlacza 7-segmentowego
*/
static unsigned int LCD[] = {a|b|c|d|e|f, b|c, a|b|g2|g1|e|d, a|b|c|d|g2,  //0,1,2,3
                             f|g1|g2|c, a|f|g1|g2|c|d, a|f|e|d|c|g2|g1, a|b|c,  //4,5,6,7
                             a|b|c|d|e|f|g1|g2, a|b|c|d|f|g1|g2, 0};  //8,9, NULL
 
/* inicjalizacja wyswietlacza
*/
void lcd_init (void)
{
  BTCTL = 0x0000;
  LCDCTL |= 0x7F; //S0-S23
  LCDCTL_bit.LCDMX0 = 3; // 4-mux
  LCDCTL_bit.LCDSON = 1;  //wlacz LCD 
}



/* wyswietla cyfre 'digit' na pozycji okreslonej przez 'pos'
*/
void put_1_digit (unsigned char pos, unsigned char digit)
{
  switch (pos)
  {
    case 1: LCDM1 = (LCD[digit]);
            LCDM2 = (LCD[digit] >> 8); break;
    case 2: LCDM3 = (LCD[digit]);
            LCDM4 = (LCD[digit] >> 8); break;
    case 3: LCDM5 = (LCD[digit]);
            LCDM6 = (LCD[digit] >> 8); break;
    case 4: LCDM7 = (LCD[digit]);
            LCDM8 = (LCD[digit] >> 8); break;   
    case 5: LCDM9 = (LCD[digit]);
            LCDM10 = (LCD[digit] >> 8); break;
    case 6: LCDM11 = (LCD[digit]);
            LCDM12 = (LCD[digit] >> 8); break;   
    case 7: LCDM13 = (LCD[digit]);
            LCDM14 = (LCD[digit] >> 8); break;
  }
}

/* wyswietla typ unsigned char 'byte' na wyswietlaczu
*/
void lcd_put_byte (unsigned char byte)
{
  unsigned char lcd_buf[2];  // tablica zmiennych tymczasowych
  lcd_buf[0] = byte / 100;  // cyfra setek
  lcd_buf[1] = ((byte - (lcd_buf[0]*100))/10);  // cyfra jednosci
  if (lcd_buf[0] != 0)  // jesli nie ma setek, nie wyswietlaj zera
    put_1_digit (4, lcd_buf[0]);  // wyswietl setki
  else 
    put_1_digit (4, 10);
  if (lcd_buf[0] != 0 || lcd_buf[1] != 0)  //jesli nie ma dziesiatek, niz wyswietlaj zera
    put_1_digit (5, lcd_buf[1]);  // wyswietl dziesiatki
  else 
    put_1_digit (5, 10);
  put_1_digit (6, (byte % 10));  // wyswietl jednosci
}

/*  czyszczenie LCD poprzez wpisanie zer
*/
void lcd_clear (void)
{
  unsigned char coun;
  for (coun = 0; coun <=7; coun++)
    put_1_digit (coun, 10);
}

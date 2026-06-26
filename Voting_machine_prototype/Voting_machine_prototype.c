#include <msp430.h>
#include <inttypes.h>
// Defining Switch and LED connections
#define SW1 BIT0
#define SW2 BIT3
#define SW3 BIT7
#define LED1 BIT4
#define LED2 BIT5

//Defining the UART connections
#define RX BIT1
#define TX BIT2

#define lcd_setcgramaddr 0x40
// Defining LCD connections
#define CMD 0
#define DATA 1
#define RS BIT2
#define EN BIT3
#define D4 BIT4
#define D5 BIT5
#define D6 BIT6
#define D7 BIT7

uint16_t totalvotes = 0;
uint16_t bjpVotes = 0;
uint16_t congressVotes = 0;
uint16_t aapVotes = 0;
uint16_t kjpVotes = 0;
uint16_t notaVotes = 0;

// Writing delay function
void delay(uint16_t t){
    uint16_t i;
    for(i=t;i>0;i--){
        __delay_cycles(100);
    }
}
// Writing Pulse enable function
void pulseen(){
    P2OUT |= EN;
    delay(1);
    P2OUT &= ~EN;
    delay(1);
}
// Function to write data/command to lcd
void lcd_write(uint8_t value, uint8_t mode){
    if(mode == CMD){
        P2OUT &= ~RS;
    }
    else{
        P2OUT |= RS;
    }
    P2OUT = ((P2OUT & 0x0F)|(value & 0xF0));
    pulseen();
    delay(1);
    
    P2OUT = ((P2OUT & 0X0F)| ((value << 4) & 0XF0));
    pulseen();
    delay(1);
}
//Function to write a string
void lcd_print(const char *s)
{
    while(*s){
        lcd_write(*s,DATA);
        s++;
    }
}
// function to move cursor
void set_cursor(uint8_t row,uint8_t col){
    const uint8_t row_offsets[] = {0x00,0x40};
    lcd_write(0x80|(col + row_offsets[row]), CMD);
    delay(1);
}
// Initialize LCD
void lcd_init(){
    P2DIR |= (D4 | D5 | D6 | D7 | RS | EN);
    P2OUT &= ~(D4 | D5 | D6 | D7 | RS | EN);
    
    delay(150);
    lcd_write(0x33, CMD);
    delay(50);
    lcd_write(0x32, CMD);
    delay(1);
    lcd_write(0x28, CMD);
    delay(1);
    lcd_write(0x0C, CMD);
    delay(1);
    lcd_write(0x01, CMD);
    delay(1);
    lcd_write(0x06, CMD);
    delay(1);
    set_cursor(0,0);    
}
// Defining Party 1 symbol
uint8_t Lotus[8] = {0x04,0x0E,0x1F,0x1B,0x0A,0x04,0x04,0x00};
uint8_t Hand[8]  = {0x04,0x0E,0x1E,0x1F,0x0A,0x08,0x08,0x00};
uint8_t Broom[8] = {0x02,0x06,0x0C,0x18,0x1F,0x13,0x0D,0x00};
uint8_t Star[8]  = {0x04,0x0E,0x1F,0x0E,0x1F,0x0A,0x04,0x00};
uint8_t Nota[8]  = {0x11,0x0A,0x04,0x0A,0x04,0x11,0x00,0x00};
// Character creating function
void lcd_createchar(uint8_t location,uint8_t charmap[]){
    location &= 0x07;
    lcd_write(lcd_setcgramaddr|(location << 3),CMD);
    uint8_t i = 0;
    for(i=0;i<8;i++){
        lcd_write(charmap[i],DATA);
    }
}
// GPIO configurations
void register_settings_for_gpio(){
    P2SEL &= ~(BIT6 | BIT7);
    P2SEL2 &= ~(BIT6 | BIT7);
    P1DIR &= ~(SW1 | SW2 | SW3);
    P1DIR |= (LED1 | LED2);
    P1OUT |= (LED1);
    P1REN |= (SW1 | SW2 | SW3);
    P1OUT |= (SW1 | SW2 | SW3);
    
}
// Function to display the start phase of the machine
void display_startup(){
        lcd_write(0x01, CMD);
        delay(1000);
        
        set_cursor(0,0);
        lcd_print("VOTING MACHINE");
        delay(10000);

        lcd_write(0x01, CMD);
        delay(1000);
        
        set_cursor(0,0);
        lcd_print("PRESS SW2 TO");
        set_cursor(1,0);
        lcd_print("SHOW PARTY LIST");
}
// Create Character Symbols for every Party
void create_characters()
{
    lcd_createchar(0, Lotus);
    lcd_createchar(1, Hand);
    lcd_createchar(2, Broom);
    lcd_createchar(3, Star);
    lcd_createchar(4, Nota);
}

void show_party(uint8_t value,const char name[]){
                 
         set_cursor(0,0);
         lcd_print("SELECT Party :");
         set_cursor(1,0);
         lcd_write(value,DATA);
         lcd_print(name);
}

// Display party function

void display_party()
{   uint8_t currentparty = 1;
    lcd_write(0x01, CMD);
    delay(100);
    while(1){
        switch(currentparty){
            case 1:
            show_party(0,"BJP     ");
            break;
            case 2: 
            show_party(1,"CONGRESS");
            break;
            case 3:
            show_party(2,"AAP     ");
            break;
            case 4:
            show_party(3, "KJP    ");
            break;
            case 5:
            show_party(4,"NOTA    ");
            break;
        }
        // ---------- ADMIN MODE ----------
if((P1IN & SW3) == 0)
{
    delay(20);

    if((P1IN & SW3) == 0)
    {
        while((P1IN & SW3) == 0);

        lcd_write(0x01, CMD);
        delay(100);

        set_cursor(0,0);
        lcd_print("ADMIN MODE");

        set_cursor(1,0);
        lcd_print("CHECK PUTTY");

        uart_send_results();

        delay(500);

        return;
    }
}
        if((P1IN & SW1) == 0){
            delay(20);
            if((P1IN & SW1) == 0){
                currentparty++;
                if(currentparty > 5){
                    currentparty = 1;
                }
            while((P1IN & SW1)==0);
            }
        }
        if((P1IN & SW2) == 0){
            delay(20);
            if((P1IN & SW2) == 0){
                totalvotes += 1;
                unsigned int i = 0;
                switch(currentparty){
                    case 1:
                    bjpVotes += 1;
                    break;
                    case 2: 
                    congressVotes += 1;
                    break;
                    case 3:
                    aapVotes += 1;
                    break;
                    case 4:
                    kjpVotes += 1;
                    break;
                    case 5:
                    notaVotes += 1;
                    break;
                    }
                P1OUT &= ~LED2;
                lcd_write(0x01, CMD);
                delay(100);
                set_cursor(0, 4);
                lcd_print("THANK");
                set_cursor(1, 6);
                lcd_print("YOU");
                for(i = 0;i<10;i++){
                P1OUT ^= LED2;
                delay(100);
            }
                while((P1IN & SW2) == 0);
                delay(10000);
                currentparty = 1;
                continue;
            }
        }
    }
}

// Writing Function for UART transmission
void register_setting_for_uart(){
    P1SEL |= RX | TX;
    P1SEL2 |= RX | TX;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;
    UCA0BR0 = 104;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;
    UCA0CTL1 &= ~UCSWRST;
}

void uart_send_character(char c){
    while(!(IFG2 & UCA0TXIFG));
    UCA0TXBUF = c;
}

void uart_send_string(const char *s){
    while(*s){
        uart_send_character(*s);
        s++;
    }
}

void uart_send_numb(uint16_t number){
    char digits[5];
    uint8_t i = 0;
    if(number == 0){
        uart_send_character('0');
        return ;
    }
    while(number > 0){
        digits[i] = (number % 10) + '0';
        number /= 10;
        i++; 
    }
    while(i>0){
        i--;
        uart_send_character(digits[i]);
    }
}

void uart_send_results()
{
    uart_send_string("\r\n");
    uart_send_string("----- ELECTION RESULTS -----\r\n\r\n");

    uart_send_string("Total Votes : ");
    uart_send_numb(totalvotes);
    uart_send_string("\r\n\r\n");

    uart_send_string("BJP        : ");
    uart_send_numb(bjpVotes);
    uart_send_string("\r\n");

    uart_send_string("CONGRESS  : ");
    uart_send_numb(congressVotes);
    uart_send_string("\r\n");

    uart_send_string("AAP        : ");
    uart_send_numb(aapVotes);
    uart_send_string("\r\n");

    uart_send_string("KJP        : ");
    uart_send_numb(kjpVotes);
    uart_send_string("\r\n");

    uart_send_string("NOTA       : ");
    uart_send_numb(notaVotes);
    uart_send_string("\r\n");

    uart_send_string("----------------------------\r\n");
}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    register_settings_for_gpio();
    lcd_init();
    register_setting_for_uart();
    create_characters();
    display_startup();
    while(1)
    {
        
        if((P1IN & SW2) == 0)
        {
            delay(20);

            if((P1IN & SW2) == 0)
            {
                while((P1IN & SW2) == 0);

                display_party();
            }
        }
    }
}
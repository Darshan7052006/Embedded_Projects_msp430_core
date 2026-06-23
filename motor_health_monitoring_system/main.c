#include <msp430.h>
#include <inttypes.h>
#include <stdio.h>

#define SYSTEM_OK       1
#define SYSTEM_FAULT    0

#define NORMAL 0
#define OVERHEAT 1
#define OVERCURRENT 2
#define TEMP_LIMIT 50.0f
#define CURRENT_LIMIT 1.0f

/* LCD Pins */
#define CMD 0
#define DATA 1
#define LCD_RS BIT0
#define LCD_EN BIT1
#define LCD_D4 BIT2
#define LCD_D5 BIT3
#define LCD_D6 BIT4
#define LCD_D7 BIT5

/* Sensors */
#define CURRENT_SENSOR BIT6
#define TEMP_SENSOR BIT7

/* Motor Driver */
#define MOTOR_ENA BIT0
#define MOTOR_IN1 BIT1
#define MOTOR_IN2 BIT2

/* Push Button */
#define START_BUTTON BIT3

void register_settings_for_gpio(void){
    P1DIR |= (LCD_RS | LCD_EN | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
    P1DIR &= ~(TEMP_SENSOR | CURRENT_SENSOR);

    P2DIR |= (MOTOR_ENA | MOTOR_IN1 | MOTOR_IN2);
    P2DIR &= ~START_BUTTON;

    P2REN |= START_BUTTON;
    P2OUT |= START_BUTTON;

    P1OUT &= ~(LCD_RS | LCD_EN | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
    P2OUT &= ~(MOTOR_ENA | MOTOR_IN1 | MOTOR_IN2);
}

void delay(uint16_t t){
    uint16_t i;
    for(i = t; i > 0; i--){
        __delay_cycles(100);
    }
}

void pulseen(void){
    P1OUT |= LCD_EN;
    delay(1);
    P1OUT &= ~LCD_EN;
    delay(1);
}

void lcd_write(uint8_t value,uint8_t mode){
    if(mode == CMD)
        P1OUT &= ~LCD_RS;
    else
        P1OUT |= LCD_RS;
    P1OUT = ((P1OUT & 0x03) | ((value & 0xF0) >> 2));
    pulseen();
    delay(1);
    P1OUT = ((P1OUT & 0x03) | ((value & 0x0F) << 2));
    pulseen();
    delay(1);
}

void lcd_print(char *s){
    while(*s){
        lcd_write(*s,DATA);
        s++;
    }
}

void lcd_setcursor(uint8_t row,uint8_t column){
    const uint8_t row_offsets[] = {0x00,0x40};
    lcd_write(0x80 | (column + row_offsets[row]),CMD);
    delay(1);
}

void lcd_init(void){
    P1OUT &= ~(LCD_RS | LCD_EN | LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);
    delay(150);
    lcd_write(0x33,CMD);
    delay(50);
    lcd_write(0x32,CMD);
    delay(1);
    lcd_write(0x28,CMD);
    delay(1);
    lcd_write(0x0C,CMD);
    delay(1);
    lcd_write(0x01,CMD);
    delay(20);
    lcd_write(0x06,CMD);
    delay(1);
    lcd_setcursor(0,0);
}

void adc_init(void){
    ADC10CTL0 |= ADC10ON;
    ADC10AE0 |= (CURRENT_SENSOR | TEMP_SENSOR);
    ADC10CTL0 |= (SREF_0 | ADC10SHT_2);
}

uint16_t adc_read(uint8_t channel){
    ADC10CTL0 &= ~ENC;
    ADC10CTL1 &= ~(INCH_6 | INCH_7);
    ADC10CTL1 |= channel;

    ADC10CTL0 |= (ENC | ADC10SC);

    while(ADC10CTL1 & ADC10BUSY);

    return ADC10MEM;
}

float adc_voltage(uint16_t adc_value){
    float voltage;
    voltage = (adc_value * 3.3f) / 1023.0f;
    return voltage;
}

float temperature_read(float voltage){
    float temperature;
    temperature = voltage/0.01f;
    return temperature;
}

float current_read(float voltage){
    float current;
    current = (voltage - 1.65f)/0.066f;
    return current;
}

void motor_start(void){
    P2OUT |= MOTOR_ENA;
    P2OUT |= MOTOR_IN1;
    P2OUT &= ~MOTOR_IN2;
}

void motor_stop(void){
    P2OUT &= ~MOTOR_ENA;
    P2OUT &= ~MOTOR_IN1;
    P2OUT &= ~MOTOR_IN2;
}

void lcd_print_int(int value){
    char buffer[8];

    sprintf(buffer,"%d",value);
    lcd_print(buffer);
}

void lcd_print_float(float value){
    char buffer[16];

    sprintf(buffer,"%.2f",value);
    lcd_print(buffer);
}

void lcd_display(float temp,float current){
    lcd_write(0x01,CMD);
    delay(20);

    lcd_setcursor(0,0);
    lcd_print("T:");
    lcd_print_float(temp);
    lcd_print("C ");

    lcd_setcursor(1,0);
    lcd_print("I:");
    lcd_print_float(current);
    lcd_print("A ");
}

int check_fault(float temp,float current){
if(temp > TEMP_LIMIT){
    motor_stop();
    lcd_write(0x01,CMD);
    delay(20);
    lcd_setcursor(0,0);
    lcd_print("OVERHEAT");
    delay(200);
    return SYSTEM_FAULT;
    }
if(current > CURRENT_LIMIT){
    motor_stop();
    lcd_write(0x01,CMD);
    delay(20);
    lcd_setcursor(0,0);
    lcd_print("OVERCURRENT");
    delay(200);
    return SYSTEM_FAULT;
    }
    return SYSTEM_OK;
}

int main(void){
    float temp;
    float current;
    int status;
    WDTCTL = WDTPW | WDTHOLD;
    register_settings_for_gpio();
    lcd_init();
    adc_init();
    lcd_print("SYSTEM READY");
    delay(100);
    while(1){
    if(!(P2IN & START_BUTTON))
    {
    delay(10);
    while(!(P2IN & START_BUTTON));
    motor_start();
    while(1){
    temp = temperature_read(adc_voltage(adc_read(INCH_7)));
    current = current_read(adc_voltage(adc_read(INCH_6)));
    lcd_display(temp,current);
    status = check_fault(temp,current);
    if(status == SYSTEM_FAULT)
    break;
    delay(50);
    }
    delay(100);
    }
}}
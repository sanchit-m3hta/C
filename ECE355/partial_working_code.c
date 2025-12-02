#include <stdio.h>
#include "diag/Trace.h"
#include <string.h>
#include "cmsis/cmsis_device.h"
#include "stm32f051x8.h"
#include "stm32f0xx_hal_spi.h"

// ----------------------------------------------------------------------------
// STM32F0 led blink sample (trace via $(trace))
// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

/* Definitions of registers and their bits are
  given in system/include/cmsis/stm32f051x8.h */
/* Clock prescaler for TIM2 timer: no prescaling */
#define myTIM2_PRESCALER ((uint16_t)0x0000)
/* Maximum possible setting for overflow */
#define myTIM2_PERIOD ((uint32_t)0xFFFFFFFF)




unsigned int Freq = 0;  // Example: measured frequency value (global variable)
unsigned int potRes = 0;   // Example: measured resistance value (global variable)
unsigned int input_select = 2; //either function gen freq or the 555timer freq
unsigned int Triggered555 = 0;

void oled_Write(unsigned char);
void oled_Write_Cmd(unsigned char);
void oled_Write_Data(unsigned char);

void oled_config(void);
void refresh_OLED(void);

void ADC_init(void);

void myTIM2_Init(void);

void myGPIOA_Init(void);
void myGPIOB_Init(void);
void myEXTI_Init(void);
void DAC_init(void);



SPI_HandleTypeDef SPI_Handle;

unsigned char oled_init_cmds[] =
{
    0xAE,
    0x20, 0x00,
    0x40,
    0xA0 | 0x01,
    0xA8, 0x40 - 1,
    0xC0 | 0x08,
    0xD3, 0x00,
    0xDA, 0x32,
    0xD5, 0x80,
    0xD9, 0x22,
    0xDB, 0x30,
    0x81, 0xFF,
    0xA4,
    0xA6,
    0xAD, 0x30,
    0x8D, 0x10,
    0xAE | 0x01,
    0xC0,
    0xA0
};

unsigned char Characters[][8] = {
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // SPACE
    {0b00000000, 0b00000000, 0b01011111, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // !
    {0b00000000, 0b00000111, 0b00000000, 0b00000111, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // "
    {0b00010100, 0b01111111, 0b00010100, 0b01111111, 0b00010100,0b00000000, 0b00000000, 0b00000000},  // #
    {0b00100100, 0b00101010, 0b01111111, 0b00101010, 0b00010010,0b00000000, 0b00000000, 0b00000000},  // $
    {0b00100011, 0b00010011, 0b00001000, 0b01100100, 0b01100010,0b00000000, 0b00000000, 0b00000000},  // %
    {0b00110110, 0b01001001, 0b01010101, 0b00100010, 0b01010000,0b00000000, 0b00000000, 0b00000000},  // &
    {0b00000000, 0b00000101, 0b00000011, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // '
    {0b00000000, 0b00011100, 0b00100010, 0b01000001, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // (
    {0b00000000, 0b01000001, 0b00100010, 0b00011100, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // )
    {0b00010100, 0b00001000, 0b00111110, 0b00001000, 0b00010100,0b00000000, 0b00000000, 0b00000000},  // *
    {0b00001000, 0b00001000, 0b00111110, 0b00001000, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // +
    {0b00000000, 0b01010000, 0b00110000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // ,
    {0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // -
    {0b00000000, 0b01100000, 0b01100000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // .
    {0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010,0b00000000, 0b00000000, 0b00000000},  // /
    {0b00111110, 0b01010001, 0b01001001, 0b01000101, 0b00111110,0b00000000, 0b00000000, 0b00000000},  // 0
    {0b00000000, 0b01000010, 0b01111111, 0b01000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // 1
    {0b01000010, 0b01100001, 0b01010001, 0b01001001, 0b01000110,0b00000000, 0b00000000, 0b00000000},  // 2
    {0b00100001, 0b01000001, 0b01000101, 0b01001011, 0b00110001,0b00000000, 0b00000000, 0b00000000},  // 3
    {0b00011000, 0b00010100, 0b00010010, 0b01111111, 0b00010000,0b00000000, 0b00000000, 0b00000000},  // 4
    {0b00100111, 0b01000101, 0b01000101, 0b01000101, 0b00111001,0b00000000, 0b00000000, 0b00000000},  // 5
    {0b00111100, 0b01001010, 0b01001001, 0b01001001, 0b00110000,0b00000000, 0b00000000, 0b00000000},  // 6
    {0b00000011, 0b00000001, 0b01110001, 0b00001001, 0b00000111,0b00000000, 0b00000000, 0b00000000},  // 7
    {0b00110110, 0b01001001, 0b01001001, 0b01001001, 0b00110110,0b00000000, 0b00000000, 0b00000000},  // 8
    {0b00000110, 0b01001001, 0b01001001, 0b00101001, 0b00011110,0b00000000, 0b00000000, 0b00000000},  // 9
    {0b00000000, 0b00110110, 0b00110110, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // :
    {0b00000000, 0b01010110, 0b00110110, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // ;
    {0b00001000, 0b00010100, 0b00100010, 0b01000001, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // <
    {0b00010100, 0b00010100, 0b00010100, 0b00010100, 0b00010100,0b00000000, 0b00000000, 0b00000000},  // =
    {0b00000000, 0b01000001, 0b00100010, 0b00010100, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // >
    {0b00000010, 0b00000001, 0b01010001, 0b00001001, 0b00000110,0b00000000, 0b00000000, 0b00000000},  // ?
    {0b00110010, 0b01001001, 0b01111001, 0b01000001, 0b00111110,0b00000000, 0b00000000, 0b00000000},  // @
    {0b01111110, 0b00010001, 0b00010001, 0b00010001, 0b01111110,0b00000000, 0b00000000, 0b00000000},  // A
    {0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b00110110,0b00000000, 0b00000000, 0b00000000},  // B
    {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00100010,0b00000000, 0b00000000, 0b00000000},  // C
    {0b01111111, 0b01000001, 0b01000001, 0b00100010, 0b00011100,0b00000000, 0b00000000, 0b00000000},  // D
    {0b01111111, 0b01001001, 0b01001001, 0b01001001, 0b01000001,0b00000000, 0b00000000, 0b00000000},  // E
    {0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00000001,0b00000000, 0b00000000, 0b00000000},  // F
    {0b00111110, 0b01000001, 0b01001001, 0b01001001, 0b01111010,0b00000000, 0b00000000, 0b00000000},  // G
    {0b01111111, 0b00001000, 0b00001000, 0b00001000, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // H
    {0b01000000, 0b01000001, 0b01111111, 0b01000001, 0b01000000,0b00000000, 0b00000000, 0b00000000},  // I
    {0b00100000, 0b01000000, 0b01000001, 0b00111111, 0b00000001,0b00000000, 0b00000000, 0b00000000},  // J
    {0b01111111, 0b00001000, 0b00010100, 0b00100010, 0b01000001,0b00000000, 0b00000000, 0b00000000},  // K
    {0b01111111, 0b01000000, 0b01000000, 0b01000000, 0b01000000,0b00000000, 0b00000000, 0b00000000},  // L
    {0b01111111, 0b00000010, 0b00001100, 0b00000010, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // M
    {0b01111111, 0b00000100, 0b00001000, 0b00010000, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // N
    {0b00111110, 0b01000001, 0b01000001, 0b01000001, 0b00111110,0b00000000, 0b00000000, 0b00000000},  // O
    {0b01111111, 0b00001001, 0b00001001, 0b00001001, 0b00000110,0b00000000, 0b00000000, 0b00000000},  // P
    {0b00111110, 0b01000001, 0b01010001, 0b00100001, 0b01011110,0b00000000, 0b00000000, 0b00000000},  // Q
    {0b01111111, 0b00001001, 0b00011001, 0b00101001, 0b01000110,0b00000000, 0b00000000, 0b00000000},  // R
    {0b01000110, 0b01001001, 0b01001001, 0b01001001, 0b00110001,0b00000000, 0b00000000, 0b00000000},  // S
    {0b00000001, 0b00000001, 0b01111111, 0b00000001, 0b00000001,0b00000000, 0b00000000, 0b00000000},  // T
    {0b00111111, 0b01000000, 0b01000000, 0b01000000, 0b00111111,0b00000000, 0b00000000, 0b00000000},  // U
    {0b00011111, 0b00100000, 0b01000000, 0b00100000, 0b00011111,0b00000000, 0b00000000, 0b00000000},  // V
    {0b00111111, 0b01000000, 0b00111000, 0b01000000, 0b00111111,0b00000000, 0b00000000, 0b00000000},  // W
    {0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011,0b00000000, 0b00000000, 0b00000000},  // X
    {0b00000111, 0b00001000, 0b01110000, 0b00001000, 0b00000111,0b00000000, 0b00000000, 0b00000000},  // Y
    {0b01100001, 0b01010001, 0b01001001, 0b01000101, 0b01000011,0b00000000, 0b00000000, 0b00000000},  // Z
    {0b01111111, 0b01000001, 0b00000000, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // [
    {0b00010101, 0b00010110, 0b01111100, 0b00010110, 0b00010101,0b00000000, 0b00000000, 0b00000000},  // back slash
    {0b00000000, 0b00000000, 0b00000000, 0b01000001, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // ]
    {0b00000100, 0b00000010, 0b00000001, 0b00000010, 0b00000100,0b00000000, 0b00000000, 0b00000000},  // ^
    {0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000,0b00000000, 0b00000000, 0b00000000},  // _
    {0b00000000, 0b00000001, 0b00000010, 0b00000100, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // `
    {0b00100000, 0b01010100, 0b01010100, 0b01010100, 0b01111000,0b00000000, 0b00000000, 0b00000000},  // a
    {0b01111111, 0b01001000, 0b01000100, 0b01000100, 0b00111000,0b00000000, 0b00000000, 0b00000000},  // b
    {0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00100000,0b00000000, 0b00000000, 0b00000000},  // c
    {0b00111000, 0b01000100, 0b01000100, 0b01001000, 0b01111111,0b00000000, 0b00000000, 0b00000000},  // d
    {0b00111000, 0b01010100, 0b01010100, 0b01010100, 0b00011000,0b00000000, 0b00000000, 0b00000000},  // e
    {0b00001000, 0b01111110, 0b00001001, 0b00000001, 0b00000010,0b00000000, 0b00000000, 0b00000000},  // f
    {0b00001100, 0b01010010, 0b01010010, 0b01010010, 0b00111110,0b00000000, 0b00000000, 0b00000000},  // g
    {0b01111111, 0b00001000, 0b00000100, 0b00000100, 0b01111000,0b00000000, 0b00000000, 0b00000000},  // h
    {0b00000000, 0b01000100, 0b01111101, 0b01000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // i
    {0b00100000, 0b01000000, 0b01000100, 0b00111101, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // j
    {0b01111111, 0b00010000, 0b00101000, 0b01000100, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // k
    {0b00000000, 0b01000001, 0b01111111, 0b01000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // l
    {0b01111100, 0b00000100, 0b00011000, 0b00000100, 0b01111000,0b00000000, 0b00000000, 0b00000000},  // m
    {0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b01111000,0b00000000, 0b00000000, 0b00000000},  // n
    {0b00111000, 0b01000100, 0b01000100, 0b01000100, 0b00111000,0b00000000, 0b00000000, 0b00000000},  // o
    {0b01111100, 0b00010100, 0b00010100, 0b00010100, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // p
    {0b00001000, 0b00010100, 0b00010100, 0b00011000, 0b01111100,0b00000000, 0b00000000, 0b00000000},  // q
    {0b01111100, 0b00001000, 0b00000100, 0b00000100, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // r
    {0b01001000, 0b01010100, 0b01010100, 0b01010100, 0b00100000,0b00000000, 0b00000000, 0b00000000},  // s
    {0b00000100, 0b00111111, 0b01000100, 0b01000000, 0b00100000,0b00000000, 0b00000000, 0b00000000},  // t
    {0b00111100, 0b01000000, 0b01000000, 0b00100000, 0b01111100,0b00000000, 0b00000000, 0b00000000},  // u
    {0b00011100, 0b00100000, 0b01000000, 0b00100000, 0b00011100,0b00000000, 0b00000000, 0b00000000},  // v
    {0b00111100, 0b01000000, 0b00111000, 0b01000000, 0b00111100,0b00000000, 0b00000000, 0b00000000},  // w
    {0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100,0b00000000, 0b00000000, 0b00000000},  // x
    {0b00001100, 0b01010000, 0b01010000, 0b01010000, 0b00111100,0b00000000, 0b00000000, 0b00000000},  // y
    {0b01000100, 0b01100100, 0b01010100, 0b01001100, 0b01000100,0b00000000, 0b00000000, 0b00000000},  // z
    {0b00000000, 0b00001000, 0b00110110, 0b01000001, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // {
    {0b00000000, 0b00000000, 0b01111111, 0b00000000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // |
    {0b00000000, 0b01000001, 0b00110110, 0b00001000, 0b00000000,0b00000000, 0b00000000, 0b00000000},  // }
    {0b00001000, 0b00001000, 0b00101010, 0b00011100, 0b00001000,0b00000000, 0b00000000, 0b00000000},  // ~
    {0b00001000, 0b00011100, 0b00101010, 0b00001000, 0b00001000,0b00000000, 0b00000000, 0b00000000}   // <-
};

//user define to implement delay
void delay_ms(unsigned int ms)
{
    ms *= 48000;
    for(unsigned int i = 0; i < ms; i++)
    {
        asm("NOP");
    }
}

void SystemClock48MHz( void )
{
    RCC->CR &= ~(RCC_CR_PLLON);
    while (( RCC->CR & RCC_CR_PLLRDY ) != 0 );

    RCC->CFGR = 0x00280000;

    RCC->CR |= RCC_CR_PLLON;
    while (( RCC->CR & RCC_CR_PLLRDY ) != RCC_CR_PLLRDY );

    RCC->CFGR = ( RCC->CFGR & (~RCC_CFGR_SW_Msk)) | RCC_CFGR_SW_PLL;

    SystemCoreClockUpdate();
}







int main(int argc, char* argv[])
{
    SystemClock48MHz(); //init clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;  /* Enable SYSCFG clock */
    myGPIOA_Init();        /* Initialize I/O port PA */
    myGPIOB_Init();        /* Initialize I/O port PB */
    myTIM2_Init();        /* Initialize timer TIM2 */
    myEXTI_Init();        /* Initialize EXTI */
    ADC_init();
    DAC_init();

    Freq = 3000;
    potRes = 9999;

    oled_config();

    while (1)
    {
        refresh_OLED();
        ADC1->CR |= ADC_CR_ADSTART;
        while(!(ADC1->ISR & ADC_ISR_EOC));
        float adc_value = ADC1->DR;
        potRes = (adc_value / 4095 )*5000;
        DAC->DHR12R1 = adc_value;
    }
    return 0;
}









void refresh_OLED( void )
{
    unsigned char Buffer[17];

    snprintf( Buffer, sizeof( Buffer ), "R: %5u Ohms", potRes );

    oled_Write_Cmd(0xB2);
    oled_Write_Cmd(0x0F);
    oled_Write_Cmd(0x10);

    for(int i = 0; Buffer[i] != '\0'; i++)
    {
        int c = (int)Buffer[i];
        for(int j = 0; j < 8; j++)
        {
            oled_Write_Data(Characters[c][j]);
        }
    }

    snprintf( Buffer, sizeof( Buffer ), "F: %5u Hz", Freq );

    oled_Write_Cmd(0xB5);
    oled_Write_Cmd(0x0F);
    oled_Write_Cmd(0x10);

    for(int i = 0; Buffer[i] != '\0'; i++)
    {
        int c = (int)Buffer[i];
        for(int j = 0; j < 8; j++)
        {
            oled_Write_Data(Characters[c][j]);
        }
    }

    delay_ms(100);
}

void oled_Write_Cmd( unsigned char cmd )
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    oled_Write( cmd );
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
}

void oled_Write_Data( unsigned char data )
{
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_RESET);
    oled_Write( data );
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8, GPIO_PIN_SET);
}

void oled_Write( unsigned char Value )
{
    while( (SPI2->SR & SPI_SR_TXE) == 0);

    HAL_SPI_Transmit( &SPI_Handle, &Value, 1, HAL_MAX_DELAY );

    while( (SPI2->SR & SPI_SR_TXE) == 0);
}

void oled_config( void )
{

    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    GPIOB->MODER &= ~GPIO_MODER_MODER13_0;
    GPIOB->MODER &= ~GPIO_MODER_MODER13_1;
    GPIOB->MODER |= GPIO_MODER_MODER13_1;

    GPIOB->MODER &= ~GPIO_MODER_MODER15_0;
    GPIOB->MODER &= ~GPIO_MODER_MODER15_1;
    GPIOB->MODER |= GPIO_MODER_MODER15_1;

    GPIOB->MODER |= GPIO_MODER_MODER8_0;
    GPIOB->MODER |= GPIO_MODER_MODER9_0;
    GPIOB->MODER |= GPIO_MODER_MODER11_0;



    // Configure PB13 as AF0
    GPIOB->AFR[1] &= ~(0xF << 20);
    GPIOB->AFR[1] |=  (0x0 << 20);

    // Configure PB15 as AF0
   GPIOB->AFR[1] &= ~(0xF << 28);  
    GPIOB->AFR[1] |=  (0x0 << 28);

    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

    SPI_Handle.Instance = SPI2;

    SPI_Handle.Init.Direction = SPI_DIRECTION_1LINE;
    SPI_Handle.Init.Mode = SPI_MODE_MASTER;
    SPI_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
    SPI_Handle.Init.CLKPolarity = SPI_POLARITY_LOW;
    SPI_Handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    SPI_Handle.Init.NSS = SPI_NSS_SOFT;
    SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    SPI_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SPI_Handle.Init.CRCPolynomial = 7;

    HAL_SPI_Init( &SPI_Handle );
    __HAL_SPI_ENABLE( &SPI_Handle );

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11, GPIO_PIN_RESET);
    delay_ms(2);
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11, GPIO_PIN_SET);
    delay_ms(2);

    for ( unsigned int i = 0; i < sizeof( oled_init_cmds ); i++ )
    {
        oled_Write_Cmd( oled_init_cmds[i] );
    }

    for(int page = 0; page < 8; page++)
    {
        oled_Write_Cmd(0xB0 + page);
        oled_Write_Cmd(0x00);
        oled_Write_Cmd(0x00);
        for(int column = 0; column < 130; column++)
        {
            oled_Write_Data(0x00);
        }
    }
}



void ADC_init(void)
{
    //enable ADC clock
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    //calibration
    ADC1->CR |= ADC_CR_ADCAL;
    while(ADC1->CR & ADC_CR_ADCAL);
    //configure ADC settings
    ADC1->CFGR1 &= ~ADC_CFGR1_RES; // set resolution to 12 bits
    ADC1->SMPR |= ADC_SMPR_SMP_1;
    // Select channel 1 PA1
    ADC1->CHSELR = ADC_CHSELR_CHSEL1;
    GPIOA->MODER |= GPIO_MODER_MODER1; //configure PA1 as analog
    //Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while(!(ADC1->ISR & ADC_ISR_ADRDY)); // wait for ADC to stabilize
}
void DAC_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;
    DAC->CR |= DAC_CR_EN1;
    DAC->CR &= ~DAC_CR_TEN1;
}

void myGPIOA_Init(void)
{

    RCC->AHBENR &= RCC_AHBENR_GPIOAEN; //enableing clock for a pins

    //configure PA0 as input for user switch
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
    GPIOA->MODER &= ~(GPIO_MODER_MODER0);

    //Configure PA4 as analog
    GPIOA->MODER |= GPIO_MODER_MODER4;

}



void myGPIOB_Init(void)
{

    RCC->AHBENR &= RCC_AHBENR_GPIOBEN;

    //Configure PB2 for input 
    GPIOB->MODER &= ~(GPIO_MODER_MODER2);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR2);

    GPIOB->MODER &= ~(GPIO_MODER_MODER3);
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR1);
}
void myTIM2_Init(void)
{
    /* Enable clock for TIM2 peripheral */
    // Relevant register: RCC->APB1ENR
    RCC->APB1ENR |= 0x1;
    /* Configure TIM2: buffer auto-reload, count up, stop on overflow,
     * enable update events, interrupt on overflow only */
    // Relevant register: TIM2->CR1
    TIM2->CR1 &= 0x8C; //1000 1100
    /* Set clock prescaler value */
    TIM2->PSC = myTIM2_PRESCALER;
    /* Set auto-reloaded delay */
    TIM2->ARR = myTIM2_PERIOD;
    /* Update timer registers */
    // Relevant register: TIM2->EGR
    TIM2->EGR |= 0x1;
    /* Assign TIM2 interrupt priority = 0 in NVIC */
    // Relevant register: NVIC->IP[3], or use NVIC_SetPriority
    NVIC_SetPriority(TIM2_IRQn, 0);
    /* Enable TIM2 interrupts in NVIC */
    // Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
    NVIC_EnableIRQ(TIM2_IRQn);
    /* Enable update interrupt generation */
    // Relevant register: TIM2->DIER
    TIM2->DIER |= 0x1;
}
void myEXTI_Init()
{
    /* Map EXTI2 line to PB2 */
    // Relevant register: SYSCFG->EXTICR[0]
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2; // clearing EXTICR 2
    SYSCFG->EXTICR[0] |= 0x100; 
    /* EXTI2 line interrupts: set rising-edge trigger */
    // Relevant register: EXTI->RTSR
    EXTI->RTSR |= EXTI_RTSR_RT2; 
    /* Unmask interrupts from EXTI2 line */
    // Relevant register: EXTI->IMR
    EXTI->IMR |= EXTI_IMR_IM2;
    /* Assign EXTI2 interrupt priority = 0 in NVIC */
    // Relevant register: NVIC->IP[2], or use NVIC_SetPriority
    NVIC_SetPriority(EXTI2_3_IRQn, 0);
    /* Enable EXTI2 interrupts in NVIC */
    // Relevant register: NVIC->ISER[0], or use NVIC_EnableIRQ
    NVIC_EnableIRQ(EXTI2_3_IRQn);
    
    /*FIX THIS*/
    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
    SYSCFG->EXTICR[0] &=  ~SYSCFG_EXTICR1_EXTI3;// for 555 timer
    SYSCFG->EXTICR[0] |=  0x1000;

    EXTI->RTSR |= EXTI_RTSR_RT0;
    EXTI->RTSR |= EXTI_RTSR_RT3;

    EXTI->IMR |= EXTI_IMR_IM0;
    EXTI->IMR |= EXTI_IMR_IM3;

    NVIC_SetPriority(EXTI0_1_IRQn, 1);

    NVIC_EnableIRQ(EXTI0_1_IRQn);

}
/* This handler is declared in system/src/cmsis/vectors_stm32f051x8.c */
void TIM2_IRQHandler()
{
    /* Check if update interrupt flag is indeed set */
    if ((TIM2->SR & TIM_SR_UIF) != 0)
    {
        trace_printf("\n*** Overflow! ***\n");
        /* Clear update interrupt flag */
        // Relevant register: TIM2->SR
        TIM2->SR &= ~TIM_SR_UIF;
        /* Restart stopped timer */
        // Relevant register: TIM2->CR1
        TIM2->CR1 |= TIM_CR1_CEN;
    }
}
/* This handler is declared in system/src/cmsis/vectors_stm32f051x8.c */
void EXTI2_3_IRQHandler()
{
    NVIC_DisableIRQ(EXTI2_3_IRQn);
    // Declare/initialize your local variables here...
    //int first_edge = 0;
    unsigned int count = 0;
    unsigned int frequency = 0;
    unsigned int period = 0;
    /* Check if EXTI2 interrupt pending flag is indeed set */
    if ((EXTI->PR & EXTI_PR_PR2) != 1)
    {
        if((TIM2->CR1 & TIM_CR1_CEN) == 0)
        {
            TIM2->CNT &= 0x0; // total count
            TIM2->CR1 |= TIM_CR1_CEN;
        }
        else
        {
            TIM2->CR1 &= ~0x1;
            count = TIM2->CNT;
            //trace_printf("%d count\n", count);
            frequency = (unsigned int)((float)48000000/(float)count);
            //period = (unsigned int)(1000000*1/frequency);
            //trace_printf("Period: %d MicroS\n", period);
            //trace_printf("Frequency: %d Hz\n", frequency);
            Freq = frequency;
            //period = 1.0/current_count;
            //trace_printf("%.3f", period);
        }

    }
    EXTI->PR |= EXTI_PR_PR2;

    NVIC_EnableIRQ(EXTI2_3_IRQn);

}

void EXTI0_1_IRQHandler()
{
    unsigned int temp = EXTI->PR & EXTI_PR_PR0;
    unsigned int temp2 = EXTI->PR & EXTI_PR_PR3;
    NVIC_DisableIRQ(EXTI0_1_IRQn);

    if( temp != 0) //check if PA0 interrupt pending flag is set
    {
        temp = 0;
        trace_printf("Entering first if\n");

        if(input_select == 2)
        {
            trace_printf("Entering second if\n");

            input_select = 1;
            EXTI->IMR &= ~EXTI_IMR_IM2; // Masking function generator interrupt
            EXTI->IMR |= EXTI_IMR_IM3; // Unmasking NE555 timer interrupt
        }
        else if(input_select == 1)
        {
            input_select = 2;
            EXTI->IMR &= ~EXTI_IMR_IM3;
            EXTI->IMR |= EXTI_IMR_IM2;
        }
    EXTI->PR |= EXTI_PR_PR0;

    }
    else if(temp2 != 0)
    {
        temp2 = 0;
        trace_printf("Entering timer 555 if\n");

        if(input_select == 1)
        {
            float currentCount = 0;
            float timeElapsed = 0;
            float period = 0;
            float frequency = 0;
            if(Triggered555 == 0)
            {
                TIM2->CNT = 0;
                TIM2->CR1 |= TIM_CR1_CEN;
                Triggered555 = 1;
            }
            else
            {
                TIM2->CR1 &= ~TIM_CR1_CEN;
                currentCount = TIM2->CNT;
                timeElapsed = currentCount;
                period = (timeElapsed * 1000.0f) / SystemCoreClock;
                frequency = SystemCoreClock / timeElapsed;
                Freq = (unsigned int)frequency;
                Triggered555 = 0;
            }
        }

        EXTI->PR |= EXTI_PR_PR3;
    }

    delay_ms(100);
    NVIC_EnableIRQ(EXTI0_1_IRQn);

}



#pragma GCC diagnostic pop


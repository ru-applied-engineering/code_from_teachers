/*
 * uart.h
 *
 *  Created on: Feb 14, 2021
 *      Author: kristomaack
 */

#ifndef uart
#define uart

#ifdef __cplusplus
extern "C" {
#endif

void init_Uart();
void TxData(char data);
void UART_Transmit_String(const char s[]);
int strlen2(char s[]);
void reverse(char s[]);
void UART_itoa(int n, char s[]);
void UART_Transmit_Decimal(int i);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

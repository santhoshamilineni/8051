/*
 * uart.c
 *
 * Copyright 2016 santhosh <amilinenisanthosh@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 * */

#include<at89x52.h>
#include<stdio.h>

/*Delay in milli sec    */
void delay_ms(unsigned int k) {
    unsigned int i;    
    unsigned char j;
    for(i=0;i<k;i++) {
        for(j=0;j<110;j++);
    }
}

/*Send one character in usart*/
void uart_send_char(unsigned char ch) {
    SBUF=ch;
    while(TI==0);
    TI=0;
}

/*Send string through usart*/
void uart_send_string(unsigned char *buff) {
    while(*buff) {
        uart_send_char(*buff);
        buff++;
    }
}

unsigned char uart_receive_char() {
    unsigned char ch;
    while(RI==0);
    ch=SBUF;
    RI=0;
    return(ch);
}

/*USART Init function*/
void sconfig() {
    SCON=0x50;
    TMOD=0x20;
    TH1=0xfd;
    TR1=1;
}


void main(){
    unsigned char ch;
    sconfig();
    uart_send_string("Welcome\r\n");
    /* Keep listening uart RX pin, if any data available send data into uart tx pin */
    while(1){
        ch=uart_receive_char();
        uart_send_char(ch);
    }
    
}

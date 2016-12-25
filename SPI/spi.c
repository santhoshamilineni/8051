/*
 * SPI.c
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
#include<string.h>
#include<stdlib.h>



#define CS			P1_4
#define MISO		P1_5
#define MOSI		P1_6
#define SCK			P1_7


//opcodes
#define WREN  6
#define WRDI  4
#define RDSR  5
#define WRSR  1
#define READ  3
#define WRITE 2


/*Delay in milli sec	*/
void delay_ms(unsigned int k) {
	unsigned int i;	
	unsigned char j;
	for(i=0;i<k;i++) {
		for(j=0;j<110;j++); 
	}
}

/*Send one character in usart*/
void send(unsigned char ch) {
	SBUF=ch;
	while(TI==0);
	TI=0;
}

/*Send string through usart*/
void uart_send_string(unsigned char *buff) {
	while(*buff) {
		send(*buff);
		buff++;
	}
}


/*USART Init function*/
void sconfig() {
	SCON=0x50;
	TMOD=0x20;
	TH1=0xfd;
	TR1=1;
}

void SPI_config(){	
	SCK=0;
	MISO=0;
	MOSI=0;
	CS=1;
}

void write_byte(unsigned char ch){
	unsigned char i;
	for(i=1;i<9;i++){
		SCK=1;
		MOSI=(ch&(0x80>>i))?1:0;
		SCK=0;
	}
}

unsigned char red_byte(){
	unsigned char i=0,ch=0;

		for(i=0;i<8;i++){
			SCK=1;
			if(MISO)
				ch|=(0x80>>i);
			SCK=0;
		}
		return ch;
}
void configure_eprom_writemode(){
	CS=0;
	write_byte(WREN);	//Enabling the write latch in eprom
	CS=1;   
}
void spi_write_byte(unsigned char add,unsigned char loc,unsigned char dat){
	
	CS=0;
	write_byte(add);
	write_byte(loc);
	write_byte(dat);
	CS=1;             
	delay_ms(20);
}
void spi_read_byte(unsigned char add,unsigned char loc){
	unsigned char ch=0;
	CS=0;
	write_byte(add);
	write_byte(loc);
	ch=red_byte();
	send(ch);
	CS=1;         	
}
void main(){
	sconfig();
	SPI_config();
	uart_send_string("Welcome\r\n");
	configure_eprom_writemode();
	spi_write_byte(WRITE,0x00,'S');
	spi_read_byte(READ,0x00);
}

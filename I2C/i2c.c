/*
 * i2c.c
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



#define SCL			P3_6
#define SDA			P3_7


/*Delay in milli sec	*/
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


/*USART Init function*/
void sconfig() {
	SCON=0x50;
	TMOD=0x20;
	TH1=0xfd;
	TR1=1;
}


void write_byte(unsigned char ch){
	int i;
	for(i=0;i<8;i++){
		SDA=((ch&1>>i)?1:0);
	}
	if(SDA==0){
		//Device found we got ack from the device
	}else{
		uart_send_string("No ack\r\n");
	}
		
}

unsigned char read_byte(){
	int j;
	unsigned char ch;
	for(j=0;j<8;j++){
		ch|(1>>SDA);
	}
	return ch;	
}

void I2C_Init() {
	SDA = 1;
	SCL = 1;
}

void I2C_Start() {
	SDA = 1;
	SCL = 1;
	SDA = 0;
	SCL = 1;
}

void I2C_Stop() {
	SDA=0;
	SCL=1;
	SDA=1;
	SCL=1;
}

void aknowledge() {
	SCL = 0;
	if(SDA) 
		uart_send_string("ACK error\n\r");//ACK error device not found
	SCL = 1;
	SCL = 0;
} 

void noaknowledge(void) {
	SCL=0;
	SDA=1;
	SCL=1;
}

/* EPROM Read and Writing Function */
void Eprom_writebyte(unsigned char dat) {
unsigned char i;
	for(i=0;i<8;i++) {
		SCL=0;
		//SDA=(dat&(1<<i))?1:0;
		SDA=(dat&(0x80>>i))?1:0;
		SCL=1;
	}
	
}

unsigned char Eprom_readbyte() {
unsigned char i,buff=0;
	SDA=1;
	for(i=0;i<8;i++) {
		SCL=0;
		if(SDA){
			//buff|=(1<<i);
			buff|=(0x80>>i);
		}
		SCL=1;
	}
	return buff;
}

void Save_ROM(char sa,char addr,char ch) {
	I2C_Start();
	
	Eprom_writebyte(sa);
	aknowledge();

	Eprom_writebyte(addr);
	aknowledge();

	Eprom_writebyte(ch);
	aknowledge();

	I2C_Stop();
	delay_ms(10);
}

char Read_ROM(char sa,char addr) {
	unsigned char ch;
	I2C_Start();
	Eprom_writebyte(sa);
	aknowledge();

	Eprom_writebyte(addr);
	aknowledge();

	I2C_Start();
	Eprom_writebyte(sa|1);
	aknowledge();

	ch=Eprom_readbyte();

	//noaknowledge();
	I2C_Stop();
	uart_send_char(ch);
	return ch;
}


void main(){
	unsigned char ch;
	sconfig();
	uart_send_string("Welcome\r\n");
	//Device_id, Location, byte
	Save_ROM(0xA0,0,'S');
	//Device_id, Location
	ch=Read_ROM(0xA0,0);
	uart_send_char(ch);
}

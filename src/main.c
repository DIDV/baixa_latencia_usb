#include <p18f4550.h>
#include <timers.h>
#include "usb_functions.h"

void high_isr(void);
#pragma code high_vector=0x08
 void interrupt_at_high_vector(void)
 {
  _asm GOTO high_isr _endasm
 }

#pragma code

#pragma interrupt high_isr
 void high_isr(void)
 {
     // codigo interrupcao
}

int processa_controle( char controle );
char recebe_dado_usb();
void config_pic();

void main(void)
{
    char byte_recebido;
    config_pic();                       // Configura��es iniciais do PIC (Ports, etc.);
    usb_install();                      // Inicializa��o do USB;
    Delay_ms(3);                        // Aguarda um tempo antes de acionar o rele de alimenta��o dos motores

    do
    {
        byte_recebido = recebe_dado_usb();  // Recebe os dados vindos da rasp;
        processa_controle(byte_recebido);   // Envia o dado obtido para que o controle tome uma decis�o;
    } while(1);
}


/* Fun��o no qual tem como objetivo buscar os dados no buffer de entrada do
 PIC, s� retornando dela quando alguma informa��o for recebida*/
char recebe_dado_usb()
{
    char byte_recebido;

    do
    {
        /*Maquina de estados do USB, no qual verifica se o mesmo esta configurado
         e executa a inclus�o e exclus�o de dados nos buffer de entrada e sa�da.*/
        usb_handler();
        
        /* O valor 0xFF funciona como padrao para a variavel byte_recebido,
         * e significa que nenhum byte foi recebido pelo PIC. */
        byte_recebido = 0xFF;

        /* Caso tenha algum char no buffer de entrada do PIC,
         * esse sera' transferido para a variavel byte_recebido.
         * Caso nao, o valor inicial 0xFF sera' mantido. */
        poll_getc_cdc(&byte_recebido);

        /* Se nenhum byte tiver sido recebido, esse ciclo do loop
         * sera' encerrado nesse ponto. */
    } while ( byte_recebido == 0xFF );

    return byte_recebido;                   // Retorna o dado recebido;

} 




/* Fun��o de controle, no qual toma as decis�es do que ser� feito de acordo
 com o comando recebido da Rasp;*/
int processa_controle( char controle )
{
    switch(controle)
    {
        default:                // N�o faz nada e retorna para receber proximo controle;
            putc_cdc('N');
            putc_cdc('o');
            putc_cdc('p');

    }
    return 1;
}



/*Configura��o inicial do Pic*/
void config_pic(void)
{
    
    TRISA = 0x0F;       // PortA 0 a 3 configurados como entrada (anal�gica) e 4 a 7 como sa�da;
    TRISB = 0x00;       // PortB 0 a 7 como sa�da;
    TRISC = 0x00;       // PortC 0 a 7 como sa�da;
    TRISD = 0x00;       // PortD 0 a 7 como sa�da;
    TRISE = 0x00;       // PortE 0 a 7 como sa�da;
    PORTA = 0x00;       // Todo portA com n�vel l�gico baixo;
    PORTB = 0x00;       // Todo portB com n�vel l�gico baixo;
    PORTC = 0x00;       // Todo portC com n�vel l�gico baixo;
    PORTD = 0x00;       // Todo portD com n�vel l�gico baixo;
    PORTE = 0x00;       // Todo portE com n�vel l�gico baixo;

    ADCON1 = 0x0D;      // 0b0000 1011 - Port A0 at� A3 como anal�gico (Bit 3,2,1 e 0) Setado como +5 e 0V (Bit 6 e 5)
    SSPCON1bits.SSPEN = 1;
}

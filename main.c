#include "recursos/miLibreria.h"		// FUNCIONES DE CONFIGURACION Y HABILITACION
/*----------------------------------------------------------------------------------*/

//                             	VARIABLES GLOBALES			 						//
/************************************************************************************/
const int ECGnormal[256]={120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
        120,120,121,122,124,126,127,129,131,133,135,137,139,141,142,143,144,144,144,143,142,
        140,138,136,133,131,128,126,124,122,121,120,120,120,120,120,120,120,120,120,120,120,120,
        120,120,120,120,120,120,120,120,119,117,115,113,110,108,106,104,102,102,103,108,116,127,
        139,153,167,181,196,209,220,230,236,240,240,238,234,229,222,214,205,196,185,175,164,153,
        142,132,122,113,104,97,91,87,85,84,93,107,119,120,120,120,120,120,120,120,120,120,
        120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,121,121,122,123,
        124,125,126,127,128,129,129,130,131,131,132,132,132,132,131,131,130,129,128,127,126,125,
        124,123,122,122,121,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
        120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
        120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,
        120,120,120,120,120,120,120,120,120,120};

int lectura;
char cadena[8];
int estado=1;
int conteo = 0;
/*----------------------------------------------------------------------------------*/
//                             		SUBRUTINAS 				 						//
/************************************************************************************/



//                             SUBRUTINAS DE INTERRUPCION			 				//
/************************************************************************************/
//								HANDLER DE INTERRUPCION UART						//
void UART0IntHandler()
{
	limpiarUART(UART0);
}

void UART1IntHandler()
{
	limpiarUART(UART1);
	// LLENAR UN STRING
	leerStr(UART1,cadena);
	if (cadena[0]=='O')
	{
		estado = 1;
	}
	else if(cadena[0]=='F')
	{
		estado = 0;
	}
}

void ComparadorHandler()
{

}

/*----------------------------------------------------------------------------------*/
//                          	   PROGRAMA PRINCIPAL				 				//
int main(void)
{
	// CONFIGURAR EL RELOJ A LA FRECUENCIA MÁS ALTA ESTABLE
	configurarReloj(FRECUENCIA50MHZ);
	// HABILITAR LA FUNCIÓN PARA UTILIZAR LA FUNCION DELAY
	habilitarDelay();
	// HABILITAR EL LED RGB
	configurarRBG();
	IntMasterEnable();
	// LIMPIAR EL LED
	colorRBG(NEGRO);
	// HABILITAR UART A 9600 BAUDIOS
	configurarUART(UART1,9600);
	// HABILITAR EL ADC CANAL 1-PE3
	configurarADCInt1CH(CANAL_0);
	// CICLO
	while (1)
	{
		if (estado == 1)
		{
			/*
			lectura = leerADCInt(0);
			int2str(ECGnormal[conteo],cadena,3);
			UARTPrint(UART1,"V");
			UARTPrint(UART1,cadena);
			conteo = conteo + 1;
			if(conteo == 250)
			{
				conteo = 0;
			}
			delay(4*milis);
			*/
			lectura = leerADCInt(0)*0.05806;
			int2str(lectura,cadena,3);
			UARTPrint(UART1,"V");
			UARTPrint(UART1,cadena);
			delay(1*milis);
		}
		else
		{
			colorRBG(ROJO);
			delay(250*milis);
			colorRBG(NEGRO);
			delay(1500*milis);
		}
	} // FIN WHILE
}//main


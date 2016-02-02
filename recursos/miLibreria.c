/*---------------------------------------------------------------------
		 	 	 	 	 	 ARCHIVO C
		UPIBI 2015 LIBRERIAS PARA TIVAC TM4C123GXL - VERSION 4.0.1
		FECHA DE ULTIMA EDICIÓN: 		07/03/2015
=====================================================================*/


#include "miLibreria.h"
int moduloI2C = 0;
int lecturaUART = 0;

/*---------------------------------------------------------------------
				DECLARACIÓN DE VARIABLES GLOBALES				 */

short canales, secuencia, comparador, comparadorb;
/*					DECLARACIÓN DE VARIABLES GLOBALES
=====================================================================*/



/*---------------------------------------------------------------------
						LIBRARY DEL ADC				 		 		 */
/* PARA HACER UNA LECTURA DE ADC SE TIENE QUE:
 * LLAMAR configurarADCIntNCH DONDE N ES EL NUMERO DE CANALES A USAR
 * LLAMAR LA FUNCION leerADCInt Y DE PARAMETRO USAR LA POSICION
 * DEL CANAL QUE SE ASIGNO EN configurarADCIntNCH  */

int limpiarComparador(int SELECCION)
{
	/* ESTA FUNCION CHECA EL FLAG DEL COMPARADOR, Y LIMPIA
	 * EL FLAG DE LA INTERRUPCIÓN DE ESE COMPARADOR
	 * SE DEBE USAR DENTRO DEL HANDLER DE LA INTERRUPCION */
    int status = ADCComparatorIntStatus(ADC0);
    ADCComparatorIntClear(ADC0, status);
	ADCIntClear(ADC0, COMPARADOR);
	return status;
}

int leerComparador(int SELECCION)
{
	/* LEE EL VALOR DEL CANAL ASIGNADO PARA EL COMPARADOR EN
	 * EL MOMENTO EN QUE OCURRE LA INTERRUPCION
	 * EL PARAMETRO SELECCION PUEDE SER COMPARADOR O COMPARADORES
	 * NO EXISTE UN TIEMPO DE RETRASO MUY GRANDE
	 * SE RECOMIENDA NO EMPLEARLA Y MEJOR LEER OTRO CANAL DE ADC
	 * CONECTADO AL MISMO PUNTO DEL COMPARADOR	 */
	ADCSequenceDataGet(ADC0,SELECCION, valor);
	return valor[0];
}

void configurarComparador(int CANAL,
		int LIMINFERIOR, int LIMSUPERIOR, uint32_t CONDICION, int PRIORIDAD)
{
	/* CANALES DISPONIBLES: DESDE 0 HASTA 11
	 * EVITAR TRASLAPAR CON LOS CANALES DE OTROS COMPARADORES CONFIGURADOS
	 * LIMITES DISPONIBLES: DESDE0 HASTA 4095
	 * PRIODIDAD: DESDE 0(MAXIMA) HASTA 4(MINIMA)
	 * CONDICIONES DE INTERRUPCIÓN:
	 * 	NUNCA
	 * 	SIEMPRE_BAJO, UNA_BAJO,
	 * 	HISTERESIS_SIEMPRE_BAJO, HISTERESIS_UNA_BAJO
	 * 	SIEMPRE_ENMEDIO, UNA_ENMEDIO
	 * 	SIEMPRE_ALTO, UNA_ALTO,
	 * 	HISTERESIS_SIEMPRE_ALTO,HISTERESIS_UNA_ALTO	 */
	// SE USA EL COMPARADOR 2 POR DEFAULT
	int comparador = 2;
	// SE HABILITA EL RELOJ DEL ADC0
	SysCtlPeripheralEnable(RELOJ_ADC0);
	// SE EMPLEAR LA OPCION PARA LEER EN TODOMOMENTO
	ADCSequenceConfigure(ADC0, COMPARADOR, ADC_TRIGGER_ALWAYS, PRIORIDAD);
	// EL OVERSAMPLE DE 32 BITS PARA ESTABILIZAR LAS MEDICIONES
	ADCHardwareOversampleConfigure(ADC0, 64);
	// CONFIGURAR EL COMPARADOR COMO PASO 0 Y ASIGNAR EL CANAL
	ADCSequenceStepConfigure(ADC0, COMPARADOR,
			0,CANAL|(comparador*0x10000+0x80000));
	// SELECIONAR OTRO PASO DONDE SE INDICA
	// LA INTERRUPCION Y FINAL DE CONFIG
	ADCSequenceStepConfigure(ADC0, COMPARADOR,
			1,CANAL|ADC_CTL_IE| ADC_CTL_END);
	// ESTABLECER LA REGION DE COMPARADOR
	ADCComparatorRegionSet(ADC0, comparador, LIMINFERIOR, LIMSUPERIOR);
	// ELEGIR LA CONDICIÓN PARA GENERAR INTERRUPCION
	ADCComparatorConfigure(ADC0, comparador, CONDICION);
	// REINICIAR EL COMPARADOR
	ADCComparatorReset(ADC0, comparador, true, true);
	// HABILITAR LA INTERRUPCION DEL COMPARADOR
	ADCComparatorIntEnable(ADC0,COMPARADOR);
	// SELECCIONAR LA PRIORIDAD DE COMPARACION
	/* TODO IMPLEMENTAR 3 COMPARADORES EXTRAS MOVIENDO:
	 * COMPARADOR =  0, 1, 2, 3;
	 * SECUENCIA = 0, 1, 2, 3;
	 * INT_ADC0SS1, INT_ADC0SS1, INT_ADC0SS2, INT_ADC0SS3 */
	IntPrioritySet(INT_ADC0SS2, PRIORIDAD);
	// HABILITAR LA INTERRUPCION DE LA SECUENCIA 0
	IntEnable(INT_ADC0SS2);
	// HABILITAR LA SECUENCIA 0 COMO COMPARADOR
	ADCSequenceEnable(ADC0,COMPARADOR);
}

void configurarComparadores(int PRIMERCANAL, int SEGUNDOCANAL,
		int LIMITE1, int LIMITE2, int LIMITE3, int LIMITE4,
		uint32_t CONDICION1, uint32_t CONDICION2)
{
	/* CANALES DISPONIBLES: DESDE 0 HASTA 11
	 * EVITAR TRASLAPAR CON LOS CANALES DE OTROS
	 * COMPARADORES CONFIGURADOS
	 * SE PUEDEN REPETIR LOS CANALES PARA TENER 2
	 * CONDICIONES DE COMPARACION
	 * LIMITES DISPONIBLES: DESDE0 HASTA 4095
	 * PRIODIDAD: DESDE 0(MAXIMA) HASTA 4(MINIMA)
	 * CONDICIONES DE INTERRUPCIÓN:
	 * 	NUNCA
	 * 	SIEMPRE_BAJO, UNA_BAJO,
	 * 	HISTERESIS_SIEMPRE_BAJO, HISTERESIS_UNA_BAJO
	 * 	SIEMPRE_ENMEDIO, UNA_ENMEDIO
	 * 	SIEMPRE_ALTO, UNA_ALTO,
	 * 	HISTERESIS_SIEMPRE_ALTO,HISTERESIS_UNA_ALTO	 */
	// SE USA EL COMPARADOR 0 Y 1 POR DEFAULT
	int comparadora = 0;
	int comparadorb = 1;
	SysCtlPeripheralEnable(RELOJ_ADC0);
	// SE EMPLEA SOLO UNA SECUENCIA CON LOS 2 COMPARADORES
	ADCSequenceConfigure(ADC0, COMPARADORES, ADC_TRIGGER_ALWAYS, 0x00);
	ADCHardwareOversampleConfigure(ADC0, 64);
	// SE ASIGNA EL CANAL AL COMPARADOR A
	ADCSequenceStepConfigure(ADC0, COMPARADORES,0,
			PRIMERCANAL|(comparadora*0x10000+0x80000));
	// SE ASIGNA EL CANAL AL COMPARADOR B
	ADCSequenceStepConfigure(ADC0, COMPARADORES,1,
			SEGUNDOCANAL|(comparadorb*0x10000+0x80000));
	// SE HABILITAN LAS INTERRUPCIONES Y DELIMITA LA SECUENCIA
	ADCSequenceStepConfigure(ADC0, COMPARADORES,2,
			PRIMERCANAL|SEGUNDOCANAL|ADC_CTL_IE| ADC_CTL_END);
	// SE ELIGEN LAS REGIONES Y CONDICIONES DE INTERRUPCION Y RESETEA
	ADCComparatorRegionSet(ADC0, comparadora, LIMITE1, LIMITE2);
	ADCComparatorConfigure(ADC0, comparadora, CONDICION1);
	ADCComparatorReset(ADC0, comparadora, true, true);
	ADCComparatorRegionSet(ADC0, comparadorb, LIMITE3, LIMITE4);
	ADCComparatorConfigure(ADC0, comparadorb, CONDICION2);
	ADCComparatorReset(ADC0, comparadorb, true, true);
	// SE HABILITA LA INTERRUPCION DE LA SECUENCIA ELEGIDA
	ADCComparatorIntEnable(ADC0,COMPARADORES);
	// SE ASIGNA LA PRIORIDAD
	IntPrioritySet(INT_ADC0SS1, 0x00);
	// SE HABILITA LA INTERRUPCION DE ESA SECUENCIA
	IntEnable(INT_ADC0SS1);
	// HABILITAR LA SECUENCIA
	ADCSequenceEnable(ADC0,COMPARADORES);
}

void ADC1IntHandler()
{
	/* SE LLAMA CUANDO LLEGA LA INTERRUPCIÓN DE LECTURA DE ADC
	 * SE ALMACENA EN EL BUFFER LA LECTURA DE 1 U 8 CANALES
	 * SE REQUIERE LLAMAR LA FUNCION LEER PARA RECUPERAR ESTE VALOR
	 */
    ADCIntClear(ADC1, secuencia);
    if (canales == 1)
    {
        ADCSequenceDataGet(ADC1, secuencia, ADC11CH);
    }
    else if (canales == 2)
    {
        ADCSequenceDataGet(ADC1, secuencia, ADC12CH);
    }
    else if (canales == 4)
    {
        ADCSequenceDataGet(ADC1, secuencia, ADC14CH);
    }
    else if (canales == 8)
    {
        ADCSequenceDataGet(ADC1, secuencia, ADC18CH);
    }
}

int leerADCInt(short POSICION_CANAL)
{
	// 73 ciclos de reloj
	/* SE PIDE LA POSICION DEL CANAL ASIGNADO EN LA FUNCION
	 * SEGUN LA FUNCION CONFIGURARA SE PUEDEN TENER DE 0 A 8 CANALES
	 * CONFIGURAR, SE RECUPERAR UN VALOR ENTERO DE 0 A 4096	 */
    ADCProcessorTrigger(ADC1, secuencia);
    switch ( canales )
    {
    case 1:
        return ADC11CH[POSICION_CANAL];
    case 2:
        return ADC12CH[POSICION_CANAL];
    case 3:
        return ADC14CH[POSICION_CANAL];
    case 4:
        return ADC14CH[POSICION_CANAL];
    default:
    	return -1;
    }
}

void configurarADCInt1CH(int CANAL)
{
	// POSICIONES DE CANAL DISPONIBLES [0];
	canales = 1;
	secuencia = SECUENCIADOR_1MUESTRAS;
	// COMPROBAR CANALES CORRECTOS
	if(CANAL < 0 && CANAL > 11){while(1){colorRBG(AMARILLO);}}
	// SE EMPLEA EL MODULO ADC1, Y EL SECUENCIADOR DE 1 MUESTRA
	// HABILITAR RELOJ
    SysCtlPeripheralEnable(RELOJ_ADC1);
    //OVERSAMPLING PARA MAYOR EXACTITUD
	ADCHardwareOversampleConfigure(ADC1, 64);
	// CONFIGURAR MODO DE LECTURA
	ADCSequenceConfigure(ADC1, SECUENCIADOR_1MUESTRAS, TRIGGER_PROCESSOR, 1);
	// CONFIGURAR CANALES, INTERRUPCIONES Y SECUENCIADOR_1MUESTRAS DE LECTURA
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_1MUESTRAS, 0, CANAL| ADC_CTL_IE
			|ADC_CTL_END);

	// INTERRUPCION
	ADCIntEnable(ADC1, SECUENCIADOR_1MUESTRAS);			// HABILITAR EL ADC1
	// HABILITAR LA INTERRUPCION DEL ADC1 Y SECUENCIADOR_1MUESTRAS CORRESPONDIENTE
	IntEnable(INT_ADC1SS3);
	// HABILITAR LA SECUENCIADOR_1MUESTRAS CORRESPONDIENTE
	ADCSequenceEnable(ADC1, SECUENCIADOR_1MUESTRAS);
	// HABILITAR LA INTERRUPCION
	ADCIntClear(ADC1, SECUENCIADOR_1MUESTRAS);
	// PRIORIDAD BAJA ?
	IntPrioritySet(INT_ADC1SS3, 0);
}

void configurarADCInt2CH(int CANAL1, int CANAL2)
{
	// POSICIONES DE CANAL DISPONIBLES [0] Y [1];
	canales = 2;
	secuencia = SECUENCIADOR_2MUESTRAS;
	// HABILITAR RELOJ
    SysCtlPeripheralEnable(RELOJ_ADC1);
    //OVERSAMPLING PARA MAYOR EXACTITUD
	ADCHardwareOversampleConfigure(ADC1, 64);
	// CONFIGURAR MODO DE LECTURA
	ADCSequenceConfigure(ADC1, SECUENCIADOR_2MUESTRAS, TRIGGER_PROCESSOR, 1);
	// CONFIGURAR CANALES, INTERRUPCIONES Y SECUENCIADOR_2MUESTRAS DE LECTURA
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_2MUESTRAS, 0, CANAL1);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_2MUESTRAS, 1, CANAL2| ADC_CTL_IE
			|ADC_CTL_END);
	// INTERRUPCION
	ADCIntEnable(ADC1, SECUENCIADOR_2MUESTRAS);			// HABILITAR EL ADC1
	// HABILITAR LA INTERRUPCION DEL ADC1 Y SECUENCIADOR_2MUESTRAS CORRESPONDIENTE
	IntEnable(INT_ADC1SS2);
	// HABILITAR LA SECUENCIADOR_2MUESTRAS CORRESPONDIENTE
	ADCSequenceEnable(ADC1, SECUENCIADOR_2MUESTRAS);
	// HABILITAR LA INTERRUPCION
	ADCIntClear(ADC1, SECUENCIADOR_2MUESTRAS);
	// PRIORIDAD BAJA ?
	IntPrioritySet(INT_ADC1SS3, 0);
}

void configurarADCInt4CH(int CANAL1, int CANAL2, int CANAL3, int CANAL4)
{
	// POSICIONES DE CANAL DISPONIBLES [0] HASTA [3];
	canales = 4;
	secuencia = SECUENCIADOR_4MUESTRAS;
	// HABILITAR RELOJ
    SysCtlPeripheralEnable(RELOJ_ADC1);
    //OVERSAMPLING PARA MAYOR EXACTITUD
	ADCHardwareOversampleConfigure(ADC1, 64);
	// CONFIGURAR MODO DE LECTURA
	ADCSequenceConfigure(ADC1, SECUENCIADOR_4MUESTRAS, TRIGGER_PROCESSOR, 1);
	// CONFIGURAR CANALES, INTERRUPCIONES Y SECUENCIADOR_4MUESTRAS DE LECTURA
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_4MUESTRAS, 0, CANAL1);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_4MUESTRAS, 1, CANAL2);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_4MUESTRAS, 2, CANAL3);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_4MUESTRAS, 3, CANAL4| ADC_CTL_IE
			|ADC_CTL_END);

	// INTERRUPCION
	ADCIntEnable(ADC1, SECUENCIADOR_4MUESTRAS);			// HABILITAR EL ADC1
	// HABILITAR LA INTERRUPCION DEL ADC1 Y SECUENCIADOR_4MUESTRASD
	IntEnable(INT_ADC1SS1);
	// HABILITAR LA SECUENCIADOR_4MUESTRASDOR_4MUESTRAS CORRESPONDIENTE
	ADCSequenceEnable(ADC1, SECUENCIADOR_4MUESTRAS);
	// HABILITAR LA INTERRUPCION
	ADCIntClear(ADC1, SECUENCIADOR_4MUESTRAS);
	// PRIORIDAD BAJA ?
	IntPrioritySet(INT_ADC1SS3, 0);
}

void configurarADCInt8CH(int CANAL1, int CANAL2, int CANAL3, int CANAL4,
		int CANAL5, int CANAL6, int CANAL7, int CANAL8)
{
	// POSICIONES DE CANAL DISPONIBLES [0] HASTA [7];
	canales = 8;
	secuencia = SECUENCIADOR_8MUESTRAS;
	// HABILITAR RELOJ
    SysCtlPeripheralEnable(RELOJ_ADC1);
    //OVERSAMPLING PARA MAYOR EXACTITUD
	ADCHardwareOversampleConfigure(ADC1, 64);
	// CONFIGURAR MODO DE LECTURA
	ADCSequenceConfigure(ADC1, SECUENCIADOR_8MUESTRAS, TRIGGER_PROCESSOR, 1);
	// CONFIGURAR CANALES, INTERRUPCIONES Y SECUENCIADOR_8MUESTRAS DE LECTURA
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_8MUESTRAS, 0, CANAL1);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_8MUESTRAS, 1, CANAL2);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_8MUESTRAS, 2, CANAL3);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_8MUESTRAS, 3, CANAL4);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_8MUESTRAS, 4, CANAL5);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_8MUESTRAS, 5, CANAL6);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_8MUESTRAS, 6, CANAL7);
	ADCSequenceStepConfigure(ADC1, SECUENCIADOR_8MUESTRAS, 7, CANAL8|
			ADC_CTL_IE |ADC_CTL_END);
	// INTERRUPCION
	ADCIntEnable(ADC1, SECUENCIADOR_8MUESTRAS);			// HABILITAR EL ADC1
	// HABILITAR LA INTERRUPCION DEL ADC1 Y SECUENCIADOR_8MUESTRAS CORRESPONDIENTE
	IntEnable(INT_ADC1SS0);
	// HABILITAR LA SECUENCIADOR_8MUESTRAS CORRESPONDIENTE
	ADCSequenceEnable(ADC1, SECUENCIADOR_8MUESTRAS);
	// HABILITAR LA INTERRUPCION
	ADCIntClear(ADC1, SECUENCIADOR_8MUESTRAS);
	// PRIORIDAD BAJA ?
	IntPrioritySet(INT_ADC1SS3, 0);
}

/*							LIBRARY DEL ADC
=====================================================================*/



/*---------------------------------------------------------------------
						LIBRARY DE GPIO			 		 		     */
// PUERTOS DISPONIBLES: DESDE A HASTA F
// PINES DISPONIBLES: DESDE 0 HASTA 7 EN ALGUNOS CASOS*
// TIPOS DISPONIBLES: ENTRADA O SALIDA

void configurarPuerto(uint32_t PUERTO, uint8_t PINES, uint32_t TIPO)
{
	/* SE REQUIERE INICIAR EL PUERTO CON LOS RESPECTIVOS PINES
	 * ELIGIENDO EL TIPO DE ENTRADA O SALIDA
	 * POR DEFAULT, LOS PUERTOS SON DIGITALES 	 */

	switch (PUERTO)
	{
		case PUERTO_A:
			SysCtlPeripheralEnable(RELOJ_PUERTO_A);
			break;
		case PUERTO_B:
			SysCtlPeripheralEnable(RELOJ_PUERTO_B);
			break;
		case PUERTO_C:
			SysCtlPeripheralEnable(RELOJ_PUERTO_C);
			break;
		case PUERTO_D:
			SysCtlPeripheralEnable(RELOJ_PUERTO_D);
			break;
		case PUERTO_E:
			SysCtlPeripheralEnable(RELOJ_PUERTO_E);
			break;
		case PUERTO_F:
			SysCtlPeripheralEnable(RELOJ_PUERTO_F);
			break;
		default:
			while(1)
			{
				colorRBG(AMARILLO);
			}
	}

	// SELECCIONAR TIPO
	switch (TIPO)
	{
		case ENTRADA:
			GPIOPinTypeGPIOInput(PUERTO,PINES);
			break;
		case SALIDA:
			GPIOPinTypeGPIOOutput(PUERTO,PINES);
			break;
		default:
			while(1)
			{
				colorRBG(AMARILLO);
			}
	}
}

void escribirPuerto(uint32_t PUERTO, uint8_t PINES, uint8_t VALOR)
{
	/* PARA LOGRAR LA ESCRITURA CORRECTA, SOLO SE ESCRIBEN LOS PINES
	 * QUE FUERON CONFIGURADOS EN LA FUNCION configurarPuerto
	 * EL VALOR MÁXIMO QUE SE LOGRA ES 255	 */
	GPIOPinWrite(PUERTO, PINES, VALOR);
}

int leerPuerto(uint32_t PUERTO, uint8_t PINES)
{
	/* PARA LOGRAR LA LECTURA CORRECTA, SOLO SE ESCRIBEN LOS PINES
	 * QUE FUERON CONFIGURADOS EN LA FUNCION configurarPuerto
	 * EL VALOR MÁXIMO QUE SE RECUPERA ES 255	 */
	return GPIOPinRead(PUERTO, PINES);
}

void configurarRBG()
{
	// EL PUERTO QUE PERMITE LOS COLORES ES EL F,
	// EN SUS PRIMEROS 3 PINES
	SysCtlPeripheralEnable(RELOJ_PUERTO_F);
	GPIOPinTypeGPIOOutput(PUERTO_F,PIN_1|PIN_2|PIN_3);
}

void colorRBG(uint32_t COLOR)
{
	/* COLORES DISPONIBLES: NEGRO, ROJO, AZUL, VERDE,
	 * COLORES DISPONIBLES: MORADO, AMARILLO, VERDEAZUL, BLANCO,
	 * SE DEBE INICIAR LA FUNCION configurarRBG	 */
	GPIOPinWrite(PUERTO_F, PIN_1|PIN_2|PIN_3, COLOR);
}
/*						LIBRARY DE GPIO
=====================================================================*/



/*---------------------------------------------------------------------
					LIBRARY DE INTERRUPCIONES		 		 		 */
// TODO IMPLEMENTAR MÁS INTERRUPCIONES
// NOTA*:  SE DEBE USAR 	"IntMasterEnable();" ANTES DEL WHILE
void configurarInterrupcion(uint32_t MODULO, int PRIORIDAD,
		uint32_t CONDICION)
{
	switch (MODULO)
	{
	case UART0:
		IntEnable(INT_UART0);
		UARTIntEnable(UART0_BASE, CONDICION);
		IntPrioritySet(INT_UART0 , PRIORIDAD);
		break;
	case UART1:
		IntEnable(INT_UART1);
		UARTIntEnable(UART1_BASE, CONDICION);
		IntPrioritySet(INT_UART1 , PRIORIDAD);
		break;
	case PUERTO_C:
		IntEnable(INT_GPIOC);
		 //ELEGIR EL TRIGGER Y LOS PINES
		GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, CONDICION);
		//HABILITAR LOS PINES Y CREAR FLAGS
		GPIOIntEnable(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
		IntPrioritySet(INT_GPIOC, PRIORIDAD);
		break;
	case TIMER0_BASE:
		break;
	default:
		while(1)
		{
			colorRBG(AMARILLO);
		}
	}
}
/*					LIBRARY DE INTERRUPCIONES
=====================================================================*/



/*---------------------------------------------------------------------
					LIBRARY DE RELOJ Y DELAY		 		 		 */
void configurarReloj(uint32_t FRECUENCIA)
{
	// TODO AGREGAR MAS FRECUENCIAS
	/* SE CONFIGURAR UNA FRECUENCIA VALIDA, Y SE ALMACENAN
	 * LOS CICLOS QUE EQUIVALEN A 1 SEGUNDO (FRECUENCIA)
	 * 1 MILISEGUNDO Y UN MICROSEGUNDO	 */
	switch (FRECUENCIA)
	{
		case FRECUENCIA4MHZ:
			SysCtlClockSet(SYSCTL_SYSDIV_50 |SYSCTL_USE_PLL |
					SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
			frecuencia = SysCtlClockGet();
			break;
		case FRECUENCIA8MHZ:
			SysCtlClockSet(SYSCTL_SYSDIV_25 |SYSCTL_USE_PLL |
					SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
			frecuencia = SysCtlClockGet();
			break;
		case FRECUENCIA16MHZ:
			SysCtlClockSet(SYSCTL_SYSDIV_12_5 |SYSCTL_USE_PLL |
					SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
			frecuencia = SysCtlClockGet();
			break;
		case FRECUENCIA20MHZ:
			SysCtlClockSet(SYSCTL_SYSDIV_10 |SYSCTL_USE_PLL |
					SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
			frecuencia = SysCtlClockGet();
			break;
		case FRECUENCIA40MHZ:
			SysCtlClockSet(SYSCTL_SYSDIV_5 |SYSCTL_USE_PLL |
					SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
			frecuencia = SysCtlClockGet();
			break;
		case FRECUENCIA50MHZ:
			SysCtlClockSet(SYSCTL_SYSDIV_4  |SYSCTL_USE_PLL |
					SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
			frecuencia = SysCtlClockGet();
			break;
		case FRECUENCIA80MHZ:
			SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL |
					SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
			frecuencia = SysCtlClockGet();
			break;
		default:
			error();
			break;
	}
	milis = frecuencia/1000;
	micros = milis/1000;
	wait = 0;	// identificador para delay
}

void habilitarDelay()
{
	/* SE REQUIERE LLAMAR ESTA FUNCION ANTES DE USAR
	 * LA FUNCION delay , CON ESTO SE OPTIMIZÓ
	 * EL TIEMPO DE EXECUION DE LA SEGUNDA FUNCION	 */
	// HABILITAR EL RELOJ
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	// CONFIGURAR PERIODO
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	// CONFIGURAR INTERRUPCION DEL TIMER
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	//
	TimerUpdateMode(TIMER0_BASE,TIMER_A,TIMER_UP_LOAD_IMMEDIATE);
	// ASIGNAR PRIORIDAD (NO LA MAXIMA)
	IntPrioritySet(INT_TIMER0A,1);
}

void delay(int CICLOS)
{
	// LAMAR habilitarDelay() EN LA CONFIGURACION
	TimerLoadSet(TIMER0_BASE, TIMER_A, CICLOS-240);
	IntEnable(INT_TIMER0A);
	// PRENDER EL TIMER
	TimerEnable(TIMER0_BASE, TIMER_A);
	// INICIAR FLAG DE DELAY
	while( wait == 0) //delay
	{
	}
	wait = 0;
}

void delay_us(uint32_t microsegundos)
{
	// FUNCION ALTERNATIVA, MENOS PRECISA EN OCACIONES*
	// PERMITE TENER DELAYS MENORES A 50us
	SysCtlDelay(microsegundos*micros/3);
}

void Timer0IntHandler(void)
{
	/* HANDLER NECESARIO PARA EMPLEAR LA FUNCION DE DELAY POR TIMER
	 * PERMITE REINICIAR Y LIMPIAR EL FLAG	 */
	// LIMPIAR LA INTERRUPCION
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	// CONFIGURAR EL CONTADOR
	wait = 1;
	// DESHABILITAR INTERRUPCION DEL TIMER
	IntDisable(INT_TIMER0A);
	// APAGAR EL TIMER
	TimerDisable(TIMER0_BASE, TIMER_A);
}
/*					LIBRARY DE RELOJ Y DELAY
=====================================================================*/


void habilitarI2C_1CH(int MODULO)
{
	switch(MODULO)
	{
	case I2C0:	//enable I2C module 0
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	    //enable GPIO peripheral that contains I2C 0
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	    // Configure the pin muxing for I2C0 functions on port B2 and B3.
	    GPIOPinConfigure(PB2_I2C0_SCL); //CAFE 14
	    GPIOPinConfigure(PB3_I2C0_SDA);	//ROJO  15
	    // Select the I2C function for these pins.
	    GPIOPinTypeI2CSCL(PUERTO_B, PIN_2);
	    GPIOPinTypeI2C(PUERTO_B, PIN_3);
	    // Enable and initialize the I2C0 master module.  Use the system clock for
	    // the I2C0 module.  The last parameter sets the I2C data transfer rate.
	    // If false the data rate is set to 100kbps and if true the data rate will
	    // be set to 400kbps.
	    I2CMasterInitExpClk(MODULO, SysCtlClockGet(), true);
	    moduloI2C = MODULO;
		break;
	}
}

void enviarI2C_1CH(int DIRECCION, int DATO)
{
	// NOTA, LA DIRECCIÓN ES DE 7 BITS.
	// NOTA LOS DATOS ESTAN INVERTIDOS PARA FACILIAR
    I2CMasterSlaveAddrSet(moduloI2C, DIRECCION, false);
    I2CMasterDataPut(moduloI2C, 255-DATO);
    I2CMasterControl(moduloI2C, I2C_MASTER_CMD_SINGLE_SEND);
}

/*---------------------------------------------------------------------
					LIBRARY DE COMUNICACION UART	 		 		 */
void configurarUART(uint32_t MODULO, uint32_t BAUDRATE)
{
	// TODOS LOS MODULOS IMPLEMENTADOS
	switch (MODULO)
	{
	case UART0:
		SysCtlPeripheralEnable(RELOJ_UART0);
		SysCtlPeripheralEnable(RELOJ_PUERTO_A);
		GPIOPinConfigure(U0RX_PA0);
		GPIOPinConfigure(U0TX_PA1);
		GPIOPinTypeUART(PUERTO_A, PIN_0 | PIN_1);
		IntEnable(INT_UART0);
		UARTIntEnable(UART0, RX|RT);
		IntPrioritySet(INT_UART0 , 0x01);
		UARTClockSourceGet(UART0);
		break;
	case UART1:
		SysCtlPeripheralEnable(RELOJ_UART1);
		SysCtlPeripheralEnable(RELOJ_PUERTO_B);
		GPIOPinConfigure(U1RX_PB0);
		GPIOPinConfigure(U1TX_PB1);
		GPIOPinTypeUART(PUERTO_B, PIN_0 | PIN_1);
		IntEnable(INT_UART1);
		UARTIntEnable(UART1, RX|RT);
		IntPrioritySet(INT_UART1 , 0x01);
		break;
	case UART2:
		SysCtlPeripheralEnable(RELOJ_UART2);
		SysCtlPeripheralEnable(RELOJ_PUERTO_D);
		GPIOPinConfigure(U2RX_PD6);
		GPIOPinConfigure(U2TX_PD7);
		GPIOPinTypeUART(PUERTO_D, PIN_6 | PIN_7);
		IntEnable(INT_UART2);
		UARTIntEnable(UART2, RX|RT);
		IntPrioritySet(INT_UART2 , 0x01);
		break;
	case UART3:
		SysCtlPeripheralEnable(RELOJ_UART3);
		SysCtlPeripheralEnable(RELOJ_PUERTO_C);
		GPIOPinConfigure(U3RX_PC6);
		GPIOPinConfigure(U3TX_PC7);
		GPIOPinTypeUART(PUERTO_C, PIN_6 | PIN_7);
		IntEnable(INT_UART3);
		UARTIntEnable(UART3, RX|RT);
		IntPrioritySet(INT_UART3 , 0x01);
		break;
	case UART4:
		SysCtlPeripheralEnable(RELOJ_UART4);
		SysCtlPeripheralEnable(RELOJ_PUERTO_C);
		GPIOPinConfigure(U4RX_PC4);
		GPIOPinConfigure(U4TX_PC5);
		GPIOPinTypeUART(PUERTO_C, PIN_5 | PIN_4);
		IntEnable(INT_UART4);
		UARTIntEnable(UART4, RX|RT);
		IntPrioritySet(INT_UART4 , 0x01);
		break;
	case UART5:
		SysCtlPeripheralEnable(RELOJ_UART5);
		SysCtlPeripheralEnable(RELOJ_PUERTO_E);
		GPIOPinConfigure(U5RX_PE4);
		GPIOPinConfigure(U5TX_PE5);
		GPIOPinTypeUART(PUERTO_E, PIN_4 | PIN_5);
		IntEnable(INT_UART5);
		UARTIntEnable(UART5, RX|RT);
		IntPrioritySet(INT_UART5 , 0x01);
		break;
	case UART6:
		SysCtlPeripheralEnable(RELOJ_UART6);
		SysCtlPeripheralEnable(RELOJ_PUERTO_D);
		GPIOPinConfigure(U6RX_PD4);
		GPIOPinConfigure(U6TX_PD5);
		GPIOPinTypeUART(PUERTO_D, PIN_4 | PIN_5);
		IntEnable(INT_UART6);
		UARTIntEnable(UART6, RX|RT);
		IntPrioritySet(INT_UART6 , 0x01);
		break;
	case UART7:
		SysCtlPeripheralEnable(RELOJ_UART7);
		SysCtlPeripheralEnable(RELOJ_PUERTO_E);
		GPIOPinConfigure(U7RX_PE0);
		GPIOPinConfigure(U7TX_PE1);
		GPIOPinTypeUART(PUERTO_E, PIN_0 | PIN_1);
		IntEnable(INT_UART7);
		UARTIntEnable(UART7, RX|RT);
		IntPrioritySet(INT_UART7 , 0x01);
		break;
	default:
		while(1)
		{
			colorRBG(AMARILLO);
		}
	}
	UARTConfigSetExpClk(MODULO, SysCtlClockGet(), BAUDRATE,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

void UARTPrint(uint32_t MODULO, const char *MENSAJE)
{
	/* MODULOS DISPONIBLES: UART0 Y UART1
	 * EL MENSAJE SE ESCRIBE ENTRE COMILLAS "HOLA"
	 * NO ENVIAR UN TERMINADOR, UTIL PARA CONTATENAR  */
	uint8_t ui8index;
	uint8_t ui8largo;
	// DETERMINA EL TAMAÑO DEL STRING " "
	for(ui8largo = 0; MENSAJE[ui8largo] != '\0'; ui8largo++)
	{}
	// ENVIA LOS CARACTERES
	for (ui8index = 0; ui8index<ui8largo;ui8index++)
	{
		UARTCharPut(MODULO, MENSAJE[ui8index]);
	}
}

void UARTPrintf(uint32_t MODULO, uint32_t MENSAJE)
{
	/* MODULOS DISPONIBLES: UART0 Y UART1
	 * ENVIA LA VARIABLE MENSAJE
	 * NO ENVIAR UN TERMINADOR, UTIL PARA CONTATENAR  */
	UARTCharPut(MODULO, MENSAJE);
}

void UARTPrintln(uint32_t MODULO, const char *MENSAJE)
{
	/* MODULOS DISPONIBLES: UART0 Y UART1
	 * EL MENSAJE SE ESCRIBE ENTRE COMILLAS "HOLA"
	 * ENVIAR UN TERMINADOR, UTIL PARA TERMINAR LA CADENA  */
	uint8_t ui8index;
	uint8_t ui8largo;
	//determina el tamaño del texto antes del zero
	for(ui8largo = 0; MENSAJE[ui8largo] != '\0'; ui8largo++)
	{}
	// envia los caracteres
	for (ui8index = 0; ui8index<ui8largo;ui8index++)
	{
		UARTCharPut(MODULO, MENSAJE[ui8index]);
	}
	//UARTCharPut(MODULO,13);	// CR  \r
	UARTCharPut(MODULO,10); // LN  \n
	UARTCharPut(MODULO,13); // LN  \n
}

void UARTPrintfln(uint32_t MODULO, uint32_t MENSAJE)
{
	/* MODULOS DISPONIBLES: UART0 Y UART1
	 * ENVIA LA VARIABLE MENSAJE
	 * ENVIAR UN TERMINADOR, UTIL PARA TERMINAR LA CADENA  */
	UARTCharPut(MODULO, MENSAJE);
	UARTCharPut(MODULO,13);	// CR  \r
	UARTCharPut(MODULO,10); // LN  \n
}

void limpiarUART(uint32_t MODULO)
{
	uint32_t ui32Status;
	switch (MODULO)
	{
		case UART0:
			ui32Status = UARTIntStatus(MODULO, true); //get interrupt status
			UARTIntClear(MODULO, ui32Status); //clear the asserted interrupts
			break;
		case UART1:
			ui32Status = UARTIntStatus(MODULO, true); //get interrupt status
			UARTIntClear(MODULO, ui32Status); //clear the asserted interrupts
			break;
		default:
			while(1)
			{
				colorRBG(AMARILLO);
			}
	}
}

int leerUART(uint32_t MODULO)
{
	int read;
	switch (MODULO)
	{
		case UART0:
			read = UARTCharGetNonBlocking(MODULO);
			break;
		case UART1:
			read = UARTCharGetNonBlocking(MODULO);
			break;
		default:
			while(1)
			{
				colorRBG(AMARILLO);
			}
	}
	return read;
}

void leerStr(uint32_t MODULO, char *cadena)
{
	/* ESTA VERSION AHORRA EL PARAMETRO TAMAÑO
	* LLENA EL PARAMETRO *CADENA HASTA QUE ENCUENTRE UN TERMINADOR
	* O HASTA SATURARLO.
	* CONDICION: LA LONGITUD DE LA CADENA DEBE SER MAYOR AL NUMERO
	* DE CARACTERES ESPERADOS.
	* EL LARGO DE LA CADENA SIN ENCONTRAR TERMINADOR ES DE 32
	* */
	short index = 0;
	short largo = 32;
	int lecturaUART = 0;
	// VALIDAR EL MODULO UART0 HASTA UART7
	int validar = (MODULO-0x40000000)/0x1000;
	if(!(validar>=12 && validar <=19))	{error();}

	// EMPEZAR A LLENAR LA CADENA
	while(UARTCharsAvail(MODULO))
	{
		lecturaUART = UARTCharGetNonBlocking(MODULO);
		if(lecturaUART == '\n' || lecturaUART == '\r')
		{
			break;
		}
		if(index < largo)
		{
			cadena[index] = lecturaUART;
		}
		index = index + 1;
	}
	// AGREGAR EL TERMINADOR  DE CADENA
	cadena[index] = '\0';
}
/*					LIBRARY DE COMUNICACION UART
=====================================================================*/




/*---------------------------------------------------------------------
					GENERICAS Y MISCELANEAS			 		 		 */
void int2str(int ENTERO, char * cadena , int digitos)
{
	/* LA NUEVA VERSION 3.0 PERMITE CONTROLAR LA LONGITUD DEL STRING CON
	 * EL PARAMETRO DIGITOS, SE LE DEBE PASAR COMO PARAMETRO EL APUNTADOR
	 * DEL ARREGLO DONDE SE GUARDARÁ EL STRING DE IGUAL O MAYOR LONGITUD
	 * cadena[digitos]
	 * */
	int potencia = 0;	//acumula el multiplicador
	short j = digitos-1;	//limite de orden
	short i = 0;		// contador
	int resultado = 0;	// aqui se guarda el resultado

	for(i=0; i <= j; i++)
	{
		potencia = pow(10,j-i);
		resultado = ENTERO/potencia;
		ENTERO = ENTERO - resultado*potencia;
		cadena[i]=resultado + 48;
	}
}

int str2int(char *cadena, int desfase)
{
	/* LA NUEVA VERSION PERMITE DEVOLVER EL VALOR ENTERO DEL STRING DADO,
	 * TODO EN VERSIONES FUTURAS PUEDE SER POSIBLE QUITAR EL PARAMETRO largo
	 * EL DESFASE ES EL NUMERO DE CARACTERES QUE VA A IGNORAR PARA INICAR LA
	 * CONVERSION EJEMPLO A100, DAR DESFASE DE 1 PARA IGNORAR LA A.
	 * ESTA VERSION INTENTARÁ ELIMINAR EL PARAMETRO DE largo
	 *
	 */
	short largo = 0;
	int regreso = 0;
	short i = 0;
	short j = 0;
	int entero = 0;
	int potencia = 0;
	// determinar el tamaño del parametro *cadena
	for(largo = 0; cadena[largo] != '\0'; largo++)
	{	}

	largo = largo - desfase;

	j = largo - 1;
	for(i=0+desfase;i<=largo+desfase;i++)
	{
		potencia = pow(10,j);
		entero = cadena[i] - 48;
		regreso = regreso + entero*potencia;
		j = j - 1;
	}
	return regreso;
}

void error()
{
	while(1)
	{
		colorRBG(AMARILLO);
	}
}
/*						GENERICAS Y MISCELANEAS
=====================================================================*/

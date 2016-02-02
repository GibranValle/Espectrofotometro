/*------------------------------------------------------------------------------
   DECLARACION DE VARIABLES GLOBALES ACCESIBLES EN TOO EL PROYECTO   */
int wait; 				// FLAG PARA EL DELAY POR INTERRUPCION DE TIMER
int frecuencia;
int milis;
int micros;
unsigned int valor[4];
// IDENTIFICADOR DE SECUENCIA DEL COMPARADOR ADC0S2
static int COMPARADORES = 1;
static int COMPARADOR = 2;
// VARIABLES PARA ADC CON INTERRUPCION
unsigned int ADC11CH[1];
unsigned int ADC12CH[2];
unsigned int ADC14CH[4];
unsigned int ADC18CH[8];
// VARIABLES PARA I2C

/*	DECLARACION DE VARIABLES GLOBALES ACCESIBLES EN TOO EL PROYECTO
==============================================================================*/




/*------------------------------------------------------------------------------
   	   	   	   	   	   	   	     MACROS PERSONALIZADOS 						  */

/* 										ADC				    				  */
		//RELOJ
#define RELOJ_ADC0      				0xf0003800  // ADC 0
#define RELOJ_ADC1      				0xf0003801  // ADC 1
		//MODULOS
#define ADC0               				0x40038000  // ADC0
#define ADC1               				0x40039000  // ADC1
		//PARAMEROS ui32Config
#define HABILITAR_INT           		0x00000040  // Interrupt enable
#define FIN_SECUENCIA           		0x00000020  // Sequence end select
#define SELECT_DIFERENCIAL      		0x00000010  // Differential select
		// CANALES DISPONIBLES
#define SENSOR_TEMPERATURA     			0x00000080  // Temperature sensor
#define CANAL_0             			0x00000000  // PE3
#define CANAL_1             			0x00000001  // PE2
#define CANAL_2             			0x00000002  // PE1
#define CANAL_3             			0x00000003  // PE0
#define CANAL_4             			0x00000004  // PD3
#define CANAL_5             			0x00000005  // PD2
#define CANAL_6             			0x00000006  // PD1
#define CANAL_7             			0x00000007  // PD0
#define CANAL_8             			0x00000008  // PE5
#define CANAL_9             			0x00000009  // PE4
#define CANAL_10            			0x0000000A  // PB4
#define CANAL_11            			0x0000000B  // PB5
		//TRIGGERS
#define TRIGGER_PROCESSOR   			0x00000000  // Processor event
#define TRIGGER_COMP0       			0x00000001  // Analog comparator 0 event
#define TRIGGER_COMP1       			0x00000002  // Analog comparator 1 event
#define TRIGGER_COMP2       			0x00000003  // Analog comparator 2 event
#define TRIGGER_EXTERNAL    			0x00000004  // External event
#define TRIGGER_TIMER       			0x00000005  // Timer event
#define TRIGGER_PWM0        			0x00000006  // PWM0 event
#define TRIGGER_PWM1        			0x00000007  // PWM1 event
#define TRIGGER_PWM2        			0x00000008  // PWM2 event
#define TRIGGER_PWM3        			0x00000009  // PWM3 event
#define TRIGGER_NEVER       			0x0000000E  // Never Trigger
#define TRIGGER_ALWAYS      			0x0000000F  // Always event
#define TRIGGER_PWM_MOD0    			0x00000000  // PWM triggers from PWM0
#define TRIGGER_PWM_MOD1    			0x00000010  // PWM triggers from PWM1
		//SECUENCIAS
#define SECUENCIADOR_8MUESTRAS  		0X00000000
#define SECUENCIADOR_4MUESTRAS  		0X00000001
#define SECUENCIADOR_2MUESTRAS  		0X00000002
#define SECUENCIADOR_1MUESTRAS  		0X00000003
//sequencer zero captures
//! up to eight samples, sequencers one and two capture up to four samples,
//! and sequencer three captures a single sample.
		// MACROS DE CONDICION DE INTERRUPCION
#define NUNCA			      			0x00000000  // Interrupt Disabled
#define SIEMPRE_BAJO			 		0x00000010  // Interrupt Low Always
#define UNA_BAJO				   		0x00000011  // Interrupt Low Once
#define HISTERESIS_SIEMPRE_BAJO			0x00000012  // Interrupt Low Always (Hysteresis)
#define HISTERESIS_UNA_BAJO		 		0x00000013  // Interrupt Low Once (Hysteresis)
#define SIEMPRE_ENMEDIO			 		0x00000014  // Interrupt Mid Always
#define UNA_ENMEDIO				   		0x00000015  // Interrupt Mid Once
#define SIEMPRE_ALTO	 				0x0000001C  // Interrupt High Always
#define UNA_ALTO				  		0x0000001D  // Interrupt High Once
#define HISTERESIS_SIEMPRE_ALTO 		0x0000001E  // Interrupt High Always	(Hysteresis)
#define HISTERESIS_UNA_ALTO		 		0x0000001F  // Interrupt High Once (Hysteresis)
		// COMPARADORES DISPONIBLES
#define COMPARADOR0            			0x00080000  // Select Comparator 0
#define COMPARADOR1            			0x00090000  // Select Comparator 1
#define COMPARADOR2            			0x000A0000  // Select Comparator 2
#define COMPARADOR3           			0x000B0000  // Select Comparator 3
#define COMPARADOR4            			0x000C0000  // Select Comparator 4
#define COMPARADOR5            			0x000D0000  // Select Comparator 5
#define COMPARADOR6            			0x000E0000  // Select Comparator 6
#define COMPARADOR7            			0x000F0000  // Select Comparator 7
/* 										ADC				    				  */


		//   *******************   GPIO   ********************  //

/*      				PINES DISPONIBLES SEGUN EL PUERTO

							B0		D0	E0	F0
							B1		D1	E1	F1
						A2	B2		D2	E2	F2
						A3	B3		D3	E3	F3
						A4	B4	C4		E4	F4
						A5	B5	C5		E5
						A6	B6	C6	D6
						A7	B7	C7	D7		                           */
		// PUERTOS DISPONIBLES
#define PUERTO_A						0x40004000
#define PUERTO_B						0x40005000  //5,0,1,
#define PUERTO_C	        			0x40006000
#define PUERTO_D						0x40007000
#define PUERTO_E						0x40024000
#define PUERTO_F						0x40025000
		// PINES
#define PIN_0							0x00000001
#define PIN_1							0x00000002
#define PIN_2							0x00000004
#define PIN_3							0x00000008
#define PIN_4							0x00000010
#define PIN_5							0x00000020
#define PIN_6							0x00000040
#define PIN_7							0x00000080
#define PIN_TODOS						0x000000FF
		// COLORES DISPONIBLES DEL LED RBG
#define NEGRO 							0x00000001
#define ROJO 							0x00000002
#define AZUL 							0x00000004
#define VERDE 							0x00000008
#define MORADO 							PIN_1|PIN_2
#define AMARILLO						PIN_1|PIN_3
#define VERDEAZUL						PIN_2|PIN_3
#define BLANCO							PIN_1|PIN_2|PIN_3
		// RELOJ
#define RELOJ_PUERTO_A					0xf0000800
#define RELOJ_PUERTO_B     				0xf0000801
#define RELOJ_PUERTO_C    				0xf0000802
#define RELOJ_PUERTO_D    				0xf0000803
#define RELOJ_PUERTO_E     				0xf0000804
#define RELOJ_PUERTO_F     				0xf0000805
		// TIPO DE PUERTO
#define ENTRADA							0x00000011
#define SALIDA							0x00000012
		//   --------------------   GPIO   --------------------  //


		//   **********   FRECUENCIAS DISPONIBLES   ***********  //
#define FRECUENCIA4MHZ						0x03D0900
#define FRECUENCIA8MHZ						0x07A1200
#define FRECUENCIA16MHZ						0x0F42400
#define FRECUENCIA20MHZ						0x1312D00
#define FRECUENCIA40MHZ						0x2625A00
#define FRECUENCIA50MHZ						0x2FAF080
#define FRECUENCIA80MHZ						0x4C4B400
		//   ---------    FRECUENCIAS DISPONIBLES   ----------  //


		//   *****************      UART      *****************  //
		//BAUD RATE
#define BAUD1200						0x000004B0
#define BAUD2400						0x00000960
#define BAUD4800						0x000012C0
#define BAUD9600						0x00002580
#define BAUD19200						0x00004B00
#define BAUD38400						0x00009600
#define BAUD57600						0x0000E100
#define BAUD115200						0x0001C200
		//RELOJ
#define RELOJ_UART0     	 			0xf0001800
#define RELOJ_UART1    		     		0xf0001801
#define RELOJ_UART2    					0xf0001802
#define RELOJ_UART3     				0xf0001803
#define RELOJ_UART4     				0xf0001804
#define RELOJ_UART5    					0xf0001805
#define RELOJ_UART6     				0xf0001806
#define RELOJ_UART7     				0xf0001807
		//MODULOS
#define UART0            		  		0x4000C000
#define UART1           		   		0x4000D000
#define UART2            		  		0x4000E000
#define UART3           		   		0x4000F000
#define UART4           		   		0x40010000
#define UART5           		 		0x40011000
#define UART6            		  		0x40012000
#define UART7	             	 		0x40013000
		// PUERTOS_UART
#define PUERTO_UART0            		0x00000000
#define PUERTO_UART1           		   	0x00000001
#define PUERTO_UART2            		0x00000002
#define PUERTO_UART3           		   	0x00000003
#define PUERTO_UART4           		   	0x00000004
#define PUERTO_UART5           		 	0x00000005
#define PUERTO_UART6            		0x00000006
#define PUERTO_UART7	             	0x00000007
		// PINES
#define U0RX_PA0           				0x00000001//1
#define U0TX_PA1           				0x00000401//1
#define U1RX_PB0           				0x00010001//2
#define U1TX_PB1           				0x00010401//2
#define U2RX_PD6           				0x00031801//3
#define U2TX_PD7           				0x00031C01//3
#define U3RX_PC6           				0x00021801//4
#define U3TX_PC7           				0x00021C01//4
#define U4RX_PC4           				0x00021001//5
#define U4TX_PC5           				0x00021401//5
#define U5RX_PE4           				0x00041001//6
#define U5TX_PE5           				0x00041401//6
#define U6RX_PD4           				0x00031001//7
#define U6TX_PD5           				0x00031401//7
#define U7RX_PE0           				0x00040001//8
#define U7TX_PE1           				0x00040401//8
		//   -----------------      UART      -----------------  //



//   				**************      I2C     	 ***************   //
// modulos
#define I2C0               		0x40020000  // I2C0
#define I2C1              		0x40021000  // I2C1
#define I2C2       				0x40022000  // I2C2
#define I2C3	   				0x40023000  // I2C3
#define I2C4               		0x400C0000  // I2C4
#define I2C5               		0x400C1000  // I2C5
#define I2C6               		0x400C2000  // I2C6
#define I2C7               		0x400C3000  // I2C7
#define I2C8    	        	0x400B8000  // I2C8
#define I2C9       	     		0x400B9000  // I2C9

// pines
#define PB2_I2C0_SCL        0x00010803
#define PB3_I2C0_SDA        0x00010C03


#define PA6_I2C1_SCL        	0x00001803
#define PA7_I2C1_SDA        	0x00001C03

#define PE4_I2C2_SCL        	0x00041003
#define PE5_I2C2_SDA        	0x00041403

#define PD0_I2C3_SCL        	0x00030003
#define PD1_I2C3_SDA        	0x00030403

#define PG2_I2C4_SCL        	0x00060803
#define PG3_I2C4_SDA        	0x00060C03

#define PG6_I2C5_SCL        	0x00061803
#define PG7_I2C5_SDA        	0x00061C03

#define PA6_I2C6_SCL        	0x00001802
#define PA7_I2C6_SDA        	0x00001C02

#define PD0_I2C7_SCL        	0x00030002
#define PD1_I2C7_SDA        	0x00030402

#define PD2_I2C8_SCL        	0x00030802
#define PD3_I2C8_SDA        	0x00030C02

#define PE6_I2C9_SCL        	0x00041802
#define PE7_I2C9_SDA        	0x00041C02
//   				---------------     I2C           --------------  //









		//   **************      INTERRUPT      ***************  //
		// CONDICIONES DE INT POR GPIO
#define FLANCO_NEGATIVO    	   			0x00000000  // Interrupt on falling edge
#define FLANCO_POSITIVO        			0x00000004  // Interrupt on rising edge
#define AMBOS_FLANCOS        			0x00000001  // Interrupt on both edges
#define NIVEL_BAJO          			0x00000002  // Interrupt on low level
#define NIVEL_ALTO         				0x00000006  // Interrupt on high level
#define GPIO_DISCRETE_INT       		0x00010000  // Interrupt for individual pins
		// PUERTO DE INTERRUPCION
#define INT_PUERTO_A					0X0000000A
#define INT_PUERTO_B					0X0000000B
#define INT_PUERTO_C					0X0000000C
#define INT_PUERTO_D					0X0000000D
#define INT_PUERTO_E					0X0000000E
#define INT_PUERTO_F					0X0000000F
		// CONDICIONES UART
#define RT           					0x00000040       // Receive Timeout Interrupt Mask
#define TX             					0x00000020       // Transmit Interrupt Mask
#define RX             					0x00000010       // Receive Interrupt Mask
		//   --------------      INTERRUPT      --------------   //

/*--------------------------------------------------------------------------------*/



//  ------------------         INCLUDES TIVAWARE	        ----------------------//
/**********************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"
#include <math.h>
#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"

/*--------------------------------------------------------------------------------*/




//  ------------------         PROTOTIPOS DE FUNCIONES        --------------------//
/**********************************************************************************/

		//   ********************   ADC   ********************  //
int limpiarComparador(int SELECCION);
int leerComparador(int SELECCION);
void configurarComparador(int CANAL,
		int LIMINFERIOR, int LIMSUPERIOR, uint32_t CONDICION, int PRIORIDAD);
void configurarComparadores(int PRIMERCANAL, int SEGUNDOCANAL,
		int LIMITE1, int LIMITE2, int LIMITE3, int LIMITE4,
		uint32_t CONDICION1, uint32_t CONDICION2);
int leerADCInt(short POSICION_CANAL);
void configurarADCInt1CH(int CANAL);
void configurarADCInt2CH(int CANAL1, int CANAL2);
void configurarADCInt4CH(int CANAL1, int CANAL2, int CANAL3, int CANAL4);
void configurarADCInt8CH(int CANAL1, int CANAL2, int CANAL3, int CANAL4,
		int CANAL5, int CANAL6, int CANAL7, int CANAL8);

		//   --------------------   ADC   --------------------  //


		//   *******************   GPIO   ********************  //
void configurarPuerto(uint32_t PUERTO, uint8_t PINES, uint32_t TIPO);
void escribirPuerto(uint32_t PUERTO, uint8_t PINES, uint8_t VALOR);
int leerPuerto(uint32_t PUERTO, uint8_t PINES);
void configurarRBG();
void colorRBG(uint32_t COLOR);
		//   -------------------   GPIO   --------------------  //


		//   ****************    INTERRUPT   *****************  //
void configurarInterrupcion(uint32_t MODULO, int PRIORIDAD,
		uint32_t CONDICION);
		//   ----------------    INTERRUPT   -----------------  //


		//   *****************   SYSCLOCK   ******************  //
void configurarReloj(uint32_t FRECUENCIA);
void delay_us(uint32_t microsegundo);
void habilitarDelay();
void delay(int CICLOS);

		//   -----------------   SYSCLOCK   ------------------  //

		//   *******************   I2C   ********************  //
void habilitarI2C_1CH(int MODULO);
void enviarI2C_1CH(int DIRECCION, int DATO);
		//   -------------------   I2C   --------------------  //


		//   *******************   UART   ********************  //
//							 RVISION 2015						//
void configurarUART(uint32_t Modulo, uint32_t Baudaje);
void UARTPrint(uint32_t MODULO, const char *Mensaje);
void UARTPrintf(uint32_t MODULO, uint32_t Mensaje);
void UARTPrintln(uint32_t MODULO, const char *Mensaje);
void UARTPrintfln(uint32_t MODULO, uint32_t Mensaje);
void limpiarUART(uint32_t MODULO);
int leerUART(uint32_t MODULO);
void leerStr(uint32_t MODULO, char *cadena);
		//   -------------------   UART   --------------------  //

		//   *****************   GENERICAS   ******************* //
void int2str(int ENTERO, char * cadena , int digitos);
int str2int(char *cadena, int desfase);
void error();


		//   -----------------   GENERICAS   ------------------  //


/*--------------------------------------------------------------------------------*/

#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"

int main(void)
{
   int t;
   int mindere=0, minizq, i;
   //int dato = 0;
   //float ref;// referencia = posicion [63] del arreglo
   //float y;// salida del sistema (posicion del arreglo i)
   //float ek =0; // suma de todos los errores
   //float Ts =0.1; // periodo de muestreo de la señal
   float e;// e = ref-y
   float e_1 =0; // error del instante de tiempo anterior
   float u;// salida del controlador
   float Kp = 0.8; // ganancia proporcional 0.6
   //float Ki = 0.1; // ganancia integral
   float Kd = 0.2; // ganancia derivativa
   float salida; //salida PD
   float servo;
   uint16_t MAX; //Valor de referencia para buscar el minimo 0 a 4096
   
   TFC_Init();
   
   for(;;)
      
   {
         TFC_Task();

         switch(TFC_GetDIP_Switch())
         {
         default:
         case 0 :
           //TFC_SetLineScanExposureTime(30000); //TFC_Config.h, TFC_ADC, TFC_LineScanCamera.c
           //TFC_ReadBatteryVoltage TFC_ADC.h
           TFC_SetMotorPWM(0,0); //se detienen los motores
           TFC_SetServo(0,0); //se coloca el servo en el centro
           //Espero a que se precione el boton 1 
           if(TFC_PUSH_BUTTON_1_PRESSED)
             //si se preciona dato=1 para llamar case 1
             //dato=1;
           
        ////////////////////////////////////////////////////////////////////////////7
           if(TFC_Ticker[0]>100 && LineScanImageReady==1)
                  {
                    TFC_Ticker[0] = 0;
                    LineScanImageReady=0;
                    TERMINAL_PRINTF("\r\n");
                    TERMINAL_PRINTF("L:");
                    
                    if(t==0)
                      t=3;
                    else
                      t--;
                    
                    TFC_SetBatteryLED_Level(t);
                    
                    for(i=0;i<128;i++)
                    {
                      TERMINAL_PRINTF("%X,",LineScanImage0[i]);
                    }
                            
                    for(i=0;i<128;i++)
                    {
                      TERMINAL_PRINTF("%X",LineScanImage1[i]);
                      if(i==127)
                        TERMINAL_PRINTF("\r\n",LineScanImage1[i]);
                      else
                        TERMINAL_PRINTF(",",LineScanImage1[i]);
                    }
                  }
        
        ////////////////////////////////////////////////////////////////////////////7
            
                 break;
               
         case 1:
               
            TFC_HBRIDGE_ENABLE;
                       
            if(TFC_PUSH_BUTTON_0_PRESSED){
            	//dato=0;
            }
             
             if(TFC_Ticker[0]>100 && LineScanImageReady==1)
             {
            	 TFC_Ticker[0] = 0;
            	 LineScanImageReady=0;
            	 TERMINAL_PRINTF("\r\n");
            	 TERMINAL_PRINTF("L:");
            	 
            	 for(i=0;i<128;i++)
            	 {
            		 TERMINAL_PRINTF("%X,",LineScanImage0[i]);
            	 }
            	 
            	 MAX = 4000;
            	             	 
            	 for(i=64; i>5; i--){
            		 if(LineScanImage0[i]<MAX){
            			 minizq = i;
            			 MAX = LineScanImage0[i]; 
            		 }
            	 }
            	 
            	 for(i=64; i<123; i++){
            		 if(LineScanImage0[i]<MAX){
            			 mindere = i;
            			 MAX = LineScanImage0[i];
            		 }
            	 }
                      	 
            	 
            	 servo = (mindere + minizq)/2;
            	 servo = (servo - 64)/64.0f;  //-1, 1
            	 
                 e = servo;// error actual
                 //ek += e;// acumulo todos los errores para el integral //ek = ek + e
                 u = Kp*e+Kd*(e-e_1);//accion de control
                 e_1 = e;// error para el instante anterior e(t -1)
                 salida = u;// Registro donde se escribe el ciclo de trabajo
                 
                 TFC_SetServo(0,salida);
                 TFC_SetMotorPWM(0.4, 0.4);
             }
             
             break;
         }
   }  
   return 0;
}

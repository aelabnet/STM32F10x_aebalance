/*******************************************************************************
** �ļ���: 		ae_Com.c
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 5.20
** ����: 		  aelab
** ��������: 	2016-12-16
** ����:		  ��aeMotion for aebalanceͨ��
** ����ļ�:	ae_usart.c
** �޸���־��	2016-12-16   �����ĵ�
*******************************************************************************/
/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"
#include "stdlib.h" 
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "ae_usart.h"



/* ���� ----------------------------------------------------------------------*/
bool SendAnglePID = false;          //�Ƕ�PID���ͱ�־λ
bool SendVelocityPID = false;       //�ٶ�PID���ͱ�־λ
bool SendTurnPID = false;           //ת��PID���ͱ�־λ
bool SendComp = false;              //�����˲��������ͱ�־λ
bool SendKalman = false;            //Kalman�˲��������ͱ�־λ
bool SendInfo  = false;             //С����Ϣ���ͱ�־λ
bool SendSetting = false;           //С�����÷��ͱ�־λ
bool SendIMU = false;               //С����̬���ͱ�־λ
bool SendPWM = false;               //С�����PWM���ͱ�־λ

bool FlagStopMotor = false;                 //���ֹͣ��־λ
bool FlagCalibGyro = false;                 //������У׼��־λ
bool FlagCalibMotor = false;                //���У׼��־λ
bool FlagReset = false;                     //�ָ��������ñ�־λ
bool FlagSave = false;                      //�������ݱ�־λ
bool FlagNewSample = false;                 //�µĲ������ڱ�־λ
bool FlagNewPWM = false;                    //�µĵ��PWM��־λ


double CfgADP;                  //�Ƕ�PID��������
double CfgADI;                      //�Ƕ�PID���ֲ���
double CfgADD;                  //�Ƕ�PID΢�ֲ���

double CfgZeroAngle;              //С��Ŀ�����
double CfgControlAngleLimit;       //С��������
double CfgTurningLimit;            //С�����ת���ٶ�

double CfgVDP;                     //�ٶ�PID��������
double CfgVDI;                  //�ٶ�PID���ֲ���
double CfgVDD;                      //�ٶ�PID΢�ֲ���
uint32_t CfgVL;                   //�ٶȻ�������ֵ

double CfgTDP;                      //ת��PID��������
double CfgTDI;                      //ת��PID���ֲ���
double CfgTDD;                      //ת��PID΢�ֲ���

int32_t FreqPWM;                //���PWMƵ��

uint16_t SamplePeriod;             //�������� n*0.1ms     
double Coefficient;            //�����˲�����ϵ��

double Qangle;                          //Kalman����
double Qbias;
double Rmeasure;

double data1[20];
uint32_t data2[5];


double AngleXacc,AngleXgyro,AngleComp,AngleKalman;        //4�ֽǶȼ�����

double SppData1 = 0;                    //С��ת�����              
double SppData2 = 0;                    //С���ٶȲ���

char Firmware[] = "aebalance V1.0";     //Ӳ���汾
double Voltage = -1;                    //��ص�ѹֵ

/*******************************************************************************
  * @��������	  GetValues
  * @����˵��   ��������ַ������н����ж�
  * @�������   *input�������ַ���
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void GetValues(char *input) 
{
	/* ��λ������������� */
	if (input[0] == 'A' && input[1] == ';')          //ֹͣ���
		FlagStopMotor = true;
  else if (input[0] == 'A' && input[1] == 'C')     //������&���ٶȼ�У׼
		FlagCalibGyro = true;	
  else if (input[0] == 'M' && input[1] == 'C')     // ���У׼
    FlagCalibMotor = true;

  /* ��λ��������aebalanceС����������Ϣ */
  else if (input[0] == 'G') {                     
    if (input[1] == 'A')                           //��λ�������ýǶ�PIDֵ
      SendAnglePID = true;
    else if (input[1] == 'V')                      //��λ���������ٶ�PIDֵ
      SendVelocityPID = true;
		else if (input[1] == 'R')                      //��λ��������ת��PIDֵ
      SendTurnPID = true;
    else if (input[1] == 'C')                      //��λ�������û����˲�����ֵ
      SendComp = true;
		else if (input[1] == 'K')                      //��λ��������Kalman�˲�����ֵ
      SendKalman = true;
    else if (input[1] == 'I')                      //��λ��������С����Ϣ
      SendInfo = true;
    else if (input[1] == 'S')                      //��λ��������С�����ò���
      SendSetting = true;
		else if (input[1] == 'W')                      //��λ��������С�����PWM����
      SendPWM = true;
  }  
	
  /* ��λ������aebalanceС������ */
  else if (input[0] == 'S') {                      
    /*��λ������aebalance �Ƕ�PID���� */
    if (input[1] == 'P') {                         //����С���ĽǶ�P����
      strtok(input, ","); 
      CfgADP = atof(strtok(NULL, ";"));
    } else if (input[1] == 'I') {                  //����С���ĽǶ�I����
      strtok(input, ","); 
      CfgADI = atof(strtok(NULL, ";"));
    } else if (input[1] == 'D') {                  //����С���ĽǶ�D����
      strtok(input, ","); 
      CfgADD = atof(strtok(NULL, ";"));
    } 
				
		/*��λ������aebalance Ŀ��Ƕ� */
		else if (input[1] == 'T') { 
      strtok(input, ","); 
      CfgZeroAngle = atof(strtok(NULL, ";"));
    } 

		/*��λ������aebalance �ٶ�PID���� */		
		else if (input[1] == 'V') { 
      strtok(input, ","); 
      CfgVDP = atof(strtok(NULL, ","));            //����С�����ٶ�P����
      CfgVDI = atof(strtok(NULL, ","));            //����С�����ٶ�I����
      CfgVDD = atof(strtok(NULL, ","));            //����С�����ٶ�D����
			CfgVL = atof(strtok(NULL, ";"));             //����С���ٶȻ�������
    } 
		
		/*��λ������aebalance ת��PID���� */		
		else if (input[1] == 'R') { 
      strtok(input, ","); 
      CfgTDP = atof(strtok(NULL, ","));            //����С����ת��P����
      CfgTDI = atof(strtok(NULL, ","));            //����С����ת��I����
      CfgTDD = atof(strtok(NULL, ";"));            //����С����ת��D����
    } 		
		
		/*��λ������aebalance �����˲����� */	
		else if (input[1] == 'C') { 
      strtok(input, ","); 
			SamplePeriod = atof(strtok(NULL, ","));      //����С���Ĳ������ڲ���
      Coefficient = atof(strtok(NULL, ";"));       //����С���Ļ����˲�����
			FlagNewSample = true;  
		} 

		/*��λ������aebalance Kalman�˲����� */	
		else if (input[1] == 'K') { 
      strtok(input, ","); 
			Qangle = atof(strtok(NULL, ","));            //����С����kalman�˲���Qangle����
      Qbias = atof(strtok(NULL, ","));             //����С����kalman�˲���Qbias����
			Rmeasure = atof(strtok(NULL, ","));          //����С����kalman�˲���Rmeasure����
			SamplePeriod = atof(strtok(NULL, ";"));      //����С���Ĳ������ڲ���
			FlagNewSample = true;  
		} 
		
		/*��λ������aebalance ���PWM���� */	
		else if (input[1] == 'W') { 
      strtok(input, ","); 
			FreqPWM = atof(strtok(NULL, ";"));           
			FlagNewPWM = true;

		} 		
		
		/*��λ������aebalance ���ƽǶ����ֵ */	
		else if (input[1] == 'A') { 
      strtok(input, ","); 
      CfgControlAngleLimit = atoi(strtok(NULL, ";"));
    } 
		
		
		/*��λ������aebalance ת�����ֵ */	
		else if (input[1] == 'U') { 
      strtok(input, ","); 
      CfgTurningLimit = atoi(strtok(NULL, ";"));
    }
  }
	
	
  /* ��λ������aebalance�Ƿ�����̬���� */
  else if (input[0] == 'I') { 
    if (input[1] == 'B')                           //������̬ʹ��
      SendIMU = true; 
    else if (input[1] == 'S')                      //������̬�ر�
      SendIMU = false; 
  }
  /* ��λ������ָ�Ĭ������ */
  else if (input[0] == 'F') { 
    if (input[1] == 'R')                           //�ָ�Ĭ������
			FlagReset = true;
  }	
	  /* ��λ�����󱣴����в��� */
  else if (input[0] == 'D') { 
    if (input[1] == 'S')                           //�������в���
			FlagSave = true;
  }	
	/* ��λ������С������ָ�� */	
  else if (input[0] == 'C') { 
    if (input[1] == 'S'){                          //������λ��ͣ��ָ��
      SppData1 = 0;
		  SppData2 = 0;
		}
    else if (input[1] == 'J') {                    //������λ��ҡ��ָ��
      strtok(input, ","); 
      SppData1 = atof(strtok(NULL, ","));          // x-axis
      SppData2 = atof(strtok(NULL, ";"));          // y-axis
    }
    else if (input[1] == 'M') {                    //������λ����бң��ָ��
      strtok(input, ","); 
      SppData1 = atof(strtok(NULL, ","));          // Pitch
      SppData2 = atof(strtok(NULL, ";"));          // Roll
    }
	}
	
	
	
}



/*******************************************************************************
  * @��������	  SendValues()
  * @����˵��   ����λ��������Ϣ
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void SendValues(void) 
{
	
	if (SendAnglePID){                               //����λ�����ͽǶ�PID������Ŀ��Ƕ�
    printf("A,%.1f,%.1f,%.2f,%0.1f;",CfgADP,CfgADI,CfgADD,CfgZeroAngle);	
		SendAnglePID = false;	
	}
	else if(SendVelocityPID){                        //����λ�������ٶ�PID����
		printf("V,%.1f,%.3f,%.1f,%d;",CfgVDP,CfgVDI,CfgVDD,CfgVL);	
		SendVelocityPID = false;
	}
	else if(SendTurnPID){                            //����λ�������ٶ�PID����
		printf("R,%.4f,%.4f,%.4f;",CfgTDP,CfgTDI,CfgTDD);	
		SendTurnPID = false;
	}
	else if(SendComp){                               //����λ���������ڣ������˲�����
		printf("C,%d,%.4f;",SamplePeriod,Coefficient);	
		SendComp = false;
	}
		else if(SendKalman){                           //����λ���������ڣ�Kalman�˲�����
		printf("K,%.5f,%.5f,%.5f,%d;",Qangle,Qbias,Rmeasure,SamplePeriod);	
		SendKalman = false;
	}
	else if(SendInfo){                               //����λ������С����Ϣ,��ѹ
		printf("I,%s,%.2f;",Firmware,Voltage);	
		SendInfo = false;
	}
	else if(SendSetting){                            //����λ������С�����ò���
		printf("S,%.1f,%.1f;",CfgControlAngleLimit,CfgTurningLimit);
		SendSetting = false;
	}	
	else if(SendPWM){                                //����λ������С�����PWM
		printf("W,%d;",FreqPWM);
		SendPWM = false;
	}			

	if(SendIMU)                                       //����λ������С����̬
	{
		printf("P,%.1f,%.1f,%.1f,%.1f;",AngleXacc,AngleXgyro,AngleComp,AngleKalman);

	}
	
	
}

/*******************************************************************************
  * @��������	  ListValues
  * @����˵��   ���ݶ��л�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void ListValues(void) 
{
	data1[0] = CfgADP;
	data1[1] = CfgADI;
	data1[2] = CfgADD;
	
	data1[3] = CfgZeroAngle;
	data1[4] = CfgControlAngleLimit;
	data1[5] = CfgTurningLimit;	
	
	data1[6] = CfgVDP;
	data1[7] = CfgVDI;
	data1[8] = CfgVDD;	
	data2[0] = CfgVL;

	data1[9] = CfgTDP;
	data1[10] = CfgTDI;
	data1[11] = CfgTDD;	

	data2[1] = FreqPWM;

	data2[2] = SamplePeriod;

	data1[12] = Coefficient;	
	
	data1[13] = Qangle;	
  data1[14] = Qbias;	
	data1[15] = Rmeasure;	

}

 
/*******************************************************************************
  * @��������	  DeListValues
  * @����˵��   ����ȥ���л�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void DeListValues(void) 
{
	CfgADP = data1[0];
	CfgADI = data1[1];
	CfgADD = data1[2];
	
	CfgZeroAngle = data1[3];
	CfgControlAngleLimit = data1[4];
	CfgTurningLimit = data1[5];	
	
	CfgVDP = data1[6];
	CfgVDI = data1[7];
	CfgVDD = data1[8];	
	CfgVL = data2[0];

	CfgTDP = data1[9];
	CfgTDI = data1[10];
	CfgTDD = data1[11];	

	FreqPWM = data2[1];

	SamplePeriod = data2[2];

	Coefficient = data1[12];	
	
	Qangle = data1[13];	
  Qbias = data1[14];	
	Rmeasure = data1[15];	

}

/*******************************************************************************
  * @��������	  DefaultValues
  * @����˵��   �ָ�Ĭ��ֵ
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void DefaultValues(void) 
{
	CfgADP = -10;                  //�Ƕ�PID��������
	CfgADI = 0;                      //�Ƕ�PID���ֲ���
	CfgADD = -0.12;                  //�Ƕ�PID΢�ֲ���

	CfgZeroAngle = -15;              //С��Ŀ�����
	CfgControlAngleLimit = 15;       //С��������
	CfgTurningLimit = 40;            //С�����ת���ٶ�

	CfgVDP = -1;//-3;                     //�ٶ�PID��������
	CfgVDI = -0.09;//-0.25;                  //�ٶ�PID���ֲ���
	CfgVDD = 0;                      //�ٶ�PID΢�ֲ���
	CfgVL = 300;                   //�ٶȻ�������ֵ

	CfgTDP = 0;                      //ת��PID��������
	CfgTDI = 0;                      //ת��PID���ֲ���
	CfgTDD = 0;                      //ת��PID΢�ֲ���

	FreqPWM = 20000;                //���PWMƵ��

	SamplePeriod = 30;             //�������� n*0.1ms     
	Coefficient = 0.9984;            //�����˲�����ϵ��

	Qangle = 0;                          //Kalman����
	Qbias = 0;
	Rmeasure = 0;
	
	ListValues(); 


}

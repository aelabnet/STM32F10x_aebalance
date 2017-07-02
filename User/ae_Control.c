/* ����ͷ�ļ� *****************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"

#include "SysTickDelay.h"
#include "ae_VNH5160.h"
#include "ae_Com.h"
#include "ae_Angle.h"
#include "ae_Encode.h"


/* �������� ------------------------------------------------------------------*/


/* �궨�� --------------------------------------------------------------------*/

/* ���� ----------------------------------------------------------------------*/




double IAngle=0;                        //�ǶȻ�����
double PIDVelocity=0;                   //�ٶ�PIDֵ
double CarSpeed,LastSpeed;              //С���ٶȡ���һ��С���ٶ�
int32_t ISpeed=0;                        //�ٶȻ�����

double PIDTurn=0;                      //ת��PIDֵ
double LastDiffSpeed;                  //��һ�������ٶȲ�
double CarDiffSpeed;                   //С�������ٶȲ�
double ITurn=0;                        //ת�������

double CurrentSppData2 = 0;                    //С����ǰ�ٶ��������

uint16_t LayingDown;                    //С���Ƿ���
uint8_t SpeedControlCnt = 0;            //�ٶȻ����Ƽ���



/* �������� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/

/*******************************************************************************
  * @��������	  Constrain
  * @����˵��   ��ֵ��������
  * @�������   double input���������
								double min���������Сֵ
								double max�����е����ֵ
  * @�������   ��
  * @���ز���   ���ƺ��ֵ
  *****************************************************************************/
double Constrain(double input, double min, double max)
{
	if(input < min)
		return min;
	if(input > max)
		return max;
	return input;
}
		
/*******************************************************************************
  * @��������	  AnglePID
  * @����˵��   PID�㷨
  * @�������   targetAngle��Ŀ��Ƕ�
								currentPitch����ǰ�Ƕ�
  * @�������   ��
  * @���ز���   �Ƕ�pid���
  *****************************************************************************/
double AnglePID(double targetAngle, double currentPitch) 
{
	double err;
	double pValue,iValue,dValue,pidValue;
	
	/* ����PIDֵ */
	err =  currentPitch - targetAngle;    //��������
	pValue = CfgADP * err;
	
	IAngle += err;                        //���ּ���
	iValue = CfgADI * IAngle;
	
	dValue = CfgADD * XgyroRate;          //΢�ּ���

	pidValue = pValue + iValue + dValue;	//�Ƕ�PIDֵ
	return pidValue;		
}

/*******************************************************************************
  * @��������	  SpeedPID
  * @����˵��   PID�㷨
  * @�������   targetSpeed��Ŀ���ٶ�
  * @�������   ��
  * @���ز���   �ٶ�pid���
  *****************************************************************************/
double SpeedPID(double targetSpeed) 
{
	double err;
	double pValue,iValue,dValue,pidValue;
	int32_t plusLimit,minusLimit;
	
	plusLimit = CfgVL;
	minusLimit = -CfgVL;
	
	CarSpeed *= 0.8;                               //С���ٶȵ�ͨ�˲�
	CarSpeed += (LeftDiff + RightDiff)/2*0.2;
		
	/* ����PIDֵ */
	err = CarSpeed - targetSpeed;                  //��������
	pValue = CfgVDP * err;
	
	ISpeed += err;                                 //���ּ���
	if(ISpeed > plusLimit ) ISpeed = plusLimit ;             //��������
	if(ISpeed < minusLimit) ISpeed = minusLimit;
	iValue = CfgVDI * ISpeed;
	
	
	dValue = CfgVDD * (CarSpeed - LastSpeed);      //΢�ּ���
	LastSpeed = CarSpeed;
	
	pidValue = pValue + iValue + dValue;	         //�ٶ�PIDֵ
	return pidValue;			
}

/*******************************************************************************
  * @��������	  SpeedP
  * @����˵��   P�㷨
  * @�������   targetSpeed��Ŀ���ٶ�
  * @�������   ��
  * @���ز���   �ٶ�p���
  *****************************************************************************/
double SpeedP(double targetSpeed) 
{
	double err;
	double pValue;
	
	CarSpeed *= 0.8;                               //С���ٶȵ�ͨ�˲�
	CarSpeed += (LeftDiff + RightDiff)/2*0.2;
		
	/* ����Pֵ */
	err = CarSpeed - targetSpeed;                  //��������
	pValue = CfgVDP * err;

	LastSpeed = CarSpeed;
	ISpeed = 0;

	return pValue;			
}

/*******************************************************************************
  * @��������	  TurnPID
  * @����˵��   PID�㷨
  * @�������   targetTurn��Ŀ��ת��
  * @�������   ��
  * @���ز���   ת��pid���
  *****************************************************************************/
double TurnPID(double targetTurn) 
{
	double err, diffSpeed;
	double pValue,iValue,dValue,pidValue;
	
	diffSpeed = LeftDiff - RightDiff;
	
	CarDiffSpeed *= 0.8;                               //С���ٶȵ�ͨ�˲�
	CarDiffSpeed += diffSpeed*0.2;
	
		
	/* ����PIDֵ */
	err = CarDiffSpeed - targetTurn;                  //��������
	pValue = CfgTDP * err;
	
	ITurn += err;                                 //���ּ���
	if(ITurn > 20000) ITurn = 20000;              //��������
	if(ITurn < -20000) ITurn = -20000;
	iValue = CfgTDI * ITurn;
	
	dValue = CfgTDD * (CarDiffSpeed - LastDiffSpeed);      //΢�ּ���
	LastDiffSpeed = CarDiffSpeed;
	
	pidValue = pValue + iValue + dValue;	         //�ٶ�PIDֵ
	return pidValue;			
}

/*******************************************************************************
  * @��������	  UpdatePWM
  * @����˵��   ���µ��PWM������С��
  * @�������   valuePID��pid
								LPID����PID��ֵ
								RPID����PID��ֵ
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void UpdatePWM(double valuePID,double LPID,double RPID) 
{
	double leftPID,rightPID;
	leftPID = valuePID - LPID;                     //����PWMֵ
	rightPID = valuePID - RPID;                    //����PWMֵ
  if (leftPID >= 0)
    VNH5160_moveMotor(left, backward, leftPID);  //����VNH5160
  else
    VNH5160_moveMotor(left, forward, -leftPID);  //����VNH5160
  if (rightPID >= 0)
    VNH5160_moveMotor(right, backward, rightPID);//����VNH5160
  else
    VNH5160_moveMotor(right, forward, -rightPID);//����VNH5160
}

/*******************************************************************************
  * @��������	  CalibMotor
  * @����˵��   У׼������Ʋ���
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void CalibMotor(void)
{
	uint16_t i;
	for(i=0;i<51;i++)
	{
		VNH5160_moveMotor(left, backward, 2*i);
		VNH5160_moveMotor(right, backward, 2*i);
		printf("%d   %d    %d\n",i,LeftDiff,RightDiff);	
		delay_ms(20000);
		printf("%d   %d    %d\n",i,LeftDiff,RightDiff);	
		delay_ms(2000);
		printf("%d   %d    %d\n",i,LeftDiff,RightDiff);	
		delay_ms(2000);
		printf("%d   %d    %d\n",i,LeftDiff,RightDiff);	
		delay_ms(2000);
		printf("%d   %d    %d\n",i,LeftDiff,RightDiff);	
		delay_ms(2000);		
	}
	
}



/*******************************************************************************
  * @��������	  PWMScale
  * @����˵��   ���PWM�����ֶ�У׼
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
double PWMScale(enum Command motor,double PWMvalue){
	
// 	if(PWMvalue<=6.25){
// 		if(motor == left){
// 			return(4.15/6.25*PWMvalue);		
// 		}else{
// 			return(2.25/6.25*PWMvalue);		
// 		}
// 	}else 
	if(PWMvalue<=25){
		if(motor == left){
			return((22.15-4.15)/18.75*(PWMvalue)+4.15);		
		}else{
			return((16.7-2.25)/18.75*(PWMvalue)+2.25);		
		}		
	}else if(PWMvalue<=56.25){
		if(motor == left){
			return((52.3-22.15)/31.5*(PWMvalue-25)+22.15);		
		}else{
			return((48.2-16.7)/31.5*(PWMvalue-25)+16.7);		
		}		
	}else if(PWMvalue<=62.5){
		if(motor == left){
			return((56.6-52.3)/6.25*(PWMvalue-56.25)+52.3);		
		}else{
			return((51.5-48.2)/6.25*(PWMvalue-56.25)+48.2);		
		}		
	}else if(PWMvalue<=68.75){
		if(motor == left){
			return((58.9-56.6)/6.25*(PWMvalue-62.5)+56.6);		
		}else{
			return((56.2-51.5)/6.25*(PWMvalue-62.5)+51.5);		
		}		
	}else if(PWMvalue<=87.5){
		if(motor == left){
			return((75-58.9)/18.75*(PWMvalue-68.75)+58.9);		
		}else{
			return((75.5-56.2)/18.75*(PWMvalue-68.75)+56.2);		
		}		
	}else if(PWMvalue<=93.75){
		if(motor == left){
			return((82-75)/6.25*(PWMvalue-87.5)+75);		
		}else{
			return((83.4-75.5)/6.25*(PWMvalue-87.5)+75.5);		
		}		
	}else{
		if(motor == left){
			return((88-82)/6.25*(PWMvalue-93.75)+82);		
		}else{
			return((96.8-83.4)/6.25*(PWMvalue-93.75)+83.4);		
		}		
	}
}

/*******************************************************************************
  * @��������	  Control
  * @����˵��   С������
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
  *****************************************************************************/
void Control(void)
{
    double pitch,targetAngle,anglePIDValue,PIDValue;
	
		pitch = AngleComp;                      //���û����˲���õĽǶ���Ϊ�������	
		
		if ((LayingDown && (pitch < CfgZeroAngle - 20 || pitch > CfgZeroAngle + 20)) || (!LayingDown && (pitch < CfgZeroAngle - 40 || pitch > CfgZeroAngle + 40))) 
		{
			LayingDown = 1;                       // С����ǳ���һ���Ƕ�ֹͣ����
			VNH5160_stopMotor(left);
			VNH5160_stopMotor(right);
		}else{
			LayingDown = 0;                       //С��վ����ָ�����
	


			if(SpeedControlCnt == 0)
			{
			  if(SppData2 == 0)                   //С��ֹͣʱ��Ҫ�ٶ�PI��
				{
					CurrentSppData2 = Constrain(SppData2, CurrentSppData2-0.03, CurrentSppData2+0.03);  //���Ʊ仯�Ĳ���
					PIDVelocity = SpeedPID(CurrentSppData2*75); 	
				}
				else                                //С�����˶�ʱֻ��P������
				{
					CurrentSppData2 = Constrain(SppData2, CurrentSppData2-0.03, CurrentSppData2+0.03);  //���Ʊ仯�Ĳ���
					PIDVelocity = SpeedP(CurrentSppData2*75); 	
				}
			}
	
			targetAngle = CfgZeroAngle;
			
			anglePIDValue = AnglePID(targetAngle, pitch);                    //����Ƕ�PIDֵ
			PIDTurn = TurnPID(SppData1*100);                                            //����ת��PIDֵ                    
			
			PIDValue = anglePIDValue + PIDVelocity;//*(SpeedControlCnt+1)/10;   //�ֶμ����ٶ�PIDֵ			
			UpdatePWM(PIDValue,PIDTurn ,-PIDTurn);                    //����С��																													                                  

		} 
}




 

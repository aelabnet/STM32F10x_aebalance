/* 包含头文件 *****************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "math.h"
#include "stdlib.h"

#include "SysTickDelay.h"
#include "ae_VNH5160.h"
#include "ae_Com.h"
#include "ae_Angle.h"
#include "ae_Encode.h"


/* 类型声明 ------------------------------------------------------------------*/


/* 宏定义 --------------------------------------------------------------------*/

/* 变量 ----------------------------------------------------------------------*/




double IAngle=0;                        //角度积分项
double PIDVelocity=0;                   //速度PID值
double CarSpeed,LastSpeed;              //小车速度、上一次小车速度
int32_t ISpeed=0;                        //速度积分项

double PIDTurn=0;                      //转向PID值
double LastDiffSpeed;                  //上一次两轮速度差
double CarDiffSpeed;                   //小车两轮速度差
double ITurn=0;                        //转向积分项

double CurrentSppData2 = 0;                    //小车当前速度需求参数

uint16_t LayingDown;                    //小车是否倒下
uint8_t SpeedControlCnt = 0;            //速度环控制计数



/* 函数声明 ------------------------------------------------------------------*/
/* 函数功能 ------------------------------------------------------------------*/

/*******************************************************************************
  * @函数名称	  Constrain
  * @函数说明   对值进行限制
  * @输入参数   double input：输入参数
								double min：允许的最小值
								double max：运行的最大值
  * @输出参数   无
  * @返回参数   限制后的值
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
  * @函数名称	  AnglePID
  * @函数说明   PID算法
  * @输入参数   targetAngle：目标角度
								currentPitch：当前角度
  * @输出参数   无
  * @返回参数   角度pid结果
  *****************************************************************************/
double AnglePID(double targetAngle, double currentPitch) 
{
	double err;
	double pValue,iValue,dValue,pidValue;
	
	/* 更新PID值 */
	err =  currentPitch - targetAngle;    //比例计算
	pValue = CfgADP * err;
	
	IAngle += err;                        //积分计算
	iValue = CfgADI * IAngle;
	
	dValue = CfgADD * XgyroRate;          //微分计算

	pidValue = pValue + iValue + dValue;	//角度PID值
	return pidValue;		
}

/*******************************************************************************
  * @函数名称	  SpeedPID
  * @函数说明   PID算法
  * @输入参数   targetSpeed：目标速度
  * @输出参数   无
  * @返回参数   速度pid结果
  *****************************************************************************/
double SpeedPID(double targetSpeed) 
{
	double err;
	double pValue,iValue,dValue,pidValue;
	int32_t plusLimit,minusLimit;
	
	plusLimit = CfgVL;
	minusLimit = -CfgVL;
	
	CarSpeed *= 0.8;                               //小车速度低通滤波
	CarSpeed += (LeftDiff + RightDiff)/2*0.2;
		
	/* 更新PID值 */
	err = CarSpeed - targetSpeed;                  //比例计算
	pValue = CfgVDP * err;
	
	ISpeed += err;                                 //积分计算
	if(ISpeed > plusLimit ) ISpeed = plusLimit ;             //积分限制
	if(ISpeed < minusLimit) ISpeed = minusLimit;
	iValue = CfgVDI * ISpeed;
	
	
	dValue = CfgVDD * (CarSpeed - LastSpeed);      //微分计算
	LastSpeed = CarSpeed;
	
	pidValue = pValue + iValue + dValue;	         //速度PID值
	return pidValue;			
}

/*******************************************************************************
  * @函数名称	  SpeedP
  * @函数说明   P算法
  * @输入参数   targetSpeed：目标速度
  * @输出参数   无
  * @返回参数   速度p结果
  *****************************************************************************/
double SpeedP(double targetSpeed) 
{
	double err;
	double pValue;
	
	CarSpeed *= 0.8;                               //小车速度低通滤波
	CarSpeed += (LeftDiff + RightDiff)/2*0.2;
		
	/* 更新P值 */
	err = CarSpeed - targetSpeed;                  //比例计算
	pValue = CfgVDP * err;

	LastSpeed = CarSpeed;
	ISpeed = 0;

	return pValue;			
}

/*******************************************************************************
  * @函数名称	  TurnPID
  * @函数说明   PID算法
  * @输入参数   targetTurn：目标转向
  * @输出参数   无
  * @返回参数   转向pid结果
  *****************************************************************************/
double TurnPID(double targetTurn) 
{
	double err, diffSpeed;
	double pValue,iValue,dValue,pidValue;
	
	diffSpeed = LeftDiff - RightDiff;
	
	CarDiffSpeed *= 0.8;                               //小车速度低通滤波
	CarDiffSpeed += diffSpeed*0.2;
	
		
	/* 更新PID值 */
	err = CarDiffSpeed - targetTurn;                  //比例计算
	pValue = CfgTDP * err;
	
	ITurn += err;                                 //积分计算
	if(ITurn > 20000) ITurn = 20000;              //积分限制
	if(ITurn < -20000) ITurn = -20000;
	iValue = CfgTDI * ITurn;
	
	dValue = CfgTDD * (CarDiffSpeed - LastDiffSpeed);      //微分计算
	LastDiffSpeed = CarDiffSpeed;
	
	pidValue = pValue + iValue + dValue;	         //速度PID值
	return pidValue;			
}

/*******************************************************************************
  * @函数名称	  UpdatePWM
  * @函数说明   更新电机PWM，控制小车
  * @输入参数   valuePID：pid
								LPID：左PID差值
								RPID：右PID差值
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void UpdatePWM(double valuePID,double LPID,double RPID) 
{
	double leftPID,rightPID;
	leftPID = valuePID - LPID;                     //左轮PWM值
	rightPID = valuePID - RPID;                    //右轮PWM值
  if (leftPID >= 0)
    VNH5160_moveMotor(left, backward, leftPID);  //驱动VNH5160
  else
    VNH5160_moveMotor(left, forward, -leftPID);  //驱动VNH5160
  if (rightPID >= 0)
    VNH5160_moveMotor(right, backward, rightPID);//驱动VNH5160
  else
    VNH5160_moveMotor(right, forward, -rightPID);//驱动VNH5160
}

/*******************************************************************************
  * @函数名称	  CalibMotor
  * @函数说明   校准电机控制参数
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
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
  * @函数名称	  PWMScale
  * @函数说明   电机PWM参数分段校准
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
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
  * @函数名称	  Control
  * @函数说明   小车控制
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
  *****************************************************************************/
void Control(void)
{
    double pitch,targetAngle,anglePIDValue,PIDValue;
	
		pitch = AngleComp;                      //采用互补滤波获得的角度作为最终倾角	
		
		if ((LayingDown && (pitch < CfgZeroAngle - 20 || pitch > CfgZeroAngle + 20)) || (!LayingDown && (pitch < CfgZeroAngle - 40 || pitch > CfgZeroAngle + 40))) 
		{
			LayingDown = 1;                       // 小车倾角超过一定角度停止工作
			VNH5160_stopMotor(left);
			VNH5160_stopMotor(right);
		}else{
			LayingDown = 0;                       //小车站立后恢复工作
	


			if(SpeedControlCnt == 0)
			{
			  if(SppData2 == 0)                   //小车停止时需要速度PI环
				{
					CurrentSppData2 = Constrain(SppData2, CurrentSppData2-0.03, CurrentSppData2+0.03);  //限制变化的步进
					PIDVelocity = SpeedPID(CurrentSppData2*75); 	
				}
				else                                //小车在运动时只需P环控制
				{
					CurrentSppData2 = Constrain(SppData2, CurrentSppData2-0.03, CurrentSppData2+0.03);  //限制变化的步进
					PIDVelocity = SpeedP(CurrentSppData2*75); 	
				}
			}
	
			targetAngle = CfgZeroAngle;
			
			anglePIDValue = AnglePID(targetAngle, pitch);                    //计算角度PID值
			PIDTurn = TurnPID(SppData1*100);                                            //计算转向PID值                    
			
			PIDValue = anglePIDValue + PIDVelocity;//*(SpeedControlCnt+1)/10;   //分段加入速度PID值			
			UpdatePWM(PIDValue,PIDTurn ,-PIDTurn);                    //驱动小车																													                                  

		} 
}




 

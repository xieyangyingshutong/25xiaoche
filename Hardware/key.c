//#include "key.h"


/**************************************************************************
Function: Key scan
Input   : Double click the waiting time
Output  : 0：No action；1：click；2：Double click
函数功能：按键扫描
入口参数：双击等待时间
返回  值：按键状态 0：无动作 1：单击 2：双击
**************************************************************************/
//u8 click_N_Double (u8 time)
//{
//    static  u8 flag_key,count_key,double_key=0;
//    static  u16 count_single,Forever_count;
//    if(DL_GPIO_readPins(KEY_PORT,KEY_key_PIN)>0)  Forever_count++;   //长按标志位未置1
//    else        Forever_count=0;
//    if((DL_GPIO_readPins(KEY_PORT,KEY_key_PIN)>0)&&0==flag_key)     flag_key=1; //第一次按下
//    if(0==count_key)
//    {
//            if(flag_key==1)
//            {
//                double_key++;
//                count_key=1;            //标记按下一次
//            }
//            if(double_key==3)
//            {                                       //按下两次
//                double_key=0;
//                count_single=0;
//                return 2;                   //双击执行的指令
//            }
//    }
//    if(0==DL_GPIO_readPins(KEY_PORT,KEY_key_PIN))          flag_key=0,count_key=0;
//    if(1==double_key)
//    {
//        count_single++;
//        if(count_single>time&&Forever_count<time)
//        {
//            double_key=0;
//            count_single=0; //超时不标记为双击
//			return 1;//单击执行的指令
//        }
//        if(Forever_count>time)
//        {
//            double_key=0;
//            count_single=0;
//        }
//    }
//    return 0;
//}
///**************************************************************************
//Function: Long press detection
//Input   : none
//Output  : 0：No action；1：Long press for 2 seconds；
//函数功能：长按检测
//入口参数：无
//返回  值：按键状态 0：无动作 1：长按2s
//**************************************************************************/
//u8 Long_Press(void)
//{
//        static u16 Long_Press_count,Long_Press;
//      if(Long_Press==0&&KEY==0)  Long_Press_count++;   //长按标志位未置1
//    else                       Long_Press_count=0;
//        if(Long_Press_count>200)        //10ms扫描一次
//      {
//            Long_Press=1;
//            Long_Press_count=0;
//            return 1;
//        }
//        if(Long_Press==1)     //长按标志位置1
//        {
//            Long_Press=0;
//        }
//        return 0;
//}


//void Key(void)
//{
//	u8 tmp,tmp2;
//	tmp=click_N_Double(50);
//	if(tmp==1)
//	{
//		Flag_Stop=!Flag_Stop;
//	}		//单击控制小车的启停
//}

// key_logic.c
#include "key.h"
#include "oled.h"
#include "ti_msp_dl_config.h"

int targetCount = 1;      // 用户设置的目标圈数
int count1 = 0;           // 每圈 L2 触发4次
int count2 = 0;           // 已完成的圈数
int carRunning = 0;       // 1运行，0停止

 
uint32_t lastCount1Time = 0; // 上次 count1 增加时间
int L2_last = 0;


// ---------------------- 按键检测函数 ----------------------
int Get_Key_Next(void) {
    if (!DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_KEY1_PIN)) {
        delay_ms(10);
        if (!DL_GPIO_readPins(KEY_KEY1_PORT, KEY_KEY1_KEY1_PIN)) return 1;
    }
    return 0;
}
int Get_Key_Confirm(void) {
    if (!DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_KEY2_PIN)) {
        delay_ms(10);
        if (!DL_GPIO_readPins(KEY_KEY2_PORT, KEY_KEY2_KEY2_PIN)) return 1;
    }
    return 0;
}


// ---------------------- 按键处理逻辑 ----------------------
void Handle_Keys(void) {
    if (!carRunning) {
        if (Get_Key_Next()) {
            targetCount++;
            if (targetCount > 5) targetCount = 1;
            OLED_UpdateTarget();
            delay_ms(300);
        }
        if (Get_Key_Confirm()) {
            carRunning = 1;
            count1 = 0;
            count2 = 0;
            lastCount1Time = Systick_getTick();
            OLED_ShowString(3, 1, "follow...", 1);
            OLED_UpdateProgress();
            delay_ms(300);
        }
    }
}

// ---------------------- L2检测 + 圈数判断 ----------------------
void Check_L2_Update(void) {
    int L2_current = DL_GPIO_readPins(SENSOR_L2_PORT, SENSOR_L2_PIN) ? 1 : 0;

    if (carRunning && L2_current == 1 && L2_last == 0) {
        count1++;
        lastCount1Time = Systick_getTick();
    }
    L2_last = L2_current;

    if ((Systick_getTick() - lastCount1Time > 3000) && count1 > 0) {
        if (count1 >= 4) {
            count1 = 0;
            count2++;
            OLED_UpdateProgress();
        } else {
            count1 = 0;
        }
    }

    if (count2 >= targetCount) {
        carRunning = 0;
        Set_PWM(0,0);
        OLED_ShowString(3, 1, "finish stop", 1);
    }
}

void OLED_UpdateTarget(void) {
    OLED_ShowString(1, 1, "Target:", 1);             // 行1，显示目标圈数
    OLED_ShowNum(1, 9, targetCount, 1, 1);           // 显示数字（列靠右）
}

void OLED_UpdateProgress(void) {
    OLED_ShowString(2, 1, "Done:", 1);               // 行2，已完成圈数
    OLED_ShowNum(2, 7, count2, 1, 1);
}

void OLED_UpdateStatus(void) {
    if (carRunning) {
        OLED_ShowString(3, 1, "Status: Running ", 1); //行3，显示状态
    } else {
        OLED_ShowString(3, 1, "Status: Stopped ", 1);
    }
}

void OLED_UpdateDebug(void) {
    OLED_ShowString(4, 1, "C1:", 1);                 // 显示 count1
    OLED_ShowNum(4, 4, count1, 2, 1);
    OLED_ShowString(4, 7, "C2:", 1);                 // 显示 count2
    OLED_ShowNum(4, 10, count2, 1, 1);
}



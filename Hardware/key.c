//#include "key.h"


/**************************************************************************
Function: Key scan
Input   : Double click the waiting time
Output  : 0��No action��1��click��2��Double click
�������ܣ�����ɨ��
��ڲ�����˫���ȴ�ʱ��
����  ֵ������״̬ 0���޶��� 1������ 2��˫��
**************************************************************************/
//u8 click_N_Double (u8 time)
//{
//    static  u8 flag_key,count_key,double_key=0;
//    static  u16 count_single,Forever_count;
//    if(DL_GPIO_readPins(KEY_PORT,KEY_key_PIN)>0)  Forever_count++;   //������־λδ��1
//    else        Forever_count=0;
//    if((DL_GPIO_readPins(KEY_PORT,KEY_key_PIN)>0)&&0==flag_key)     flag_key=1; //��һ�ΰ���
//    if(0==count_key)
//    {
//            if(flag_key==1)
//            {
//                double_key++;
//                count_key=1;            //��ǰ���һ��
//            }
//            if(double_key==3)
//            {                                       //��������
//                double_key=0;
//                count_single=0;
//                return 2;                   //˫��ִ�е�ָ��
//            }
//    }
//    if(0==DL_GPIO_readPins(KEY_PORT,KEY_key_PIN))          flag_key=0,count_key=0;
//    if(1==double_key)
//    {
//        count_single++;
//        if(count_single>time&&Forever_count<time)
//        {
//            double_key=0;
//            count_single=0; //��ʱ�����Ϊ˫��
//			return 1;//����ִ�е�ָ��
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
//Output  : 0��No action��1��Long press for 2 seconds��
//�������ܣ��������
//��ڲ�������
//����  ֵ������״̬ 0���޶��� 1������2s
//**************************************************************************/
//u8 Long_Press(void)
//{
//        static u16 Long_Press_count,Long_Press;
//      if(Long_Press==0&&KEY==0)  Long_Press_count++;   //������־λδ��1
//    else                       Long_Press_count=0;
//        if(Long_Press_count>200)        //10msɨ��һ��
//      {
//            Long_Press=1;
//            Long_Press_count=0;
//            return 1;
//        }
//        if(Long_Press==1)     //������־λ��1
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
//	}		//��������С������ͣ
//}

// key_logic.c
#include "key.h"
#include "oled.h"
#include "ti_msp_dl_config.h"

int targetCount = 1;      // �û����õ�Ŀ��Ȧ��
int count1 = 0;           // ÿȦ L2 ����4��
int count2 = 0;           // ����ɵ�Ȧ��
int carRunning = 0;       // 1���У�0ֹͣ

 
uint32_t lastCount1Time = 0; // �ϴ� count1 ����ʱ��
int L2_last = 0;


// ---------------------- ������⺯�� ----------------------
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


// ---------------------- ���������߼� ----------------------
void Handle_Keys(void) {
    if (!carRunning) {
        if (Get_Key_Next()) {
            targetCount++;
            if (targetCount > MAX_TARGET_LAPS) {
                targetCount = MIN_TARGET_LAPS;
            }
            runFinished = 0U;
            runLineLost = 0U;
            OLED_UpdateTarget();
            OLED_UpdateProgress();
        }
        if (Get_Key_Confirm()) {
            carRunning = 1;
            count1 = 0;
            count2 = 0;
            lastCount1Time = Systick_getTick();
            OLED_ShowString(3, 1, "follow...", 1);
            OLED_UpdateProgress();
            OLED_UpdateStatus();
        }
    }
}

// ---------------------- L2��� + Ȧ���ж� ----------------------
void Check_L2_Update(void) {
    int L2_current = DL_GPIO_readPins(SENSOR_L2_PORT, SENSOR_L2_PIN) ? 1 : 0;

    if (l2Current == 0U) {
        if (l2Previous != 0U) {
            l2LowSince = tick_ms;
        }
        if ((uint32_t)(tick_ms - l2LowSince) >= L2_REARM_LOW_MS) {
            cornerArmed = 1U;
        }
    } else if ((l2Previous == 0U) && (cornerArmed != 0U) &&
               ((cornerSeen == 0U) ||
                ((uint32_t)(tick_ms - lastCornerTime) >= MIN_CORNER_INTERVAL_MS))) {
        lastCornerTime = tick_ms;
        cornerSeen = 1U;
        cornerArmed = 0U;
        count1++;
        lastCount1Time = Systick_getTick();
    }
    L2_last = L2_current;

    if ((Systick_getTick() - lastCount1Time > 3000) && count1 > 0) {
        if (count1 >= 4) {
            count1 = 0;
            count2++;
            OLED_UpdateProgress();

            if (count2 >= targetCount) {
                carRunning = 0U;
                runFinished = 1U;
                Set_PWM(0, 0);
                OLED_UpdateStatus();
            }
        }
    }
    l2Previous = l2Current;
}

void Stop_Car_OnLineLost(void)
{
    carRunning = 0U;
    runFinished = 0U;
    runLineLost = 1U;
    count1 = 0U;
    Set_PWM(0, 0);
    OLED_UpdateStatus();
}

void OLED_UpdateTarget(void) {
    OLED_ShowString(1, 1, "Target:", 1);             // ��1����ʾĿ��Ȧ��
    OLED_ShowNum(1, 9, targetCount, 1, 1);           // ��ʾ���֣��п��ң�
}

void OLED_UpdateProgress(void) {
    OLED_ShowString(2, 1, "Done:", 1);               // ��2�������Ȧ��
    OLED_ShowNum(2, 7, count2, 1, 1);
    OLED_ShowString(2, 8, "/", 1);
    OLED_ShowNum(2, 9, targetCount, 1, 1);
}

void OLED_UpdateStatus(void) {
    if (carRunning) {
        OLED_ShowString(3, 1, "Status: Running ", 1); //��3����ʾ״̬
    } else {
        OLED_ShowString(3, 1, "Status: Set 1-5      ", 1);
    }
}

void OLED_UpdateDebug(void) {
    OLED_ShowString(4, 1, "C1:", 1);                 // ��ʾ count1
    OLED_ShowNum(4, 4, count1, 2, 1);
    OLED_ShowString(4, 7, "C2:", 1);                 // ��ʾ count2
    OLED_ShowNum(4, 10, count2, 1, 1);
}

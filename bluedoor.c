/*********************************************************************************************
�������� ���� ���������������Ƴ���
��д�ˣ� ���� �۹��ʡ�
��дʱ�䣺����2015��11��30��
Ӳ��֧�֣�����STC12C4052AD���ⲿ12MHZ����
�ӿ�˵������������ļ��������������������岼��ͼ��  
�޸���־��
	NO.2-���뿪���Զ���λ����Ҫ���ӿ���OPEN������ʱ�ӵأ�����
����NO.3-�������ܣ���һֱ��ʱ�����Ḵλ��ֻ�д��Źص��ſ�ʱ�Żᣬ�������ſ�ʱHoldס����������
	NO.4-���ӿ���������֤��ϵͳ����ȫ
	NO.5-���������������������������������ܣ�ϵͳ����ȫ
	NO.6-���Ӷ�LEDָʾ�ƵĿ��ؼ���˸������������
	NO.7-�Ͽ�����ʱ�������ģʽ,�����PWM,���͹���(��Ҫ����ģ���״̬���)�����ɴ����жϻ���
	NO.8-�豸���Ӳ���ʱ�����ݽ���ʱ�Զ��Ͽ���������ֹһ�����Ӻ����Ͽ������˲��������
		�����һ9012�����ܿ�������ģ���Դ��
	NO.9-��������״̬����ָ�����ʱ�ɻ�ȡ�ŵ�״̬��������������Ԥ�У��Ź���ʱ�ſ�����
	NO.10-�޸�No.8��������ʹ����ģ�鲻�ȶ���崻����������β��������ܣ�ȥ���Զ��Ͽ����ӹ��ܣ��ع�No.7
	NO.11-���ӿ�������ͳ��(������״̬����),ȫ���ͳ�ƿ��Ŵ����������ϵ�ʱ����
		���޸�No.7�����ڼ�����Ҫʹ��CPU��������ʹ�ÿ���ģʽ����Ϊֱ�ӹص���ʱ��0��PWM��ʡ�磩
	NO.12-�޸�PWM��ʱ����ֵ��ʹ�жϼ��Ϊ0.05ms���Ӷ�ʵ�ֻ���������slow_rotate_back��������ģʽ
	NO.13-�����������ж�ָʾ�ơ����ӶϿ��������Զ���λ�Ĵ����ú������з�װ����������
	NO.14-�����ⲿDS1302ʱ��оƬ,ͷ�ļ�DS1302.h������������ʱ���йصĲ���
	NO.15-���ӷ��������������Ѽ�������������
	NO.16-��д�Դ����ַ�����Ĵ������룬������*��ʼ#������֧�ִ�һ��������
		  ��ͨ��������ԡ��������������Ѽ��ʱ�估������������ʱ������								
/*********************************************************************************************
˵����
��Windowsϵͳ�еġ������նˡ������������ڶ����� [ 4800��8���ޣ�1���� ]
�����STC-ISP�����еĴ������ֹ��ܣ������ڶ����� [ 4800��8���ޣ�1���� ]

/*********************************************************************************************/
//#include <STC12C2052AD.h>	//STC12C5AxxS2ϵ�е�Ƭ��ͷ�ļ�
#include <string.h>
#include <DS1302.h>

#define buffer_size 100	  // ���ڽ����ַ���������С
//#define	OVERTIME 180	  // ���ӳ�ʱʱ�䣨�룩



// ���Ŷ���
sbit PWM =P1 ^ 6 ;          // PWM�ź����
sbit KEY =P1 ^ 0 ;          // ��״̬����
sbit LED = P1 ^ 5;		  // ָʾ��
sbit BLUETOOTH = P1 ^ 4;  // ����ģ������״̬(����ʱ�ߵ�ƽ)
sbit BEEP = P1 ^ 7; 	  // ������

//����ȫ�ֱ���
xdata unsigned char cmd[50];  // �����ַ���
xdata unsigned char para[50]; // �����ַ�����ֻ�ܽ���һ��������

// �������飬�������д����ַ�������
xdata unsigned char *command[21]={
	"unlock",	 	    // ����
	"lock",			    // ����
    "lighton",			// ����
    "lightoff",			// �ص�
	"lightflash",		// ����
	"settime",		    // ����ʱ��
	"gettime",			// ��ȡʱ��
	"setremindtime",	// ���ù�������ʱ��
	"getremindtime",	// ��ȡ��������ʱ��
	"setalarmtime",		// ���ñ���ʱ������
	"getalarmtime",		// ��ȡ����ʱ������
	"getstate",			// ��ȡ����״̬
	"beepon",	    	// ����
	"remindon",			// ������������
	"remindoff",		// �رչ�������
	"alarmon",			// ������������
	"alarmoff",			// �رշ�������
	"getremind",		// ��ȡ���������Ƿ��
	"getalarm",			// ��ȡ���������Ƿ��
	"getcount",			// ��ȡ���Ŵ���
	"clearerro"		    // ����������
};

// ���������������������ر���
unsigned char separate = 0;
unsigned char cmd_index = 0;
bit cmd_start = 0;
bit cmd_end = 0;

// ���ر�־λ
bit ALRAMON = 0;             // ��������
bit OPEN = 1;				 // ���״̬
bit LIGHT = 0;				 // ����
bit FLASH = 0;				 // ����
bit ALARM = 0;				 // ����״̬
bit ALARMON = 1;			 // ��������
bit REMINDON = 1;			 // �������ѿ���
bit RECEIVE = 0;			 // ���ݽ��ձ�־λ

// ʹ��xdata�ɽ������address over flow����
xdata unsigned char yy,mo,dd,xq,hh,mm,ss;//����ʱ��ӳ��ȫ�ֱ�����ר�üĴ�����
xdata unsigned char erro = 0;		 // ��������������
xdata int count = 0;                 // 0.5ms��ʱ��1��������ʶ
xdata unsigned char angle;           // �Ƕȱ�ʶ
xdata unsigned char state = 3;	 	 // ���ڽ��յ��Ķ��״̬��ʶ
//xdata int millisecond = 0;		 // ��ʱ�������ʱ������2000Ϊһ�룩
//xdata int second = 0;				 // ��ʱ�����ʱ����
xdata int doorCount = 0;			 // ���Ŵ���ͳ��

/*********************************************************************************************
��������UART���ڳ�ʼ������
��  �ã�UART_init();
��  ������
����ֵ����
��  ��������UART���ڽ����жϣ��������ڽ��գ�����T/C1���������ʣ�ռ�ã�
��  ע���񵴾���Ϊ12MHz��PC���ڶ����� [ 4800��8���ޣ�1���� ]
/**********************************************************************************************/
void UART_init (void){
	EA = 1; //�������жϣ��粻ʹ���жϣ�����//���Σ�
	ES = 1; //����UART���ڵ��ж�

	TMOD = 0x21;	//��ʱ��T/C1������ʽ2
	SCON = 0x50;	//���ڹ�����ʽ1���������ڽ��գ�SCON = 0x40 ʱ��ֹ���ڽ��գ�
	TH1 = 0xF3;	//��ʱ����ֵ��8λ����
	TL1 = 0xF3;	//��ʱ����ֵ��8λ����
	PCON = 0x80;	//�����ʱ�Ƶ�����α��䲨����Ϊ2400��
	TR1 = 1;	//��ʱ������    
}
/**********************************************************************************************/

/*********************************************************************************************
��������UART���ڷ��ͺ���
��  �ã�UART_T (?);
��  ������ҪUART���ڷ��͵����ݣ�8λ/1�ֽڣ�
����ֵ���� 
��  �����������е����ݷ��͸�UART���ڣ�ȷ�Ϸ�����ɺ��˳�
��  ע��
/**********************************************************************************************/
void UART_T (unsigned char UART_data){ //���崮�ڷ������ݱ���
	SBUF = UART_data;	//�����յ����ݷ��ͻ�ȥ
	while(TI == 0);		//��鷢���жϱ�־λ
	TI = 0;			//����жϱ�־λΪ0���������㣩
}
/**********************************************************************************************/

/*********************************************************************************************
��������UART���ڷ����ַ�������
��  �ã�UART_TC (?);
��  ������ҪUART���ڷ��͵����ݣ�8λ/1�ֽڣ�
����ֵ���� 
��  �����򴮿ڷ���һ���ַ���,���Ȳ��ޡ�
��  ע������UART_TC("d9887321$"); �˺�����Ҫ#include <string.h>ͷ�ļ�֧�֡�
/**********************************************************************************************/
void UART_TC (unsigned char *str){
	while(*str != '\0'){
		UART_T(*str);
		*str++;
	}
	*str = 0;
}
/**********************************************************************************************/
/*********************************************************************************************
�����������뼶CPU��ʱ����
��  �ã�DELAY_MS (?);
��  ����1~65535����������Ϊ0��
����ֵ����
��  ����ռ��CPU��ʽ��ʱ�������ֵ��ͬ�ĺ���ʱ��
��  ע��Ӧ����1T��Ƭ��ʱi<600��Ӧ����12T��Ƭ��ʱi<125
/*********************************************************************************************/
void DELAY_MS (unsigned int a){
	unsigned int i;
	while( a-- != 0){
		for(i = 0; i < 600; i++);
	}
}
/*********************************************************************************************/
/*********************************************************************************************
�������������ת����
��  �ã�rotate(?);
��  ����1~5����������Ϊ0��
����ֵ����
��  �������ת������Ӧ�ĽǶ�
��  ע��������Ƕȶ�Ӧ��ϵ��1-0��, 2-45��, 3-90��, 4-135��, 5-180��
/*********************************************************************************************/
void rotate(int position)
{
	ET0 = 1; 		  // ����ʱ��1�ж�
	angle = position * 10; // �Ƕȸ�ֵ
	DELAY_MS(500);	  // ��ʱ�ȴ����ת��
	ET0 = 0;		  // �ض�ʱ��1�ж�
}
/*********************************************************************************************
��������������������
��  �ã�slow_rotate(?);
��  ����3��û����С�жϣ�ֻ���������ܿ�����
����ֵ����
��  �����������ת�������ŵĽǶ�
��  ע��
/*********************************************************************************************/
void slow_rotate_back(int position)
{	
	ET0 = 1; 		  // ����ʱ��1�ж�
	
	while (angle < position * 10)
	{	
		angle += 1; // �Ƕȸ�ֵ
		DELAY_MS(100);
	}

	DELAY_MS(500);	  // ��ʱ�ȴ����ת��
	ET0 = 0;		  // �ض�ʱ��1�ж�
}
/*********************************************************************************************/
/*********************************************************************************************
���������������������
��  �ã�get_cmd(byte_data) byte_data: SBUF�յ�������
��  ������
����ֵ����
��  �����Ѵ��ڽ��յ����ַ��ָ�Ϊһ�������һ���������ֱ�洢��cmd��para�ַ�������
��  ע�����������е����ݲ����Զ�������豻����
/**********************************************************************************************/
void get_cmd(unsigned char byte_data)
{
 	if (byte_data == '*') { 
		cmd_start = 1;
		cmd_index = 0;
		separate = 0; 
	}
	else if (byte_data == '#') { 
		cmd_start = 0;
		cmd_end = 1;
		if (separate == 1)
			para[cmd_index] = '\0';
		else
		    cmd[cmd_index] = '\0';  
	}
	else if (byte_data == ' ') {
		separate = 1;
		cmd[cmd_index] = '\0';
		cmd_index = 0;	
	}
	else if (cmd_start == 1)
	{
		switch (separate) {
			case 0:
			 	cmd[cmd_index++] = byte_data;
				break;
			case 1:
				para[cmd_index++] = byte_data;
				break;
		}
		
		if (cmd_index >= buffer_size)
			cmd_index = 0;
	}
}
/*********************************************************************************************/
/*********************************************************************************************
��������ִ�������
��  �ã�do_cmd(int n)
��  ����n �����������������鳤�ȣ�
����ֵ����
��  ���������������飬ƥ�䴮�ڽ��յ��������ִ�����������в�ͬ�����Ӧ�Ĳ�ͬ����
��  ע��ע��switch-case�±�˳������������˳���Ӧ
/**********************************************************************************************/
void do_cmd(int n)
{
 	int i = 0;

	erro = strcmp(cmd, "clearerro") == 0 ? 0 : erro;  // �������������������ִ��
	  
	for (i = 0; i < n; i++) {
	    if (strcmp(cmd, command[i]) == 0 && erro <= 3) {  // ����ƥ��ɹ��Ҵ������������3
			switch(i) {
			 	case 0:	 state = 1; break;  // ����
				case 1:  state = 3; break;  // ����
				case 2:  FLASH = 0; LIGHT = 1; LED = 0; break; // ����
				case 3:  LIGHT = 0; FLASH = 0; LED = 1; break; // �ص�
				case 4:	 LIGHT = 0; FLASH = 1; break; // ����
				case 5:	 Set_time(para); break;	// ����ʱ��
				case 6:  UART_TC(Get_time()); break;	 // ��ȡʱ��
				case 7:	 Set_remind_time(para); break;  // ���ù�������ʱ��
				case 8:	 UART_TC(Get_remind_time()); break; // ��ȡ��������ʱ��
				case 9:  Set_alarm_time(para); break; // ���ñ���ʱ������					   
				case 10: UART_TC(Get_alarm_time()); break; // ��ȡ����ʱ������
				case 11: UART_T(OPEN ? 0xff : 0x00); break;	// ��ȡ����״̬				
				case 12: BEEP = 0; break;  // ����
				case 13: REMINDON = 1; break;  // ������������
				case 14: REMINDON = 0; break;  // �رչ�������
				case 15: ALARMON = 1; break;   // ������������
				case 16: ALARMON = 0; break;   // �رշ�������
			    case 17: UART_T(REMINDON ? 0xff : 0x00); break; // ��ȡ���������Ƿ��
				case 18: UART_T(ALARMON ? 0xff : 0x00); break;	// ��ȡ���������Ƿ��			
				case 19: UART_T(doorCount / 256); UART_T(doorCount % 256); break; 
				 // ��ȡ���Ŵ�����ʮ�����ƣ��ȷ��͸߰�λ�ٷ��͵Ͱ�λ��
				
			}

			cmd[0] = '\0';
			para[0] = '\0';
			
			erro = 0; // ����������

			break;
		}

	}
	
	if (i == n && RECEIVE)	// �յ�������������У����������һ
	{
		erro++; 
		RECEIVE = 0;
	} 
}

/*********************************************************************************************/

 /*********************************************************************************************
��������UART���ڽ����жϴ�������
��  �ã�[SBUF�յ����ݺ��жϴ���]
��  ������
����ֵ����
��  ����UART���ڽ��յ�����ʱ�����жϣ��û������ݽ��д���
��  ע�������Ĵ��������Ӱ��������ݵĽ���
/**********************************************************************************************/
void UART_R (void) interrupt 4  using 1{ //�л��Ĵ����鵽1
	unsigned char UART_data; //���崮�ڽ������ݱ���

	TR0 = 1;		    // ��PWM
//	millisecond = 0;	// ��ʱ����
//	second = 0;

	ES=0; 				//�ش����ж�
	RI = 0;				//������жϱ�־λΪ0���������㣩
	UART_data = SBUF;	//�����յ�������������� UART_data

	RECEIVE = 1;		
	get_cmd(UART_data);
	//do_cmd(4);

	ES=1;//�������ж�		
	//SBUF = UART_data;	//�����յ����ݷ��ͻ�ȥ��ɾ��//����Ч��
	//while(TI == 0);	//��鷢���жϱ�־λ
	//TI = 0;		//����жϱ�־λΪ0���������㣩
}	
/**********************************************************************************************/

/*********************************************************************************************
����������ʱ��1��ʼ������
��  �ã�Time0_Init();
��  ������
����ֵ����
��  ��������0.5ms��ʱ
��  ע����
/*********************************************************************************************/
void Time0_Init()          //��ʱ����ʼ��
{
	TMOD = 0x21;           //��ʱ��0�����ڷ�ʽ1
	TH0  = 0xFF;
	TL0  = 0xCE;     		//12MZ����0xFE0B(0.5ms)
	EA = 1;					// �����ж�
	ET0 = 1;			   // ����ʱ���ж�
	TR0=1;                 //��ʱ����ʼ
}
/*********************************************************************************************
����������ʱ��0�жϴ�������
��  �ã���
��  ������
����ֵ����
��  ������������20ms��ռ�ձ�0.5~2.5ms�ߵ�ƽ��PWM��������ź�
��  ע����
/*********************************************************************************************/
void Time0_Int() interrupt 1 //�жϳ���
{
	TH0  = 0xFF;             //��װ��ֵ0xFE0B(0.5ms)
	TL0  = 0xCE;
	
	count++;				 // �ۼӼ�������

	//if (BLUETOOTH == 1)
	//	millisecond++;
	//if (millisecond == 2000)
	//{
	//	second++;
	//	millisecond = 0;
	//}

	if (count == angle)		 // �ߵ�ƽ������ռ�ձȵ���
		PWM = 0;
	else if (count == 400) {	 // һ������
	 	count = 0;
		PWM = 1;
	}

}
/*********************************************************************************************/

/*********************************************************************************************
�������������ϵ��ʼ������
��  �ã���
��  ������
����ֵ����
��  ������
��  ע���������
/*********************************************************************************************/
void door_init()
{
 	//BLUETOOTH = 0;  // ��Ҫ��ʼ�����ͣ�����������
	//POWER = 0;
	
	// ���ֳ�ʼ��
	UART_init();
	Time0_Init();
	Init_1302();  

	// ָʾ�ơ���������ʼ��
	LED = 1;
	BEEP = 1;

	//Set_1302("2016-04-27-3-23-45-00");

	//Set_time("14-02-50");

	Set_remind_time("00-30-00");
	Set_alarm_time("01-30-00,06-30-00");

	rotate(state);
	DELAY_MS(500); 	// �ȴ����λ�ó�ʼ��
	//PCON = 0x81;
}
/*********************************************************************************************/

/*********************************************************************************************
���������������ӶϿ���������
��  �ã�dis_bluetooth()
��  ������
����ֵ����
��  ����������ģ��Ͽ�����ʱ������Ӧ����
��  ע��
/*********************************************************************************************/
void dis_bluetooth()
{
 	if (!BLUETOOTH)	    // �������ӶϿ�
	{
		LED = 1;        // ��ָʾ��
		BEEP = 1;		// �ط�����
		LIGHT = 0;		// ���㳣����־λ
		FLASH = 0;		// ����������־λ

		state = 3;	    // ����Ƕȸ�λ��������
		//rotate(state);
		slow_rotate_back(state);  // ���������λ
		DELAY_MS(500); 	// �ȴ����λ��

		TR0 = 0;		// ��PWM
		//PCON = 0x81;  // �������ģʽ

	}
}
/*********************************************************************************************/

/*********************************************************************************************
��������������λ����
��  �ã�auto_back
��  ������
����ֵ����
��  �������ź������Զ���λ
��  ע��
/*********************************************************************************************/
void auto_back()
{			
	if (KEY != 0 && OPEN == 0) // ���ŵ����ŵĹ���
	{
		state = 3; 		// ���ź������Զ���λ
		OPEN = 1;
		doorCount++;	// ���Ŵ���ͳ��
	}
	else
	{
		OPEN = KEY;		// ˢ����״̬
	}
}

/*********************************************************************************************
��������ָʾ�ƴ�������
��  �ã�do_light()
��  ������
����ֵ����
��  ������ָʾ�ƽ��д���
��  ע��
/*********************************************************************************************/
void do_light()
{
	if (state == 1 && OPEN == 0 && LIGHT == 0)	 // ���������ֶ�����ʱ����
		LED = 0;			
	else if (LIGHT == 0)  // ���ֶ�����ʱ���		
		LED = 1;
		
	if (FLASH == 1)	  // ��˸��
	{
	 	LED = 0;
		DELAY_MS(100);
		LED = 1;
		DELAY_MS(100);			
	}

}
/*********************************************************************************************/
/*********************************************************************************************
�����������������Ѻ���
��  �ã�Time_remind()
��  ������
����ֵ����
��  �������趨������ʱ��ʱ��δ�������������
��  ע��һ����֮�ڹ��ŷ�������ֹͣ����
/*********************************************************************************************/
 void Time_remind()
 {
 	unsigned char rh, rm, rs;
	unsigned char *time = Get_remind_time();

	rh = ((time[0] - 0x30) * 16) + (time[1] - 0x30);
	rm = ((time[3] - 0x30) * 16) + (time[4] - 0x30);
	rs = ((time[6] - 0x30) * 16) + (time[7] - 0x30);

	if (hh == rh && mm == rm && REMINDON) // ��ָ��ʱ��
	{
		if (KEY != 0 && ss % 5 == 0)	// ��û�أ�ss % 5 == 0����5s������У�	
			ALARM = 1;  // ��������һ����
		else
			ALARM = 0;		
	}
	
 }

/**********************************************************************************************/

/*********************************************************************************************
��������������������
��  �ã�Time_alarm()
��  ������
����ֵ����
��  �������趨������ʱ������������״̬Ϊ�����������
��  ע��һ��������ʹ���Ź��Ϸ�����Ҳ����ֹͣ����
/*********************************************************************************************/
 void Time_alarm()
 {
 	unsigned char ah1, am1, as1;   // ������ʼʱ��
	unsigned char ah2, am2, as2;   // ��������ʱ��

	unsigned char *time = Get_alarm_time();

	ah1 = ((time[0] - 0x30) * 16) + (time[1] - 0x30);
	am1 = ((time[3] - 0x30) * 16) + (time[4] - 0x30);
	as1 = ((time[6] - 0x30) * 16) + (time[7] - 0x30);

	ah2 = ((time[9] - 0x30) * 16) + (time[10] - 0x30);
	am2 = ((time[12] - 0x30) * 16) + (time[13] - 0x30);
	as2 = ((time[15] - 0x30) * 16) + (time[16] - 0x30);

	if (hh >= ah1 && hh <= ah2 && mm >= am1 && mm <= am2 && KEY != 0 && ALARMON) // ����ʱ����������û��	 	
		ALARM = 1;  // ��������	

 }

/**********************************************************************************************/
/*********************************************************************************************
��������������
��  �ã���
��  ������
����ֵ����
��  ������
��  ע���������
/*********************************************************************************************/

void main()
{
	DELAY_MS(500);	// �ȴ��������ӶϿ�
   	P3M1 = 0x38;
	door_init();	// ������ʼ��

	while(1)
	{		
		rotate(state);		// ִ�ж��ת��

		dis_bluetooth();	// �������ӶϿ�����
		auto_back();		// �����Զ���λ
		do_light();			// ����ָʾ��

		Time_remind();		// ���������Ѽ��
		Time_alarm();		// �����������

		read_clockS(&yy,&mo,&dd,&xq,&hh,&mm,&ss);  // ����ʱ��
				
		BEEP = ~ALARM;		// ˢ�·�����״̬

		do_cmd(21);	   		// ִ�д����յ����ַ�������

		/*
		if (second > OVERTIME && BLUETOOTH == 1)	// ��ʱ����
		{
			POWER = 1;
			DELAY_MS(1000);		 // ����ģ�������ϵ磬�Ͽ�����
			POWER = 0;
			second = 0;
		}
		*/
							
	}
}


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

uint8_t PotAdcData;

GPIO_InitTypeDef Pot;
ADC_InitTypeDef Adcstruct;
ADC_CommonInitTypeDef AdcCommonstruct;

void config(){

	//Databus config
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	//led config
	Pot.GPIO_Mode=GPIO_Mode_OUT;
	Pot.GPIO_OType=GPIO_OType_PP;
	Pot.GPIO_Pin=GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15 ;
	Pot.GPIO_PuPd= GPIO_PuPd_NOPULL;
	Pot.GPIO_Speed=0x03;

	GPIO_Init(GPIOD, &Pot);


	//Pot config
	Pot.GPIO_Mode=GPIO_Mode_AN;
	Pot.GPIO_OType=GPIO_OType_PP;
	Pot.GPIO_Pin=GPIO_Pin_0; //PA0
	Pot.GPIO_PuPd=GPIO_PuPd_NOPULL;
	Pot.GPIO_Speed=0x03;

	GPIO_Init(GPIOA, &Pot);

	//Adc common struct config
	AdcCommonstruct.ADC_Mode=ADC_Mode_Independent;//adc baðýmsýz modda çalýþacak
	AdcCommonstruct.ADC_Prescaler=ADC_Prescaler_Div4;// ön bölücü 1/4 oranýnda
	ADC_CommonInit(&AdcCommonstruct);

	//Adc struct config
	Adcstruct.ADC_Resolution=ADC_Resolution_8b;//0-255 arasý çözünürlük
	ADC_Init(ADC1,&Adcstruct);

	ADC_Cmd(ADC1,ENABLE);// adc1 modunu aktif ettik
}

/*analog deðer okuma fonksiyonu(herzaman ayný fonksiyon kullanýlýr)
içerden  Sampletime saykýlýný ve hangi adc yi kullanýyosan onu yazman yeterli*/
uint8_t Read_ADC(){
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_56Cycles);

	ADC_SoftwareStartConv(ADC1);

	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);

	return ADC_GetConversionValue(ADC1);
}


int main(void)
{
  config();
  GPIO_ResetBits(GPIOD, GPIO_Pin_All);

  while (1)
  {
	  PotAdcData= Read_ADC();

	  switch(PotAdcData){
	  case 15:
		  GPIO_SetBits(GPIOD,GPIO_Pin_12 );
		  GPIO_ResetBits(GPIOD,GPIO_Pin_13 |GPIO_Pin_14|GPIO_Pin_15);
	   break;
	  case 30:
	      GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13 );
	  	  GPIO_ResetBits(GPIOD,GPIO_Pin_14 |GPIO_Pin_15);
	   break;
	  case 60:
	  	  GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13 |GPIO_Pin_14 );
	  	  GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	   break;
	  case 120:
	 	  GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13 |GPIO_Pin_14 );
	 	  GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	   break;
	  case 240:
	  	  GPIO_SetBits(GPIOD,GPIO_Pin_12|GPIO_Pin_13 |GPIO_Pin_14|GPIO_Pin_15 );
	   break;
	  }
  }

}


void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}

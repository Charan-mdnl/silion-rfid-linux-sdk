#include "ModuleReader.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void OnTagRead(int hReader, TAGINFO *tag, void* cookie)
{
	int i;
	printf("epc:");
 	for(i = 0; i < tag->Epclen; ++i)
 		printf("%02X", tag->EpcId[i]);
 	printf("\n");
}

void OnReadingError(int hReader, READER_ERR errcode, void* cookie)
{
	char rdrip[50];
	int internalcode;
	char *errdes;
	GetReaderAddress(hReader, rdrip);
	GetLastDetailError(hReader, &internalcode, &errdes);
	printf("reader %s error:%d, info:%s", rdrip, internalcode, errdes);
	CloseReader(hReader);
	/////
	//可在此处重新打开读写器并继续盘存标签，可尝试多次，如果多次打开读写器不成功可放弃。
	//////
}

int main()
{
	int hreader;
	Inv_Potls_ST invpotls;
	READER_ERR err;
	int ants[] = {1};
  AntPowerConf antpwrconf;
 
  err = InitReader_Notype(&hreader, "192.168.1.100", 4);
  if (err != 0)
  {
    printf("init reader err :%d\n", err);
    return -1;
  }

  //串口通信下可以修改默认波特率,需要重新上电,之后使用地址如：/dev/ttyS1:921600
  //test save buadrate
    int ismodifybaud=0;
   if(ismodifybaud)
	{ Default_Param df;
	int b=921600;
	df.val=&b;
	df.key=MTR_PARAM_SAVEINMODULE_BAUD;

	ParamSet(m_reader,MTR_PARAM_SAVEINMODULE,&df);
	return 0;
   }


   unsigned short Maxp;
	unsigned short Minp;

	if (ParamGet(m_reader, MTR_PARAM_RF_MAXPOWER, &Maxp) != MT_OK_ERR)
	{
		printf("获取功率上限失败");
		return -3;
	}
	if (ParamGet(m_reader, MTR_PARAM_RF_MINPOWER, &Minp) != MT_OK_ERR)
	{
		printf("获取功率下限失败");
		return -3;
	}

  invpotls.potlcnt = 1;
  invpotls.potls[0].potl = SL_TAG_PROTOCOL_GEN2;
  invpotls.potls[0].weight = 30;
  ParamSet(hreader, MTR_PARAM_TAG_INVPOTL, &invpotls);

  antpwrconf.antcnt = 4;
  antpwrconf.Powers[0].antid = 1;
  antpwrconf.Powers[0].readPower = 3000;
  antpwrconf.Powers[0].writePower = 3000;
  antpwrconf.Powers[1].antid = 2;
  antpwrconf.Powers[1].readPower = 3000;
  antpwrconf.Powers[1].writePower = 3000;
  antpwrconf.Powers[2].antid = 3;
 	antpwrconf.Powers[2].readPower = 3000;
	antpwrconf.Powers[2].writePower = 3000;
	antpwrconf.Powers[3].antid = 4;
	antpwrconf.Powers[3].readPower = 3000;
	antpwrconf.Powers[3].writePower = 3000;
	
	if (ParamSet(hreader, MTR_PARAM_RF_ANTPOWER, &antpwrconf) != MT_OK_ERR)
	{
		printf("ParamSet MTR_PARAM_RF_ANTPOWER err\n");
		return -1;
	}

	
	int ismoretags=1;

    if(ismoretags==1)
	{  
        //大量标签配置
	    int enc = 103;//RF_MODE_3
        if (ParamSet(hreader, MTR_PARAM_POTL_GEN2_TAGENCODING, &enc) != MT_OK_ERR)
       {
      printf("ParamSet tagcode failed");
      return -2;
        }
    }
	else if(ismoretags==2)
		{
		   int tg=3；

		   	if (ParamSet(m_hReader, MTR_PARAM_POTL_GEN2_TARGET, &tg) != MT_OK_ERR)
	       {
		       printf("ParamSet taget A-B failed");
               return -3;
           }	
			
			int m_qval=2；//静态Q值，根据标签量配置

			if (ParamSet(m_hReader, MTR_PARAM_POTL_GEN2_Q, &m_qval) != MT_OK_ERR)
	      {
		   printf("ParamSet Q failed");
		    return;
	      }
	 }

	

	OnReadingErrorBlock reblk;
	reblk.handler = OnReadingError;
	reblk.cookie = NULL;
	SetReadingErrorHandler(hreader, reblk);
	
	OnTagReadBlock trblk;
	trblk.handler = OnTagRead;
	trblk.cookie = NULL;
	SetTagReadHandler(hreader, trblk);

	BackReadOption BROption;
	memset(&BROption, 0, sizeof(BackReadOption));
	BROption.IsFastRead = 1; //快速模式和普通模式开关
        if(BROption.IsFastRead)
       {
        unsigned char  paraBuf[51]="Reader/Ex10fastmode";

        paraBuf[48]=0;
  paraBuf[49]=1;
  
        paraBuf[50]=0;//1为EX专用快速模式，0为普通快速模式

        cpara.pCusParam =paraBuf;
  
        cpara.CParamlen=1;
  
        READER_ERR ret = ParamSet(m_reader,MTR_PARAM_CUSTOM, &cpara);
        }

	BROption.ReadDuration = 250*sizeof(ants);
  BROption.ReadInterval = 0;
  err = StartReading(hreader, ants, 1, &BROption);
	if (err == MT_OK_ERR)
		printf("ResumeReading successfully\n");
	else
	{
		printf("ResumeReading failed err:%d\n", err);
		return -1;
	}
  while (1)
  {
		usleep(1000000);
	}
 	CloseReader(hreader);
	return 0;
}

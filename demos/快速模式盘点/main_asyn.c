#include "ModuleReader.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
time_t startt,endtt;
int tagcount=0;
TAGINFO taglist[1000];

int testseconds=10;//测试时间
 
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

	unsigned char  paraBuf[51]="Reader/Ex10fastmode";

        paraBuf[48]=0;
  paraBuf[49]=1;
  
        paraBuf[50]=1;//1为EX专用快速模式，0为普通快速模式

        cpara.pCusParam =paraBuf;
  
        cpara.CParamlen=1;
  
        READER_ERR ret = ParamSet(m_reader,MTR_PARAM_CUSTOM, &cpara);
	 
	err =AsyncStartReading(hreader,ants,1,0);
	 
	if (err == MT_OK_ERR)
	{	printf("ResumeReading successfully\n");

	startt=time((time_t*)NULL);
	endtt=startt;}
	else
	{
		printf("ResumeReading failed err:%d\n", err);
		return -1;
	}
	while (1)
	{
			endtt=time((time_t*)NULL);
	  int cmp=endtt-startt;
     if(cmp>=testseconds)
     {
		 AsyncStopReading(hreader);
		 break;
      }
	 else
	 {
		   err =AsyncGetTagCount(hreader, &tagcount);
           if (err == MT_OK_ERR)
		   {
			   int i;
                for(i=0;i<tagcount;i++)
				{
					TAGINFO pTInfo;
					  err =AsyncGetNextTag(hreader,&pTInfo);
					  if (err == MT_OK_ERR)
		            {
					 char out[124];
			          Hex2Str(pTInfo.EpcId, pTInfo.Epclen, out);

		     	      printf("epdid:%s   ", out);
					  }
				}
		   }
	 }
		usleep(1000);
	}



	CloseReader(hreader);
	return 0;
}

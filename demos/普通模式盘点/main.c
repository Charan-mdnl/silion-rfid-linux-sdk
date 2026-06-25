#include "../../ModuleReader.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int hreader;
	TAGINFO tag_;
	Inv_Potls_ST invpotls;
	int err;
	int ants[] = {1};
	unsigned char databuf[12];
	int tagcnt;
	int i;
	int f;
	AntPowerConf antpwrconf;
	
	char *ip_addr = "192.168.1.100";
	if (argc > 1) {
		ip_addr = argv[1];
	}
	err = InitReader_Notype(&hreader, ip_addr, 1);
	if (err != 0)
	{
		printf("init reader err :%d\n", err);
		return -1;
	}
	
        invpotls.potlcnt = 1;
        invpotls.potls[0].potl = SL_TAG_PROTOCOL_GEN2;
	invpotls.potls[0].weight = 30;
         ParamSet(hreader, MTR_PARAM_TAG_INVPOTL, &invpotls);

	//设置功率结构
	antpwrconf.antcnt = 1;
	antpwrconf.Powers[0].antid = 1; //天线号1
	antpwrconf.Powers[0].readPower = 3000; //读功率30dbm
	antpwrconf.Powers[0].writePower = 3000; //写功率30dbm

	if (ParamSet(hreader, MTR_PARAM_RF_ANTPOWER, &antpwrconf) != MT_OK_ERR)
	{
		printf("ParamSet MTR_PARAM_RF_ANTPOWER err\n");
		return -1;
	}


	printf("init reader successfully\n");
	while (1)
	{
		usleep(100000);//休眠100毫秒
		//指定天线数组和超时时间盘点epc
		err = TagInventory_Raw(hreader, ants, 1, 200, &tagcnt);
		if (err == MT_OK_ERR)
		{
			printf("------------------- tagcnt:%d\n", tagcnt);
			
 			for (i = 0; i < tagcnt; ++i)
 			{
				err = GetNextTag(hreader, &tag_);
 				printf("epc:\n");
 				for(f = 0; f < tag_.Epclen; ++f)
 				{
 					printf("%02X", tag_.EpcId[f]);
 				}
 				printf(" by ant %d, potl:%d\n", tag_.AntennaID, tag_.protocol);
 			}
		}
		else
			printf("TagInventory err:%d\n", err);

		/* 
		//读取bank区数据，tid数据，起始为0块，块数6.bank2
		if (GetTagData(hreader, 1, 2, 0, 6, databuf, NULL, 1000) == MT_OK_ERR)
		{
			for (i = 0; i < tagcnt; ++i)
			{
				printf("tid:\n");
				for(f = 0; f < 12; ++f)
				{
					printf("%02X", databuf[f]);
				}
				printf("\n");
			}
		}
		else
			printf("GetTagData err \n");
		*/
	}
	
	CloseReader(hreader);
	return 0;
}


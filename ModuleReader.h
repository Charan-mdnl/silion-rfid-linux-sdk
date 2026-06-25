#ifndef MODULE_M5E_FAMILY_READER_
#define MODULE_M5E_FAMILY_READER_

#define SDK_HEADER_FILE_VERSION 20210901

#if defined (_WIN32_WCE)
#define EXTERN __declspec(dllexport)
#elif defined (WIN32)
#define EXTERN __stdcall
#else
#define  EXTERN  
#endif

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum
	{
		MT_OK_ERR = 0,
		MT_IO_ERR = 1,
		MT_INTERNAL_DEV_ERR = 2,
		MT_CMD_FAILED_ERR = 3,
		MT_CMD_NO_TAG_ERR = 4,
		MT_M5E_FATAL_ERR = 5,
		MT_OP_NOT_SUPPORTED = 6,
		MT_INVALID_PARA = 7,
		MT_INVALID_READER_HANDLE = 8,
		MT_HARDWARE_ALERT_ERR_BY_HIGN_RETURN_LOSS = 9,
		MT_HARDWARE_ALERT_ERR_BY_TOO_MANY_RESET = 10,
		MT_HARDWARE_ALERT_ERR_BY_NO_ANTENNAS = 11,
		MT_HARDWARE_ALERT_ERR_BY_HIGH_TEMPERATURE = 12,
		MT_HARDWARE_ALERT_ERR_BY_READER_DOWN = 13,
		MT_HARDWARE_ALERT_ERR_BY_UNKNOWN_ERR = 14,
		M6E_INIT_FAILED = 15,
		MT_OP_EXECING = 16,
		MT_UNKNOWN_READER_TYPE = 17,
		MT_OP_INVALID = 18,
		MT_HARDWARE_ALERT_BY_FAILED_RESET_MODLUE = 19,
		MT_MAX_ERR_NUM = 20,
		MT_TEST_DEV_FAULT_1 = 51,
		MT_TEST_DEV_FAULT_2 = 52,
		MT_TEST_DEV_FAULT_3 = 53,
		MT_TEST_DEV_FAULT_4 = 54,
		MT_TEST_DEV_FAULT_5 = 55,
		MT_FREQUENT_ERR = 60, 
		MT_UPDFWFROMSP_OPENFILE_FAILED = 80,
		MT_UPDFWFROMSP_FILE_FORMAT_ERR = 81,

		MT_JNI_INVALID_PARA = 101,
		MT_MAX_INT_NUM = 0x7FFFFFFF,
	}READER_ERR;

	typedef enum 
	{
		MODOULE_NONE,
		MODOULE_R902_M1S,
		MODOULE_R902_M2S,
		MODOULE_M5E,
		MODOULE_M5E_C,
		MODOULE_M6E,
		MODOULE_PR9000,
		MODOULE_M5E_PRC,
		MODOULE_M6E_PRC,
		MODOULE_M6E_MICRO,
		MODOULE_SLR1100,
		MODOULE_SLR1200,
		MODOULE_SLR1300,
		MODOULE_SLR3000,
		MODOULE_SLR5100,
		MODOULE_SLR5200,
		MODOULE_SLR3100,
		MODOULE_SLR3200,
		MODOULE_SLR5300,
		MODOULE_SLR5900,
		MODOULE_SLR5800,
		MODOULE_SLR6000,
		MODOULE_SLR6100,
		MODOULE_SIM7100,
		MODOULE_SIM7200,
		MODOULE_SIM7300,
		MODOULE_SIM7400,
		MODOULE_SIM7500,
		MODOULE_SIM7600,
		MODOULE_SIM3100,
		MODOULE_SIM3200,
		MODOULE_SIM3300,
		MODOULE_SIM3400,
		MODOULE_SIM3500,
		MODOULE_SIM3600,
		MODOULE_SIM5100,
		MODOULE_SIM5200,
		MODOULE_SIM5300,
		MODOULE_SIM5400,
		MODOULE_SIM5500,
		MODOULE_SIM5600,
		MODOULE_SIM3110,
		MODOULE_SIM5110,
		MODOULE_SIM7110,
	}Module_Type;
	
	typedef enum 
	{
		MAINBOARD_NONE,
        MAINBOARD_ARM7,
        MAINBOARD_SERIAL,
        MAINBOARD_WIFI,
		MAINBOARD_ARM9,
		MAINBOARD_ARM9_WIFI,
		MAINBOARD_INTEGRATED_SERIAL,
		MAINBOARD_ARM9_V2,
		MAINBOARD_ARM9_WIFI_V2,
	} MaindBoard_Type;

	typedef enum
	{
		MODULE_TWO_ANTS,
		MODULE_FOUR_ANTS,
		MODULE_THREE_ANTS,
		MODULE_ONE_ANT,
		PR9000,
        MODULE_ARM7_TWO_ANTS,
		MODULE_ARM7_FOUR_ANTS,
		M6E_ARM7_FOUR_ANTS,
		M56_ARM7_FOUR_ANTS,
		R902_M1S,
		R902_M2S,
		ARM7_16ANTS,
		SL_COMMN_READER,
		ANT_ONE_EXT_MULTIPORTS,
	} Reader_Type;
	
	typedef struct  
	{
		Module_Type module;
		MaindBoard_Type board;
		Reader_Type logictype;
		int selfcheckants;
		 
	} HardwareDetails;

	typedef enum
	{
		SL_TAG_PROTOCOL_NONE              = 0x00,
		SL_TAG_PROTOCOL_ISO180006B        = 0x03,
		SL_TAG_PROTOCOL_GEN2              = 0x05,
		SL_TAG_PROTOCOL_ISO180006B_UCODE  = 0x06, 
		SL_TAG_PROTOCOL_IPX64             = 0x07,
		SL_TAG_PROTOCOL_IPX256            = 0x08,
		SL_TAG_PROTOCOL_GB            = 0x10,
	} SL_TagProtocol;

	typedef enum
	{
		RG_NONE = 0x0,
		RG_NA = 0x01,
		RG_EU = 0x02,
		RG_EU2 = 0X07,
		RG_EU3 = 0x08,
		RG_KR = 0x03,
		RG_PRC = 0x06,
		RG_PRC2 = 0x0A,

		RG_OPEN = 0xFF,

		RG_IN=0x04,
		RG_JP=0x05,
		RG_CE_HIGH = 0x0C,
        RG_HK = 0x0D,
        RG_TAIWAN = 0x0E,
        RG_MALAYSIA = 0x0F,
        RG_SOUTH_AFRICA = 0x10,
        RG_BRAZIL = 0x11,
        RG_THAILAND = 0x12,
        RG_SINGAPORE = 0x13,
        RG_AUSTRALIA=0x14,
        RG_URUGUAY = 0x16,
        RG_VIETNAM = 0x17,
        RG_ISRAEL = 0x18,
        RG_PHILIPPINES = 0x19,
        RG_INDONESIA = 0x1A,
        RG_NEW_ZEALAND = 0x1B,
        RG_PERU = 0x1C,
        RG_RUSSIA = 0x1D,
        RG_CE_LOW_HIGH=0x1E,
		RG_JP2_LBT6=0x1F,
		RG_JP3_NLBT19=0x20,
	} Region_Conf;
	
	typedef enum
	{
	
		MTR_PARAM_POTL_GEN2_SESSION,
		MTR_PARAM_POTL_GEN2_Q,
		MTR_PARAM_POTL_GEN2_TAGENCODING,
		MTR_PARAM_POTL_GEN2_MAXEPCLEN,
		MTR_PARAM_RF_ANTPOWER,
		MTR_PARAM_RF_MAXPOWER,
		MTR_PARAM_RF_MINPOWER,
		MTR_PARAM_TAG_FILTER,
		MTR_PARAM_TAG_EMBEDEDDATA,
		MTR_PARAM_TAG_INVPOTL,
		MTR_PARAM_READER_CONN_ANTS,
		MTR_PARAM_READER_AVAILABLE_ANTPORTS,
		MTR_PARAM_READER_IS_CHK_ANT,
		MTR_PARAM_READER_VERSION,
		MTR_PARAM_READER_IP,
		MTR_PARAM_FREQUENCY_REGION,
		MTR_PARAM_FREQUENCY_HOPTABLE,
		MTR_PARAM_POTL_GEN2_BLF,
		MTR_PARAM_POTL_GEN2_WRITEMODE,

		MTR_PARAM_POTL_GEN2_TARGET, //0:A; 1:B; 2:A->B; 3:B->A
		MTR_PARAM_TAGDATA_UNIQUEBYANT,
		MTR_PARAM_TAGDATA_UNIQUEBYEMDDATA,
		MTR_PARAM_TAGDATA_RECORDHIGHESTRSSI,
		MTR_PARAM_RF_TEMPERATURE,
		MTR_PARAM_RF_HOPTIME,
		MTR_PARAM_RF_LBT_ENABLE,
		MTR_PARAM_RF_SUPPORTEDREGIONS,
		MTR_PARAM_POTL_SUPPORTEDPROTOCOLS,
		MTR_PARAM_POTL_ISO180006B_BLF,
		MTR_PARAM_POTL_GEN2_TARI, //0:Tari of 25 microseconds;1:Tari of 12.5 microseconds;2:Tari of 6.25 microseconds
		MTR_PARAM_TRANS_TIMEOUT,
		MTR_PARAM_TAG_EMDSECUREREAD,
		MTR_PARAM_TRANSMIT_MODE,
		MTR_PARAM_POWERSAVE_MODE,
		MTR_PARAM_TAG_SEARCH_MODE,

		MTR_PARAM_POTL_ISO180006B_MODULATION_DEPTH,
		MTR_PARAM_POTL_ISO180006B_DELIMITER,
		MTR_PARAM_RF_ANTPORTS_VSWR,

		//alter by dkg
		MTR_PARAM_SAVECONFIGURATION,
		MTR_PARAM_MAXINDEX,

		//by dkg
		MTR_PARAM_INTERNALSAVECONFIGURATION,
		MTR_PARAM_CUSTOM,

		MTR_PARAM_READER_WATCHDOG,
		MTR_PARAM_READER_ERRORDATA,
		MTR_PARAM_RF_HOPANTTIME,

		MTR_PARAM_TAG_MULTISELECTORS,

		MTR_PARAM_SAVEINMODULE,
		MTR_PARAM_SAVEINMODULE_BAUD,

		MTR_PARAM_TAG_OPPOTL,
	}Mtr_Param;
    
	typedef enum
	{
		TAGOP_EMBEDDEDSPEC_NONE = 0,
		TAGOP_MONZA4_QTPEEK2PRIVATE = 1,
		TAGOP_MONZA4_QTPEEK2PUBLIC = 2,
		TAGOP_U8=3,
	} TagOpEmbeddedSpecReq_Code;

	typedef enum
	{
		AUTOSET_MODE_ID_5123=5123,
		AUTOSET_MODE_ID_5124=5124,
		AUTOSET_MODE_ID_5141=5141,
		AUTOSET_MODE_ID_5146=5146,
		AUTOSET_MODE_ID_5148=5148,
		AUTOSET_MODE_ID_5180=5180,
		AUTOSET_MODE_ID_5185=5185,
		AUTOSET_MODE_ID_5189=5189,

		AUTOSET_MODE_ID_5222=5222,
		AUTOSET_MODE_ID_5241=5241,
		AUTOSET_MODE_ID_5244=5244,
		AUTOSET_MODE_ID_5281=5281,
		AUTOSET_MODE_ID_5285=5285,
		AUTOSET_MODE_ID_5291=5291,

		AUTOSET_MODE_ID_5323=5323,
		AUTOSET_MODE_ID_5324=5324,
		AUTOSET_MODE_ID_5342=5342,
		AUTOSET_MODE_ID_5343=5343,
		AUTOSET_MODE_ID_5345=5345,
		AUTOSET_MODE_ID_5382=5382,
		AUTOSET_MODE_ID_5385=5385,
		AUTOSET_MODE_ID_5386=5386,


		IMPINJ_INVENTORY_MODE_ID_4123=4123,
		IMPINJ_INVENTORY_MODE_ID_4124=4124,
		IMPINJ_INVENTORY_MODE_ID_4141=4141,
		IMPINJ_INVENTORY_MODE_ID_4146=4146,
		IMPINJ_INVENTORY_MODE_ID_4148=4148,
		IMPINJ_INVENTORY_MODE_ID_4180=4180,
		IMPINJ_INVENTORY_MODE_ID_4185=4185,
		IMPINJ_INVENTORY_MODE_ID_4189=4189,

		IMPINJ_INVENTORY_MODE_ID_4222=4222,
		IMPINJ_INVENTORY_MODE_ID_4241=4241,
		IMPINJ_INVENTORY_MODE_ID_4244=4244,
		IMPINJ_INVENTORY_MODE_ID_4281=4281,
		IMPINJ_INVENTORY_MODE_ID_4285=4285,
		IMPINJ_INVENTORY_MODE_ID_4291=4291,

		IMPINJ_INVENTORY_MODE_ID_4323=4323,
		IMPINJ_INVENTORY_MODE_ID_4324=4324,
		IMPINJ_INVENTORY_MODE_ID_4342=4342,
		IMPINJ_INVENTORY_MODE_ID_4343=4343,
		IMPINJ_INVENTORY_MODE_ID_4345=4345,
		IMPINJ_INVENTORY_MODE_ID_4382=4382,
		IMPINJ_INVENTORY_MODE_ID_4385=4385,
		IMPINJ_INVENTORY_MODE_ID_4386=4386,

	}IMPINJ_MODE_ID;

	typedef enum
	{
		FM0= 0,
		M2= 1,
		M4= 2,
		M8= 3,
		PROF0_R2000= 16,
		PROF1_R2000= 17,
		PROF2_R2000= 18,
		PROF3_R2000= 19,
		PROF4_R2000= 20,
		PROF5_R2000= 21,
		RFM_1_EX_SFM= 101,
		RFM_3_EX_SFM= 103,
		RFM_5_EX_SFM= 105,
		RFM_7_EX_SFM= 107,
		RFM_11_EX_SFM= 111,
		RFM_12_EX_SFM= 112,
		RFM_13_EX_SFM= 113,
		RFM_15_EX_SFM= 115,
		RFM_103_EX= 203,
		RFM_345_EX= 45,
		RFM_120_EX= 220,
		RFM_2_SFM= 102,
		RFM_4_SFM= 104,
		RFM_6_SFM= 106,
		RFM_8_SFM= 108,
		RFM_9_SFM= 109,
		RFM_10_SFM= 110,
		RFM_14_SFM= 114,
		RFM_19_SFM_6B= 119,
		RFM_20_SFM_GB2= 120,
		RFM_21_SFM_GB2= 121,
		RFM_22_SFM_GB2= 122,
		RFM_23_SFM_GB2= 123,
		RFM_24_SFM_GB2= 124,
		RFM_25_SFM_GB2= 125,
		RFM_26_SFM_GB2= 126,
		RFM_27_SFM_GB2= 127,
		RFM_28_SFM_GB2= 128,
		RFM_29_SFM_GB2= 129,
		RFM_30_SFM_GB2= 130,
		RFM_31_SFM_GB2= 131,
		RFM_32_SFM_GB2= 132,
		RFM_33_SFM_GB2= 133,
		RFM_34_SFM_GB2= 134,
		RFM_35_SFM_GB2= 135,
		RFM_102_EX22=0xFF000066,
		RFM_103_EX22=0xFF000067,
		RFM_104_EX22=0xFF000068,
		RFM_120_EX22=0xFF000078,
		RFM_123_EX22=0xFF00007B,
		RFM_124_EX22=0xFF00007C,
		RFM_125_EX22=0xFF00007D,
		RFM_126_EX22=0xFF00007E,
		RFM_141_EX22=0xFF00008D,
		RFM_146_EX22=0xFF000092,
		RFM_147_EX22=0xFF000093,
		RFM_148_EX22=0xFF000094,
		RFM_180_EX22=0xFF0000B4,
		RFM_185_EX22=0xFF0000B9,
		RFM_189_EX22=0xFF0000BD,
		RFM_202_EX22=0xFF0000CA,
		RFM_203_EX22=0xFF0000CB,
		RFM_205_EX22=0xFF0000CD, 
		RFM_222_EX22=0xFF0000DE,
		RFM_223_EX22=0xFF0000DF,
		RFM_224_EX22=0xFF0000E0,
		RFM_225_EX22=0xFF0000E1,
		RFM_226_EX22=0xFF0000E2,
		RFM_241_EX22=0xFF0000F1,
		RFM_244_EX22=0xFF0000F4,
		RFM_281_EX22=0xFF000119,
		RFM_285_EX22=0xFF00011D,
		RFM_291_EX22=0xFF000123,
		RFM_302_EX22=0xFF00012E,
		RFM_323_EX22=0xFF000143,
		RFM_324_EX22=0xFF000144,
		RFM_325_EX22=0xFF000145,
		RFM_326_EX22=0xFF000146,
		RFM_342_EX22=0xFF000156,
		RFM_343_EX22=0xFF000157,
		RFM_344_EX22=0xFF000158,
		RFM_345_EX22=0xFF000159,
		RFM_382_EX22=0xFF00017E,
		RFM_385_EX22=0xFF000181,
		RFM_386_EX22=0xFF000182,
	}RFMODE;
	
#define MAXANTCNT 16
#define MAXIPSTRLEN 50
#define HOPTABLECNT 100
#define MAXEPCBYTESCNT 62
#define MAXEMBDATALEN 128
#define MAXINVPOTLSCNT 6
#define MAXGPICNT 8

	typedef struct
	{
		int GpiId;
		int State;
	} GpiState_ST;
	typedef struct
	{ 
		int gpiCount;
		GpiState_ST gpiStats[MAXGPICNT];
	} GpiInfo_ST;

	typedef struct
	{
		void *pCusParam;
		int CParamlen;
	} CustomParam_ST;

	typedef struct  
	{
		unsigned int ReadCnt;
		int RSSI;
		unsigned char AntennaID;
		unsigned int Frequency;
		unsigned int TimeStamp;
		unsigned short EmbededDatalen;
		unsigned char EmbededData[MAXEMBDATALEN];
		unsigned char Res[2];
		unsigned short Epclen;
		unsigned char PC[2];
		unsigned char CRC[2];	
		unsigned char EpcId[MAXEPCBYTESCNT];
		int Phase;
		SL_TagProtocol protocol;
	} TAGINFO;
	
	typedef struct  
	{
		unsigned char AntennaID;
		unsigned char AntConnected;
		unsigned int Frequency;
		unsigned char Temp;
		unsigned char HL;
		 int isGet;
	} MODUSINFO;

	typedef enum
	{
		TYPE_NONE=-1,
		TYPE_MODUSPARMS=0,
		TYPE_IMPINJFPARMS=1,
		TYPE_CC33BYTES=2,
		TYPE_SELECTCOMMAND=3,
	}STRCUCTTYPE;

	 typedef struct   
	{
		unsigned char CMD;/**< CMD 决定结构体哪些成员变量有效 */
		int isReTemp;
		int isReAntCT;
		int isReAntHL;
		unsigned char antQ[16];
		int Qlen;
		unsigned short anttime[16];
		int Tlen;
		unsigned short timeout;
	}MODUSPARMS;

	typedef struct
	{
		unsigned char CMD;
		unsigned short IMPINJ_MODE_ID;
		unsigned char  IMPINJ_FAST_MODE;
		//unsigned char T;//0 epcid 32，1 epc 24
		MODUSPARMS *sub;
		unsigned char SCANIDM;//1，2，3（8bits ，16，24）
		unsigned char SCANID[4];//高地址字节在前
	}IMPINJ_FASTPARMS;

 	typedef enum
	{
		 SelC_Inventoried_S0=0,
		 SelC_Inventoried_S1=1,
		 SelC_Inventoried_S2=2,
		 SelC_Inventoried_S3=3,
		 SL=4,
		 RFU1=5,
		 RFU2=6,
		 RFU3=7,

	}SelCmd_Target;


	typedef enum
	{
		Mat_SLorA_NMat_NSLorB=0,
		Mat_SLorA_NMat_no=1,
		Mat_no_NMat_SLorB=2,
		Mat_negateSLorAB_NotMat_no=3,
		Mat_NSLorB_NotMat_SLorA=4,
		Mat_NSLorB_NotMat_no=5,
		Mat_no_NotMat_SLorA=6,
		Mat_no_NotMat_negateSLorAB=7,
	}SelCmd_Action;


	typedef struct
	{
		unsigned char  seltarget;
		unsigned char  selaction;
		unsigned char  selbank;
		int			   seladdr;
		unsigned char  selbitslen;
		unsigned char  seldatas[32];
		unsigned char  seltruction;
	}SELData;

	typedef struct
	{
		unsigned short RFU;
		unsigned short  selconfig1;
		unsigned short  selconfig2;
		int selcnt;
		//unsigned short ant;
		int ants[16];
		int antlen;
		int RFU2;
		unsigned short pow;
         
		SELData seldataary[12];
	}CMDAA4DData;

	typedef struct
	{
		int tagtype;
		int pwdtype;
		int ApIndexStartBitsInEpc;
		int ApIndexBitsNumInEpc;
		int bank;
		int address;
		int blkcnt;
		unsigned int accesspwd;
	} EmbededSecureRead_ST;

	typedef struct  
	{
		char ip[50];
		char mask[50];
		char gateway[50];
	} Reader_Ip;

	typedef struct  
	{
		char ver[64];
	} Reader_Ver;

	typedef struct  
	{
		int bank;
		unsigned int startaddr;
		int flen;
		unsigned char *fdata;
		int isInvert;
	} TagFilter_ST;

#define MaxTagSelBytesCount 32
	typedef struct  
	{
		int bank;
		unsigned int startaddr;
		int slen;
		unsigned char sdata[MaxTagSelBytesCount];
	} TagSelector_ST;
#define MaxTagSelcetorCount 16
	typedef struct
	{
		TagSelector_ST multiselectors[MaxTagSelcetorCount];
		int selectorcnt;
	} MultiTagSelectors_ST;

	typedef struct  
	{
		int bank;
		int startaddr;
		int bytecnt;
		unsigned char *accesspwd;
	} EmbededData_ST;

	typedef struct  
	{
		int antid;
		unsigned short readPower;
		unsigned short writePower;
	} AntPower;

	typedef struct  
	{
		int antcnt;
		AntPower Powers[MAXANTCNT];
	}AntPowerConf;

	typedef struct
	{
		int andid;
		unsigned short power;
		Region_Conf region;
		int frequencys[HOPTABLECNT];
		unsigned char vswrs[HOPTABLECNT];
		int frecount;
	} AntPortsVSWR;

	typedef struct  
	{
		int antcnt;
		int connectedants[MAXANTCNT];
	} ConnAnts_ST;

	typedef struct
	{
		unsigned int htb[HOPTABLECNT];
		int lenhtb;
	}HoptableData_ST;

	typedef struct  
	{
		SL_TagProtocol potl;
		int weight;
	} Inv_Potl;

	typedef struct  
	{
		int potlcnt;
		Inv_Potl potls[MAXINVPOTLSCNT];
	} Inv_Potls_ST;

	int EXTERN GetSDKVersion(void);

#if !defined (__SCM__)

	typedef struct
	{
		Mtr_Param key;
		void *customkey;
		int isdefault;
		void* val;
		//int valen;
	}Default_Param;

	READER_ERR EXTERN GetHardwareDetails_BaseType(int hReader, unsigned char *outbuf);

	READER_ERR EXTERN InitReader_Notype(int *hReader, char * src, int antscnt);

	READER_ERR EXTERN AuthPassword(int hReader, unsigned char *pwd, int pwdlen);

#endif

	READER_ERR EXTERN GetHardwareDetails(int hReader, HardwareDetails *phd);

	READER_ERR EXTERN InitReader(int *hReader, char * src, Reader_Type rtype);

	void EXTERN CloseReader(int hReader);

	READER_ERR EXTERN GetTagData(int hReader, int ant, 
		unsigned char bank, unsigned int address, 
		int blkcnt, unsigned char *data, 
		unsigned char *accesspasswd, unsigned short timeout);
	
	READER_ERR EXTERN GetTagDataEx(int hReader, int ant, 
		unsigned char bank, unsigned int address, 
		int blkcnt, unsigned char *data, 
		unsigned char *accesspasswd, unsigned short timeout, 
		int option, void *pSpec);

	READER_ERR EXTERN WriteTagData(int hReader, int ant, 
		unsigned char bank, unsigned int address, 
		unsigned char *data, int datalen, 
		unsigned char *accesspasswd, unsigned short timeout);
	
	READER_ERR EXTERN WriteTagEpc(int hReader, int ant, unsigned char *Epc,
		int epclen,unsigned short timeout);

	READER_ERR EXTERN WriteTagEpcEx(int hReader, int ant, unsigned char *Epc,
		int epclen, unsigned char *accesspwd, unsigned short timeout);
	
	READER_ERR EXTERN TagInventory_Count(int hReader, int *ants, int antcnt, unsigned short timeout, 
		int *tagcnt, int isclearbuf);
	
	READER_ERR EXTERN TagInventory(int hReader, int *ants, int antcnt, unsigned short timeout, 
		TAGINFO *pTInfo, int *tagcnt);
	
	READER_ERR EXTERN TagInventory_Raw(int hReader, int *ants, int antcnt, unsigned short timeout, 
		int *tagcnt);

	READER_ERR EXTERN TagInventory_BaseType(int hReader, int *ants, int antcnt, unsigned short timeout, 
		unsigned char *outbuf, int *tagcnt);
	
	READER_ERR EXTERN TagInventory_Single(int hReader, int ant, unsigned short metaflag, unsigned short timeout, TAGINFO *tag);

	READER_ERR EXTERN GetNextTag(int hReader, TAGINFO *pTInfo);

	READER_ERR EXTERN GetNextTag_BaseType(int hReader, unsigned char *outbuf);

	READER_ERR EXTERN PsamTransceiver(int hReader, int soltid, int coslen, 
		unsigned char *cos, int *cosresplen, unsigned char *cosresp, 
		unsigned char *errcode, unsigned short timeout);

	READER_ERR EXTERN GetReaderAddress(int hReader, char *addr);

	READER_ERR EXTERN ReadTagTemperature(int hReader, int ant,unsigned char bank, 
		int wordAddress, int wordCount, int alltime, int timeselwait, 
		int timereadwait, unsigned short metaflag, unsigned char *accesspasswd, 
		unsigned char *getdata,int *datalen);

	READER_ERR EXTERN ReadTagLED(int hReader, int ant,unsigned short metaflag, 
		unsigned short timeout,unsigned char *getdata,int *datalen);

	char* EXTERN LookupErrorString(int derrcode);

	READER_ERR EXTERN GetLastDetailError(int hReader, int *derrcode, char **errstring);

	typedef enum
	{
		GPITRIGGER_NONE = 0,
		GPITRIGGER_TRI1START_TRI2STOP = 1,
		GPITRIGGER_TRI1START_TIMEOUTSTOP = 2,
		GPITRIGGER_TRI1ORTRI2START_TIMEOUTSTOP = 3,
		GPITRIGGER_TRI1ORTRI2START_TRI1ORTRI2STOP = 4,
	} GpiTrigger_Type;
	
#if !defined (__SCM__)
	
	typedef void (*ReadingErrorHandler)(int hReader, READER_ERR errcode, void* cookie);
	typedef struct
	{
		ReadingErrorHandler handler;
		void *cookie;
	} OnReadingErrorBlock;
	READER_ERR EXTERN SetReadingErrorHandler(int hReader, OnReadingErrorBlock bok);

	typedef void (*TagReadHandler)(int hReader, TAGINFO *tag, void* cookie);
	typedef struct
	{
		TagReadHandler handler;
		void *cookie;
	} OnTagReadBlock;
	READER_ERR EXTERN SetTagReadHandler(int hReader, OnTagReadBlock bok);
	  
	typedef void (*GpiStateHandler)(int hReader, int triggerid, GpiInfo_ST *gpist, void* cookie);
	/*
	bordertype:1,开始盘点；2，停止盘点
	reason：1，gpi触发；2，超时
	*/
	typedef void (*GpiTriggerBoundaryHandler)(int hReader, int bordertype, int reason, void* cookie);
	typedef struct
	{
		//满足触发条件1和2时进行通知
		GpiStateHandler gshandler;
		//通知读写器开始和停止盘存的触发原因和读写器状态
		GpiTriggerBoundaryHandler gtbhandler;
		void *cookie;
	} OnGpiTriggerBlock;
	READER_ERR EXTERN SetGpiTriggerHandler(int hReader, OnGpiTriggerBlock bok);

	typedef void (*InvIdleHandler)(int hReader, void* cookie);
	typedef struct
	{
		InvIdleHandler handler;
		void *cookie;
	} OnInvIdleBlock;
	READER_ERR EXTERN SetInvIdleHandler(int hReader, OnInvIdleBlock bok);

	typedef int (*SwitchNextAntHandler)(void *pData, int *nextchan, int *nextlogant, int *invants, int antcnt);
	typedef int (*SwitchAntBoundaryHandler)(void *pData);
	typedef int (*CustomFuncHandler) (void *pRPData, void *pSPData);
	typedef struct
	{
		SwitchNextAntHandler SwitchHandler;
		void *pSwitchData;
		SwitchAntBoundaryHandler PreSwitchHandler;
		void *pPreSwitchData;
		SwitchAntBoundaryHandler AftSwitchHandler;
		void *pAftSwitchData;
		CustomFuncHandler CustomHandler;
		void *pCustomData;
	} MFRAntSwitchBlock;
	READER_ERR EXTERN SetSwitchNextAntHandler(int hReader, MFRAntSwitchBlock blk);

	typedef struct
	{
		int IsAntennaID;
		int IsReadCnt;
		int IsRSSI;
		int IsFrequency;
		int IsTimestamp;
		int IsRFU;
		int IsEmdData;
		int IsProtocol;
	} TagMetaFlags;


	typedef struct
	{
		GpiInfo_ST GpiTrigger1States;
		GpiInfo_ST GpiTrigger2States;
		GpiTrigger_Type GpiTriggerType;
		int StopTriggerTimeout;
	} GPITrigger;

	typedef struct
	{
		unsigned short ReadDuration;
		unsigned int ReadInterval;
		int IsFastRead;
		unsigned char FastReadDutyRation;
		TagMetaFlags TMFlags;
		GPITrigger GpiTrigger;
		int IsGPITrigger;
	} BackReadOption;

	READER_ERR EXTERN StartReading(int hReader, int *ants, int antcnt, BackReadOption *pBRO);

	READER_ERR EXTERN StopReading(int hReader);

	READER_ERR EXTERN AsyncStartReading(int hReader, int *ants, int antcnt, int option);

	READER_ERR EXTERN AsyncGetTagCount(int hReader, int *tagcnt);

	READER_ERR EXTERN AsyncGetNextTag(int hReader, TAGINFO *pTInfo);

	READER_ERR EXTERN AsyncStopReading(int hReader);

	READER_ERR EXTERN FirmwareLoadFromSerialPort(char *serialportpath, char *filepath);

	READER_ERR EXTERN ChipLoadFromSerialPort(char *serialportpath, char *filepath);

	READER_ERR EXTERN RebootReader(char *address);

	READER_ERR EXTERN GetDeviceVersion(char *serialportpath, unsigned char *sfver);

	int EXTERN  A9_HttpPostFromUart(char *uartaddr,char *method, char *json, int timeout, char *respjson);

	typedef struct
	{
		int (*OnBefInvHandler)(int *ants, int antcnt, void *cbPara);
		int (*OnAftInvHandler)(void *cbPara);
		int (*OnTagReadHandler)(TAGINFO *tag, void *cbPara);
		int (*OnIdleHandler)(void *cbPara);
		int (*OnAntSwitchHandler)(int ecode, void *edata, void *cbPara);
		int (*OnErrorHandler)(int ecode, void *edata, void *cbPara);
		void *Cb_BefInvData;
		void *Cb_AftInvData;
		void *Cb_TagReadData;
		void *Cb_IdleData;
		void *Cb_AntSwitchData;
		void *Cb_ErrorData;
	} AsyncInvEvtCallBack_ST;
	READER_ERR EXTERN SetAsyncInvEvtHandler(int hReader,AsyncInvEvtCallBack_ST *cb_st);

#endif

	typedef enum
	{
		LOCK_OBJECT_KILL_PASSWORD = 0x01, //锁定对象为销毁密码
		LOCK_OBJECT_ACCESS_PASSWD = 0x02, //锁定对象为访问密码
		LOCK_OBJECT_BANK1 = 0x04, //锁定对象为bank1
		LOCK_OBJECT_BANK2 = 0x08,  //锁定对象为bank2
		LOCK_OBJECT_BANK3 = 0x10, //锁定对象为bank3		
	} Lock_Obj;
	
	typedef enum
	{
		LOCK_OBJECT_GBTID = 0x00, //锁定对象为TID
		LOCK_OBJECT_GBEPC = 0x10, //锁定对象为EPC
		LOCK_OBJECT_GBSEC = 0x20, //锁定对象为安全区
		LOCK_OBJECT_GBUSE = 0x30,  //锁定对象为用户
	 
	} Lock_Obj_GB;

	typedef enum
	{
		GB_USE_SUB0 = 0x00,  
		GB_USE_SUB1 = 0x01,  
		GB_USE_SUB2 = 0x02,  
		GB_USE_SUB3 = 0x03,  
		GB_USE_SUB4 = 0x04,  
		GB_USE_SUB5 = 0x05,  
		GB_USE_SUB6 = 0x06,  
		GB_USE_SUB7 = 0x07,  
		GB_USE_SUB8 = 0x08,  
		GB_USE_SUB9 = 0x09,  
		GB_USE_SUB10 = 0x0A,  
		GB_USE_SUB12 = 0x0B,
		GB_USE_SUB13 = 0x0C,
		GB_USE_SUB14 = 0x0D,
		GB_USE_SUB15 = 0x0E,
		GB_USE_SUB16 = 0x0F,
	} GB_USE_SUB;

	typedef enum
	{
		KILL_PASSWORD_UNLOCK = 0x0000,
		KILL_PASSWORD_LOCK = 0x0200, //销毁密码密码锁定
		KILL_PASSWORD_PERM_LOCK = 0x0300, //销毁密码永久锁定
		KILL_PASSWORD_PERM_UNLOCK = 0x0100, //销毁密码永久不锁定

		ACCESS_PASSWD_UNLOCK = 0x00,
		ACCESS_PASSWD_LOCK = 0x80, //访问密码密码锁定
		ACCESS_PASSWD_PERM_LOCK = 0xC0, //访问密码永久锁定
		ACCESS_PASSWD_PERM_UNLOCK = 0x40, //访问密码永久锁定

		BANK1_UNLOCK = 0x00,
		BANK1_LOCK = 0x20, //bank1密码锁定
		BANK1_PERM_LOCK = 0x30, //bank1永久锁定
		BANK1_PERM_UNLOCK = 0x10, //bank1永久不锁定
			
		BANK2_UNLOCK = 0x00,
		BANK2_LOCK = 0x08, //bank2密码锁定
		BANK2_PERM_LOCK = 0x0C,//bank2永久锁定
		BANK2_PERM_UNLOCK = 0x04,//bank2永久不锁定
			
		BANK3_UNLOCK = 0x00,
		BANK3_LOCK = 0x02, //bank3密码锁定
		BANK3_PERM_LOCK = 0x03, //bank3永久锁定
		BANK3_PERM_UNLOCK = 0x01, //bank3永久不锁定
	} Lock_Type;
	
	typedef enum
	{
		Config_Bank_Pro=0x00,
		Confgi_Bank_Sec=0x04,
		 
	} Lock_Type_GB_Config;
	
	typedef enum
	{
		Pro_ReadAndWrite=0x00,
		Pro_ReadOnly=0x01,
		Pro_WriteOnly=0x02,
        Pro_NONE=0x03,
		 
	} Lock_Type_GB_Config_Pro;
	
	typedef enum
	{
		Sec_Res=0x00,
		Sec_NotIden=0x01,
		Sec_IdenNoSafeTran=0x02,
        Sec_IdenSafeTran=0x03,
		 
	} Lock_Type_GB_Config_Sec;

	READER_ERR EXTERN LockTag(int hReader, int ant, 
		unsigned char lockobjects, unsigned short locktypes,
		unsigned char *accesspasswd, unsigned short timeout);

	READER_ERR EXTERN LockTagEx(int hReader, int ant, 
		unsigned short mask, unsigned short action,
		unsigned char *accesspasswd, unsigned short timeout);

	READER_ERR EXTERN KillTag(int hReader, int ant, 
		unsigned char *killpasswd,
		unsigned short timeout);
	
	READER_ERR EXTERN Lock180006BTag(int hReader, int ant, int startblk, 
		int blkcnt, unsigned short timeout);

	READER_ERR EXTERN BlockPermaLock(int hReader, int ant, int readlock, int startblk, 
		int blkrange, unsigned char *mask, unsigned char *pwd, unsigned short timeout);

	READER_ERR EXTERN BlockErase(int hReader, int ant, int bank, int wordaddr, int wordcnt, 
		unsigned char *pwd, unsigned short timeout);

	READER_ERR EXTERN SetGPO(int hReader, int gpoid, int state);

	READER_ERR EXTERN GetGPI(int hReader, int gpiid, int *state);

	READER_ERR EXTERN GetGPIEx(int hReader, GpiInfo_ST *gpiinfo);

	typedef enum
	{
		NXP_SetReadProtect,
		NXP_ResetReadProtect,
		NXP_ChangeEAS,
		NXP_EASAlarm,
		NXP_Calibrate,
			
		ALIEN_Higgs2_PartialLoadImage,
		ALIEN_Higgs2_FullLoadImage,
			
		ALIEN_Higgs3_FastLoadImage,
		ALIEN_Higgs3_LoadImage,
		ALIEN_Higgs3_BlockReadLock,
		ALIEN_Higgs3_BlockPermaLock,

		IMPINJ_M4_Qt,
		YiLian_Temperature,
		YiLian_Led,
		NXP_U8_InventoryMode = 20,
	} CustomCmdType;
	
	typedef struct
	{
		unsigned char AccessPwd[4];
		int CmdType; //0 = read the QT control bits in cache;1 = write the QT control bits 
		int MemType; //0 = Tag uses Private Memory Map;1 = Tag uses Public Memory Map
		int PersistType; //0 = write the QT Control to volatile memory;1 = write the QT Control to nonvolatile memory
		int RangeType; //0 = Tag does not reduce range;1 = Tag reduces range if in or about to be in OPEN or SECURED state
		unsigned short TimeOut;
	} IMPINJM4QtPara;

	typedef struct
	{
		int MemType;//0 = Tag uses Private Memory Map;1 = Tag uses Public Memory Map
		int RangeType;//0 = Tag does not reduce range;1 = Tag reduces range if in or about to be in OPEN or SECURED state
	} IMPINJM4QtResult;

	typedef struct  
	{
		unsigned char AccessPwd[4];
		int isSet;
		unsigned short TimeOut;
	} NXPChangeEASPara;
	
	typedef struct  
	{
		unsigned char DR;
		unsigned char MC;
		unsigned char TrExt;
		unsigned short TimeOut;
	} NXPEASAlarmPara;
	
	typedef struct  
	{
		unsigned char EASdata[8];
	} NXPEASAlarmResult;
	
	typedef struct  
	{
		unsigned char AccessPwd[4];
		unsigned char BlkBits;
		unsigned short TimeOut;
	} ALIENHiggs3BlockReadLockPara;
	
	READER_ERR EXTERN SyncStartFastReading(int hReader, int* ants, int antcnt, int option);
	READER_ERR EXTERN SyncGetNextTag(int hReader, TAGINFO *tag);
	READER_ERR EXTERN SyncStopFastReading(int hReader);

#if !defined (__SCM__)

	typedef struct
	{
		unsigned char ip[4];
		unsigned char nm[4];
		unsigned char gw[4];
		unsigned char dns[4];
		unsigned char mac[6];
		unsigned char dhcp;
		unsigned short sport;

		Module_Type modtype;
		MaindBoard_Type bdtype;
		
		unsigned char bdfwver[4];
		unsigned char modfwver[4];

		unsigned char wmode;
		unsigned int wstate;
	}DevSearchRdrInfo_ST;

	READER_ERR EXTERN SearchDevices(DevSearchRdrInfo_ST *devlist, int *listcnt, int timeout_ms);
	READER_ERR EXTERN ResetRfidModule(int hReader);

	READER_ERR EXTERN AsyncGetNextTag_BaseType(int hReader, unsigned char *outbuf);

	READER_ERR EXTERN GetLastDetailError_BaseType(int hReader, unsigned char *outbuf);

	READER_ERR EXTERN GetGPIEx_BaseType(int hReader, unsigned char *outbuf);
/*
	data:要发送的数据
	datalen：要发送数据的长度，单位字节
	timeout：超时时间，单位毫秒
	返回值：0表示成功，-1表示失败
*/
	int EXTERN DataTransportSend(int hReader, unsigned char *data, int datalen, int timeout);
	
/*
	data:接收缓冲区
	datalen：要接收数据的长度，单位字节
	timeout：超时时间，单位毫秒
	返回值：0表示成功，-1表示失败
*/
	int EXTERN DataTransportRecv(int hReader, unsigned char *data, int datalen, int timeout);

	READER_ERR EXTERN SaveDataOnReader(int hReader, int address, unsigned char *data, int datalen);
	
	READER_ERR EXTERN ReadDataOnReader(int hReader, int address, unsigned char *data, int datalen);

	READER_ERR EXTERN EraseDataOnReader(int hReader);

	READER_ERR EXTERN GetReaderDetails(int hReader, unsigned char *info);

#endif

	typedef struct {
		 STRCUCTTYPE type;
		 union {
			  MODUSPARMS ms;
			  IMPINJ_FASTPARMS im;
		 } data;
	} SpecialStruct;
	
	READER_ERR EXTERN CustomCmd(int hReader, int ant, CustomCmdType cmdtype, void *CustomPara, void *CustomRet);

	READER_ERR EXTERN CustomCmd_BaseType(int hReader, int ant, int cmdtype, unsigned char *CustomPara, unsigned char *CustomRet);

	READER_ERR EXTERN ParamGet(int hReader, Mtr_Param key, void *val);

	READER_ERR EXTERN ParamSet(int hReader, Mtr_Param key, void *val);

	READER_ERR EXTERN SpecialSensorTagOp(int hReader,int ant,int type,int timeout,unsigned char *data,int datalen,
		unsigned char *pwd,unsigned char *outdata,int *outdatalen);

    READER_ERR EXTERN SpecialStructDataConvert(int type,void *ObjectData,unsigned char *data,int *datalen);

	READER_ERR EXTERN SpecialBytesDataConvert(unsigned char *data,int *datalen,SpecialStruct *ssobject);

	void EXTERN Hex2Str(unsigned char *buf, int len, char *out);
	
	void EXTERN Str2Hex(const char *buf, int len, unsigned char *hexbuf);
	
	void EXTERN Str2Binary(const char *buf,int len,unsigned char *binarybuf);



#if defined (__SCM__)

	typedef struct  
	{
		unsigned int seconds;
		unsigned int milisecs;
	} SCM_TIME_ST;
	 
	typedef void SCM_GetSysClock(SCM_TIME_ST *time);
	typedef void SCM_Sleep(int milisecs);
	typedef int SCM_Trans_Recv(unsigned char *data, unsigned int len, unsigned int timeout);
	typedef int SCM_Trans_Send(unsigned char *data, unsigned int len, unsigned int timeout);
	typedef int SCM_Trans_Open();
	typedef int SCM_Trans_Close();
	typedef int SCM_Trans_SetPhySpeed(unsigned int speed);
	typedef int SCM_Trans_ClearRecvBuffer();
	typedef int SCM_Trans_Flush();
	typedef int SCM_Trans_IsFinResp(unsigned char *resp);

	typedef struct  
	{
		SCM_GetSysClock *scmGetSysClock;
		SCM_Sleep *scmSleep;
		SCM_Trans_Recv *scmTransRecv;
		SCM_Trans_Send *scmTransSend;
		SCM_Trans_Open *scmTransOpen;
		SCM_Trans_Close *scmTransClose;
		SCM_Trans_SetPhySpeed *scmTransSetPhySpeed;
		SCM_Trans_ClearRecvBuffer *scmTransClearRecvBuffer;
		SCM_Trans_Flush *scmTransFlush;
		SCM_Trans_IsFinResp *scmTransIsFinResp;
	} SCM_READER_CLK_TRANS;
	
	READER_ERR EXTERN SCMInitEnv(SCM_READER_CLK_TRANS clk_trans);
	READER_ERR EXTERN SCMSetDgPrintf(int pt);
	READER_ERR EXTERN StartTagInventory(int hReader, int *ants, int antcnt, unsigned short timeout);
	READER_ERR EXTERN IsFinTagInventory(int hReader, int *tagcnt);

	READER_ERR EXTERN StartGetTagData(int hReader, int ant, 
		unsigned char bank, unsigned int address, 
		int blkcnt,  unsigned char *accesspasswd, unsigned short timeout);
	READER_ERR EXTERN IsFinGetTagData(int hReader, unsigned char *data);

#endif

	
/*
以下这些函数的功能已经被ParamGet和ParamSet所代替，不建议再使用了

*/
	READER_ERR EXTERN WriteTagEpc(int hReader, int ant, 
		unsigned char *Epc, int epclen, unsigned short timeout);

	READER_ERR EXTERN SetInvPotl(int hReader, int potlcnt, SL_TagProtocol *potls, int *pweithts);

	READER_ERR EXTERN SetFilter(int hReader, int bank, unsigned int startaddr, 
		unsigned char *filterdata, int datalen, int isInvert);
	
	READER_ERR EXTERN ResetFilter(int hReader);
	
	READER_ERR EXTERN SetEmbededData(int hReader, int bank, int startaddr, int bytecnt, 
		unsigned char *accesspwd);
	
	READER_ERR EXTERN ResetEmbededData(int hReader);
	
	READER_ERR EXTERN GetAllAnts(int hReader, int *ants, int *antscnt);
 
	READER_ERR EXTERN GetAntsPower(int hReader, int *antcnt, unsigned short *rpwrs, unsigned short *wpwrs);

	READER_ERR EXTERN GetIpInfo(int hReader, char *ip, char *subnet, char *gateway);

	READER_ERR EXTERN GetGen2Session(int hReader, int *sess);

	READER_ERR EXTERN SetRegion(int hReader, Region_Conf rg);
	
	READER_ERR EXTERN SetIpInfo(int hReader, char *ip, char *subnet, char *gateway);

	READER_ERR EXTERN SetReadPower(int hReader, unsigned short power);

	READER_ERR EXTERN SetWritePower(int hReader, unsigned short power);
	
	READER_ERR EXTERN SetEPCLength(int hReader, int len);

	READER_ERR EXTERN SetGen2Session(int hReader, int sess);

	READER_ERR EXTERN SetGen2MVal(int hReader, int mval);

	READER_ERR EXTERN SetAntsPower(int hReader, int *ants, int antcnt, unsigned short *rpwrs, unsigned short *wpwrs);

	READER_ERR EXTERN GetPowerLimit(int hReader, unsigned short *MaxPower, unsigned short *MinPower);
	
#ifdef __cplusplus
}
#endif

#endif



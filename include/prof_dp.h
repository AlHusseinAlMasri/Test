	
/*********************************************************************
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Profibus Master Simulator                          *
 *   module name: prof_dp.h                                          *
 *        author: Ulrich Holeschak                                   *
 *          date: 06/11/2000                                         *
 *                                                                   *
 *      RCS info:                                                    *
 *         $Date: 2010/04/01 11:54:46 $
 *       $Locker:  $
 *     $Revision: 1.18 $
 *        $State: Exp $
 *                                                                   *
 *       history: 1.0 new                                            *
 *       history: 2.0 with DPE                                       *
 *                                                                   *
 *   description: PROFIBUS DP and DPE state machine                  *
 *                Operates with several FDL interfaces               *
 *                                                                   *
 *********************************************************************/

/*********************************************************************
	Defines
 *********************************************************************/


#ifndef _PROF_DP_H
#define _PROF_DP_H

#include "dpe_def.h"	/* definitions for dpe */

/* Custom additions */

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#define CALLBACK __stdcall
#else
typedef void* HWND;      // Placeholder for HWND
typedef bool BOOL;       // Use standard bool
typedef unsigned int UINT;
typedef unsigned long DWORD;
#define CALLBACK         // Empty definition for CALLBACK
#endif


#define _WIN32
#define _cplusplus


/* definition of callbacks */
#ifdef _WIN32
#define EXPO1
#define EXPO2 CALLBACK
#else
#define EXPO1
#define EXPO2 CALLBACK _export
#endif

/* message types for Notification messages (WPARAM) */
/* Notification messages hold these values in WPARM to distinguish the message types */
#define	MSG_TYPE_ASYNC		0x0001		/* async. operation finished, LPARAM=holds operation ID */
#define	MSG_TYPE_DPCYCLE	0x0002		/* This message is sent, after completion of one PROFIBUS DP cycle */
#define	MSG_TYPE_DPE		0x0004		/* message on DPE confirmation or indication */

/* thise macros help to analyse the functions result value DP_RESULT */
#define	DP_RESULT_CODE(_rcode) (_rcode & 0xFFFF)			/* result of code of operation */
#define	DP_RESULT_ID(_rcode) ((_rcode>>16) & 0xFFFF)		/* result ID of async operation */
															/* (only for DP_INCOMPLETE) */

/*********************************************************************
	Typedefs
 *********************************************************************/

typedef struct
{	unsigned char	offset;
	unsigned char	len;
} PROFISAFE_IO_DATA_STRC;

typedef struct
{	unsigned char	cfg_start;		// set on startup: start byte of configuration for PROFIsafe module
	unsigned char	cfg_len;		// set on startup: lenght of configuration for PROFIsafe module
	unsigned char	prm_start;		// set on startup: start byte of parameter for PROFIsafe module

	PROFISAFE_IO_DATA_STRC	out_data;	// receive: PROFIsafe output data parameter
										// OA_C  - second bit position, first byte after the F-Output Data.
										// activate_FV_C - first bit position, first byteafter the F-Output Data.

	PROFISAFE_IO_DATA_STRC	in_data;	// receive: PROFIsafe input data parameter
										// OA_Req_S - second bit position, first byte after the F-Input Data.
										// FV_activated_S - first bit position, first byte after the F-Input  Data.

	unsigned char	Comm_Error;			// receive: 1=communication error, 0=no error
} PROFISAFE_STRC; /* 8 bytes */

/* Data structure for cyclic PROFIBUS DP operations */
#define LABVIEW

#ifndef LABVIEW
typedef struct
{
	unsigned char	DPV1_Supported;				/* (1 byte) set on startup: enable DPV1 support, */
												/* a C_Ref identical to the slave address is created for MSAC1 access */
	unsigned char	Max_Channel_Data_Length;	/* (2 bytes)set on startup: max DPV1 channel length (4..244) */
	unsigned char	Extra_Alarm_SAP;			/* (3 bytes)set on startup: Extra DPV1 Alarm SAP */
	unsigned int	C1_Response_Timeout;		/* (5 bytes)set on startup: C1 Response Timeout */
	unsigned char 	NormParameters [7];			/* (12 bytes)set on startup: norm parameter. Ident number is read from slave */
	unsigned char 	UserParamLength;			/* (13 bytes)set on startup: user parameter length */
	unsigned char 	UserParameters [248];		/* (261 bytes)set on startup: user parameter */
	unsigned char 	ConfigLength;				/* (262 bytes)set on startup: configuration length, 0==read config from slave */
	unsigned char 	Configuration  [256];		/* (518 bytes)set on startup: configuration */
	unsigned char 	OutputLength;				/* (519 bytes)automatically set by DPOpenSlave() : output length */
	unsigned char 	OutputData     [256];		/* (775 bytes)set on startup and with DPWriteSlaveData(): output data to send */
	PROFISAFE_STRC	f_Param		   [18];		/* (18x8 = 144 -> 919 bytes)PROFIsafe modules info*/
	unsigned char 	InputLength;				/* (920 bytes)receive: input length */
	unsigned char 	InputData      [256];		/* (1176 bytes)receive: input data */
	unsigned char 	NormDiagnosis  [6];			/* (1182 bytes)receive: norm diagnosis */
	unsigned char 	UserDiagLength;				/* (1183 bytes)receive: user diag length */
	unsigned char 	UserDiagnosis  [256];		/* (1439 bytes)receive: user diagnosis */
	BOOL			bCommunicate;				/* (1443 bytes -> 1 Bool = 4 bytes) receive: slave is communicating (TRUE) */
	BOOL			bOperate;					/* (1447 bytes)receive: slave is in state operate (TRUE) */
	BOOL			bNewDiag;					/* (1451 bytes)receive: new diag data ,cleared by DPReadSlaveData() */
} DP_DATA;
#else
typedef struct
{
	unsigned char	ucDpData [1451];			/* Dummy data array for LabView's DLL importer; must use "Adapt to type" when passing the actual structure */
}DP_DATA;	
#endif

/* Result codes for functions */
typedef enum
{
	DP_NO_ERROR = 0x0000,		/* 0: no error */
	DP_GENERIC_ERROR,			/* 1: generic error */
	DP_CALLED_TWICE,			/* 2: DP function have been called double (only with DPSetDispatchMessage()) */
	DP_OPEN_FAILED,				/* 3: device (FDL) opening failed */
	DP_FDL_LOAD_FAILED,			/* 4: unable to load FDL library */
	DP_NOT_OPEN,				/* 5: FDL not opened */
	DP_THREAD_ENABLED,			/* 6: error, background thread enabled: function disabled */
	DP_NO_THREAD_ENABLED,		/* 7: error, background thread is not enabled: function disabled */
	DP_OUT_OF_MEMORY,			/* 8: out of memory */
	DP_CONVERTER_FAILED,		/* 9: PROFIBUS converter not found, failed or bad type */
	DP_ILL_ADDRESS,				/* 10: illegal DP slave address */
	DP_SAP_OPEN_FAILED,			/* 11: PROFIBUS SAP opening failed */
	DP_SLAVE_OPEN,				/* 12: slave is allread open */
	DP_SLAVE_NOT_OPEN,			/* 13: slave is not opened */
	DP_SLAVE_NOT_FOUND,			/* 14: slave not found */
	DP_ILLEGAL_CONFIG,			/* 15: illegal PROFIBUS configuration */
	DP_CFG_ERROR,				/* 16: configuration error on startup */
	DP_PRM_ERROR,				/* 17: parameter error on startup */
	DP_CFG_AND_PRM_ERROR,		/* 18: configuration and parameter error on startup */
	DP_BAD_LENGTH,				/* 19: data length incorrect */
	DP_INCOMPLETE,				/* 20: background operation initiated: use DPGetAsyncResult() for answer. High word holds operation ID */
	DP_NORESULT,				/* 21: no background operation finished DPGetAsyncResult() */
} _DP_RESULT;

/* type of result code */
typedef unsigned long	DP_RESULT;

/* request codes for async. operations */
typedef enum
{
	CMD_OPEN = 0,						/* command open slave */
	CMD_CLOSE,							/* command close slave */
	CMD_GETCFG,							/* command get cfg */
	CMD_SENDPRM,						/* command send parameter */
	CMD_IDENTIFY,						/* command get ident number */
	CMD_ADDR,							/* command change address */
} CMD_TYPES;

/* structure for async. command data by DPGetAsyncResult() */
typedef struct
{
	CMD_TYPES		uCmd;					/* initiate command type */
	unsigned int	uID;					/* id code of operation (high byte of DP_INCOMPLETE result) */
	DP_RESULT		ResultCode;				/* result error code of operation */
											/* union holding result for serveral commands: */
	union
	{
		struct								/* result of DPOpenSlave() CMD_OPEN */
		{
			DP_DATA			pDpData;		/* result slave data */
		} Open;
		struct								/* result of DPGetCfg() CMD_GETCFG */
		{
			unsigned char	ucLength;		/* result config length */
			unsigned char	Buffer[256];	/* result configuration */
		} GetCfg;
		struct								/* result of DPIdentifySlave() CMD_IDENTIFY */
		{
			unsigned int	uIdentNumber;	/* result ident number */
		} Ident;
	} Data;
} ASYNC_RESULT;

/*********************************************************************
	Prototypes
 *********************************************************************/

#if defined __cplusplus
extern "C" {
#endif

/*************************************************************
 *
 *  unsigned long DPInit(const char *pDllName, const char *pInitString);
 *
 *  description:
 *		Init DP and FDL module.
 *
 *  parameters:
 *		pDllName: name of FDL dll
 *				NULL=RS232 (fdlrs.dll) (default)
 *				Options:
 *					"fdlrs": Operation with serial PROFIBUS UART
 *					"fdlcp": Operation with SIEMENS Profibus cards. SINEC driver must be installed first.
 *							Tested only with CP5412A2 and SOFTNET DP!!!
 *
 *		pInitString: Init string for FDL.
 *					fdlrs: Com port "COMx", NULL=="COM2" (default)
 *					fdlcp: Name of PROFIBUS card Entry Point, NULL=="/CP_L2_1:/FLC" (default)
 *  returns:
 *		_DP_RESULT error code
 */ 
 
unsigned long DPInit(const char *pDllName, const char *pInitString);

/*************************************************************
 *
 *  unsigned long DPExit(void);
 *
 *  description:
 *		Exit DP and FDL module.
 *
 *  parameters:
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPExit(void);

/*************************************************************
 *
 *  unsigned long DPSetDispatchMessage (int (CALLBACK* DispatchFunction)(MSG *));
 *
 *  description:
 *		Set dispatcher for messages during communication.
 *		Warning: Use this function carefully, because there is the danger of
 *				multiple DP Function calls. We recommed to use DPEnableCommThread()
 *				for background communication. In thise case never use DPSetDispatchMessage()!
 *
 *  parameters:
 *		DispatchFunction: Callback function for message dispatches (Normally with DispatchMessage())
 *
 *  returns:
 *		_DP_RESULT error code
 */

//unsigned long DPSetDispatchMessage (int (CALLBACK* DispatchFunction)(MSG *));

/*************************************************************
 *
 *  unsigned long DPSethWnd (HWND hWnd);
 *
 *  description:
 *		Set window handle of main window. Only necessary if fdlcp is used
 *		and DPEnableCommThread() is NOT activated!
 *
 *  parameters:
 *		hWnd: Main window handle
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPSethWnd (HWND hWnd);

/*************************************************************
 *
 *  unsigned long DPIdentifySlave (unsigned char ucSlvAddr, unsigned int *puIdentNumber, BOOL bNoRetry);
 *
 *  description:
 *		Get ident number of specified slave. This function could also be used to find
 *		Slaves on the bus. The FDL has no retry for this operation.
 *
 *  parameters:
 *		ucSlvAddr: Address of slave to get ident number from (0..126)
 *		puIdentNumber: Pointer to buffer for result ident number.
 *				If DPEnableCommThread() is activated this parameter is not used (could be NULL)
 *				In this case the result code is DP_INCOMPLETE (High byte is ID)
 *				and the result is received with DPGetAsyncResult()
 *		bNoRetry: TRUE: No retry on fdl layer (fast!)
 *
 *  returns:
 *		_DP_RESULT error code
 *		If DPEnableCommThread() is activated the result code is DP_INCOMPLETE (High byte is ID)
 */ 

unsigned long DPIdentifySlave (unsigned char ucSlvAddr, unsigned int *puIdentNumber, BOOL bNoRetry);

/*************************************************************
 *
 *  unsigned long DPChangeAddress(unsigned char ucOldAddr, unsigned char ucNewAddr);
 *
 *  description:
 *		Change the address of the specified slave.
 *
 *  parameters:
 *		ucOldAddr: Current slave address (0..126)
 *		ucNewAddr: New slave address (0..126)
 *
 *  returns:
 *		_DP_RESULT error code
 *		If DPEnableCommThread() is activated the result code is DP_INCOMPLETE (High byte is ID)
 */ 

unsigned long DPChangeAddress(unsigned char ucOldAddr, unsigned char ucNewAddr);

/*************************************************************
 *
 *  unsigned long DPGetCfg(unsigned char ucSlvAddr, unsigned char *pBuffer, unsigned char *pucLength);
 *
 *  description:
 *		Get the default configuration of the specified slave.
 *
 *  parameters:
 *		ucSlvAddr: Slave address (0..126)
 *		pBuffer: Buffer for Config result. Must have size 256.
 *		pucLength: Result length of configuration
 *				If DPEnableCommThread() is activated pBuffer and pucLength are not used (could be NULL)
 *				In this case the result code is DP_INCOMPLETE (High byte is ID)
 *				and the result is received with DPGetAsyncResult()
 *
 *  returns:
 *		_DP_RESULT error code
 *		If DPEnableCommThread() is activated the result code is DP_INCOMPLETE (High byte is ID)
 */ 

unsigned long DPGetCfg(unsigned char ucSlvAddr, unsigned char *pBuffer, unsigned char *pucLength);

/*************************************************************
 *
 *  unsigned long DPSendPrm(unsigned char ucSlvAddr, DP_DATA * pDpData)
 *
 *  description:
 *		Send extra parameters to the specified slave.
 *
 *  parameters:
 *		ucSlvAddr: Slave address (0..126)
 *		pDpData: Data Structure with new parameter data for the slave
 *					Update NormParameters and UserParameters
 *
 *  returns:
 *		_DP_RESULT error code
 *		If DPEnableCommThread() is activated the result code is DP_INCOMPLETE (High byte is ID)
 */ 

unsigned long DPSendPrm(unsigned char ucSlvAddr, DP_DATA * pDpData);

/*************************************************************
 *
 *  unsigned long DPOpenSlave(unsigned char ucSlvAddr, DP_DATA * pDpData, unsigned int uOpenTimeout);
 *
 *  description:
 *		Open the communication with the specified slave
 *
 *  parameters:
 *		ucSlvAddr: Slave address (0..126)
 *		pDpData: Structure with init data. Look at DP_DATA with fields to initialize.
 *				We recommend to init all unsued fields with 0. After function return date will be updated.
 *				If DPEnableCommThread() is activated the update data is received with DPGetAsyncResult().
 *				In this case the result code is DP_INCOMPLETE (High byte is ID).
 *		uOpenTimeout: Timeout in sec. for slave initialisation. If 0 is specified, the result
 *				of initialisation is not tested. But if thes salve doesn't exit, an error returns.
 *
 *  returns:
 *		_DP_RESULT error code
 *		If DPEnableCommThread() is activated the result code is DP_INCOMPLETE (High byte is ID)
 */ 

unsigned long DPOpenSlave(unsigned char ucSlvAddr, DP_DATA * pDpData, unsigned int uOpenTimeout);

/*************************************************************
 *
 *  unsigned long DPCloseSlave(unsigned char ucSlvAddr);
 *
 *  description:
 *		Close the communication with the specified slave
 *
 *  parameters:
 *		ucSlvAddr: Slave address (0..126)
 *
 *  returns:
 *		_DP_RESULT error code
 *		If DPEnableCommThread() is activated the result code is DP_INCOMPLETE (High byte is ID)
 */ 

unsigned long DPCloseSlave(unsigned char ucSlvAddr);

/*************************************************************
 *
 *  unsigned long DPReadSlaveData(unsigned char ucSlvAddr, DP_DATA * pDpData, BOOL bResetDiag);
 *
 *  description:
 *		Read the operation data of the given slave.
 *
 *  parameters:
 *		ucSlvAddr: Slave address (0..126)
 *		pDpData: Buffer for result data.
 *		bResetDiag: If this value is TRUE, bNewDiag will be reset after reading.
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPReadSlaveData(unsigned char ucSlvAddr, DP_DATA * pDpData, BOOL bResetDiag);

/*************************************************************
 *
 *  unsigned long DPWriteSlaveData(unsigned char ucSlvAddr, DP_DATA * pDpData);
 *
 *  description:
 *		Write new output data to the given slave buffer.
 *		Only the output data field of DP_DATA is used.
 *
 *  parameters:
 *		ucSlvAddr: Slave address (0..126)
 *		pDpData: Buffer with output data. If the output length is invalid an error returns.
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPWriteSlaveData(unsigned char ucSlvAddr, DP_DATA * pDpData);

/*************************************************************
 *
 *  unsigned long DPDoSingleComm(void);
 *
 *  description:
 *		Initiate a single communication on the PROFIBUS.
 *		If DPEnableCommThread() is activated this function is disabled.
 *		In this case communication is done by the background thread.
 *
 *  parameters:
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPDoSingleComm(void);

/*************************************************************
 *
 *  unsigned long DPDoCycleComm(void);
 *
 *  description:
 *		Initiate a complete PROFIBUS cycle. Communication is done
 *		with all opened slaves.
 *		If DPEnableCommThread() is activated this function is disabled.
 *		In this case communication is done by the background thread.
 *
 *  parameters:
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPDoCycleComm(void);

#ifdef _WIN32
/*************************************************************
 *
 *  unsigned long DPEnableCommThread(BOOL bEnable);
 *
 *  description:
 *		Enable background communication thread.
 *		If enabled, most functions work acyclic and return DP_INCOMPLETE.
 *		The operation result is reveived with DPGetAsyncResult()
 *
 *  parameters:
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPEnableCommThread(BOOL bEnable);

/*************************************************************
 *
 *  unsigned long DPSetUserMsg(HWND hNotifyWnd, DWORD dwThreadID, UINT uNotifyMsg, unsigned int uMsgMask);
 *
 *  description:
 *		Enable notification messages if DPEnableCommThread()
 *		is activated.
 *
 *  parameters:
 *		hNotifyWnd: Window handle with window to notify. IF this parameter is NULL
 *				dwThreadID will be used insted.
 *		dwThreadID: ID of thread for message totification. The messages will be send with
 *				Post ThreadMessage to this thread.
 *		uNotifyMsg: Nofification message. Best get this value with  RegisterWindowMessage().
 *		uMsgMask: Mask for messages to enable. Possible values can be a combination of
 *			MSG_TYPE_ASYNC		async. operation finished, LPARAM=holds operation ID
 *			MSG_TYPE_DPCYCLE	This message is sent, after completion of one PROFIBUS DP cycle
 *			MSG_TYPE_DPE		message on DPE confirmation or indication
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPSetUserMsg(HWND hNotifyWnd, DWORD dwThreadID, UINT uNotifyMsg, unsigned int uMsgMask);

/*************************************************************
 *
 *  unsigned long DPGetAsyncResult(ASYNC_RESULT *pAsyncResult);
 *
 *  description:
 *		Get the result of an async. backgound operation.
 *		Only possible if DPEnableCommThread() is enabled.
 *
 *  parameters:
 *		pAsyncResult: Buffer for async result values. Look at ASYNC_RESULT for
 *				nearer informations.
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPGetAsyncResult(ASYNC_RESULT *pAsyncResult);

/*************************************************************
 *
 *  unsigned long DPECreateConnection(unsigned long *pC_Ref,
 *	BOOL bClass1, unsigned char ucClass1Addr)
 *
 *
 *  description:
 *		Create a new connection for DPE MSAC2 operation
 *		MSAC1 connections are automatically created during
 *		DPOpenSlave() if DPV1_Supported = TRUE
 *		Only possible if DPEnableCommThread() is enabled.
 *
 *  parameters:
 *		pC_Ref: Buffer for result reference handle
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPECreateConnection(unsigned long *pC_Ref);

/*************************************************************
 *
 *  unsigned long DPEDestroyConnection(unsigned long C_Ref);
 *
 *  description:
 *		Destroy connection created with DPECreateConnection.
 *		Important: If possible, abort the communication over this
 *					C_Ref before closing the connection.
 *		Only possible if DPEnableCommThread() is enabled.
 *
 *  parameters:
 *		pC_Ref: reference handle from DPECreateConnection
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPEDestroyConnection(unsigned long C_Ref);

/*************************************************************
 *
 *  unsigned long DPESendRequest(DPE_REQUEST_INFO *pRequestInfo);
 *
 *  description:
 *		Send a request to the dpe staemachine. For request types
 *		look in pde_def.h
 *		Only possible if DPEnableCommThread() is enabled.
 *
 *  parameters:
 *		pRequestInfo: information about DPE request
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPESendRequest(DPE_REQUEST_INFO *pRequestInfo);

/*************************************************************
 *
 *  unsigned long DPEReadResult(DPE_RESULT_INFO *pResultInfo);
 *
 *  description:
 *		Get Result from the dpe statemachine. This can be either a
 *		confirmation to a request or a spontaneous indication.
 *		Call this function after receiveing a MSG_TYPE_DPE message.
 *		Only possible if DPEnableCommThread() is enabled.
 *
 *  parameters:
 *		pResultInfo: information about DPE result
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPEReadResult(DPE_RESULT_INFO *pResultInfo);

/*************************************************************
 *
 *  unsigned long DPSetFDLReceiveTimeout(unsigned int uTimeout);
 *
 *  description:
 *		Set Timeout for FDL communication, default is 1000 ms
 *
 *  parameters:
 *		uTimeout: FDL timeout [ms]
 *
 *  returns:
 *		_DP_RESULT error code
 */ 

unsigned long DPSetFDLReceiveTimeout(unsigned int uTimeout);

/*************************************************************
 *
 *  unsigned long BOOL EXPO2 WhichConvFeature(unsigned char feature, const char *com);
 *
 *  description:
 *		called with feature:
 *			0x01	:= expensive
 *			0x02	:= DPV1
 *			0x04	:= master addressable
 *
 *		com: com port to use
 *		
 *	returns: TRUE  for detected	
 *	 		 FALSE for not detected	
 */ 

unsigned long WhichConvFeature(unsigned char feature, const char *com);
#endif

#if defined __cplusplus
}	// extern "C"
#endif

#endif /*_PROF_DP_H */

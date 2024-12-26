
/*********************************************************************
 *                                                                   *
 *                       Bihl+Wiedemann GmbH                         *
 *                                                                   *
 *                                                                   *
 *       project: Profibus Master Simulator                          *
 *   module name: dep_def.h                                          *
 *        author: Ulrich Holeschak                                   *
 *          date: 06/11/2000                                         *
 *                                                                   *
 *      RCS info:                                                    *
 *         $Date: 2006/01/30 15:44:51 $
 *       $Locker:  $
 *     $Revision: 1.8 $
 *        $State: Exp $
 *                                                                   *
 *       history: 1.0 new                                            *
 *                                                                   *
 *   description: PROFIBUS DPE definitions                           *
 *                                                                   *
 *********************************************************************/

/*********************************************************************
	Defines
 *********************************************************************/

#ifndef __DPE_DEF_H__
#define __DPE_DEF_H__

/*******************************************************/
// common
/*******************************************************/

#define	DPE_CREF_CLASS2		0x10000ul	/* identifiction for class 2 CREF */

// codes for reject reasons
typedef enum
{
	REJ_LE = 0,			// max_data_len_unit overflow
	REJ_PS,				// number of parallel service requests exceeded
	REJ_SE,				// MSAC1 service reject
	REJ_ABORT,			// MSAC1 abort reject
	REJ_IV,				// MSAC1 invalid service reject
} REJECT_CODES;

// codes for fault reasons (MSAC1)
typedef enum
{
	DDLM_FDL_FAULT = 0,	// FDL fault
	MSAC1_FAULT,		// MSAC1 processing fault
} FAULT_CODES;

// definition of the
// additional address parameter structure
typedef struct
{
	unsigned char	ucSType;				// 1=presence of Network/MAC address in source
	unsigned char	ucSLen;					// length of S_Addr subparameter
	unsigned char	ucDType;				// 1=presence of Network/MAC address in destination
	unsigned char	ucDLen;					// length of D_Addr subparameter
	unsigned char	AddrData [255];			// S_Addr and D_Addr subparameter
} DPE_ADD_ADDR_PARAM;

/*******************************************************/
// for request (DPESendRequest)
/*******************************************************/

// types for master class 2 requests
typedef enum
{
	MSACX_REQ_IDLE,							// no request
	MSAC2_REQ_RESET,						// MSAC2 reset request
	MSAC2_REQ_INIT,							// MSAC2 initiate request
	MSAC2_REQ_ABORT,						// MSAC2 abort request
	MSACX_REQ_READ,							// MSAC1/2 read request
	MSACX_REQ_WRITE,						// MSAC1/2 write request
	MSAC2_REQ_DTRANS,						// MSAC2 data transport request
	MSAC1_REQ_ALARM_ACK,					// MSAC1 alarm ack request
	MSAC1_REQ_START,						// MSAC1 start request (for internal use only)
	MSAC1_REQ_STOP,							// MSAC1 stop request (for internal use only)
} MSACX_REQ;

// data structure for init request
typedef struct
{
	unsigned char		ucRemAdd;						// remote address
	unsigned int		uSendTimeout;					// send timeout [10ms]
	unsigned char		ucFeaturesSupported1;			// features supported 1
	unsigned char		ucFeaturesSupported2;			// features supported 2
	unsigned char		ucProfileFeaturesSupported1;	// profile features supported 1
	unsigned char		ucProfileFeaturesSupported2;	// profile features supported 2
	unsigned int		uProfileIdentNumber;			// profile ident number
	DPE_ADD_ADDR_PARAM	AddAddrParam;					// add addr param
} DPE_REQ_INIT;

// data structure for read request
typedef struct
{
	unsigned char		ucSlotNum;			// slot number
	unsigned char		ucIndex;			// index of data block
	unsigned char		ucLength;			// read length
} DPE_REQ_READ;

// data structure for write request
typedef struct
{
	unsigned char		ucSlotNum;			// slot number
	unsigned char		ucIndex;			// index of data block
	unsigned char		ucLength;			// write length
	unsigned char		PduData [255];		// write data
} DPE_REQ_WRITE;

// data structure for data transport request
typedef struct
{
	unsigned char		ucSlotNum;			// slot number
	unsigned char		ucIndex;			// index of data block
	unsigned char		ucLength;			// write and read length
	unsigned char		PduData [255];		// write data
} DPE_REQ_TRANSPORT;

// data structure for alaram request
typedef struct
{
	unsigned char		ucSlotNum;			// slot number
	unsigned char		ucAlarmType;		// alarm type
	unsigned char		ucSeqNr;			// sequence number
} DPE_REQ_ALARM_ACK;

// data structure for abort request
typedef struct
{
	unsigned char		ucSubnet;			// subnet
	unsigned char		ucReason;			// reason code
} DPE_REQ_ABORT;

// MSAC1 data structure for start request
typedef struct
{											// remote address is identical with cref
	unsigned char		ucMaxChannelDataLength;		// max MSAC1 PDU length of slave (4..244)
	unsigned char		bExtraAlarmSAP;		// extra alarm SAP
	unsigned int		uResponseTimeout;	// response timeout [10ms]
} DPE_REQ_START;

// unition containing data part of requests
typedef union
{
	DPE_REQ_INIT		MASC2Init;			// data for init reqest (MSAC2_REQ_INIT)
	DPE_REQ_READ		MASCXRead;			// data for read request (MSACX_REQ_READ)
	DPE_REQ_WRITE		MASCXWrite;			// data for write request (MSACX_REQ_WRITE)
	DPE_REQ_TRANSPORT	MASC2DTrans;		// data for data transport request (MSAC2_REQ_DTRANS)
	DPE_REQ_ALARM_ACK	MASC1AlarmAck;		// data for alarm ack request (MSAC1_REQ_ALARM_ACK)
	DPE_REQ_ABORT		MASC2Abort;			// data for abort request (MSAC2_REQ_ABORT)
	DPE_REQ_START		MASC1Start;			// data for start request (MSAC1_REQ_START) (for internal use only)
} DPE_REQUEST_DATA;

// definition of the request data
// in DPESendRequest
typedef struct
{
	unsigned long		C_Ref;				// reference
	MSACX_REQ			MSACXReq;			// current MSAC2 request
	DPE_REQUEST_DATA	RequestData;		// data of request
} DPE_REQUEST_INFO;

/*******************************************************/
// for result (DPEReadResult)
/*******************************************************/

// types of dpe results codes
typedef enum
{
	MSAC2_CONF_INIT_POS,					// MSAC2 initiate confirmation positive
	MSAC2_CONF_INIT_NEG,					// MSAC2 initiate confirmation negative
	MSACX_CONF_READ_POS,					// MSACX read confirmation positive
	MSACX_CONF_READ_NEG,					// MSACX read confirmation negative
	MSACX_CONF_WRITE_POS,					// MSACX write confirmation positive
	MSACX_CONF_WRITE_NEG,					// MSACX write confirmation negative
	MSAC2_CONF_DTRANS_POS,					// MSAC2 data trans confirmation positive
	MSAC2_CONF_DTRANS_NEG,					// MSAC2 data trans confirmation negative
	MSAC2_CONF_RESET,						// MSAC2 reset confirmation
	MSAC2_INDI_ABORT,						// MSAC2 abort indication
	MSAC2_INDI_CLOSED,						// MSAC2 closed indication
	MSAC2_INDI_REJECT,						// MSAC2 reject indication
	MSAC2_INDI_FAULT,						// MSAC2 fault indication
	MSAC1_CONF_START,						// MSAC1 start confirmation (for internal use only)
	MSAC1_CONF_STOP,						// MSAC1 stop confirmation (for internal use only)
	MSAC1_CONF_ALARM_ACK_POS,				// MSAC1 alarm ack confirmation positive
	MSAC1_CONF_ALARM_ACK_NEG,				// MSAC1 alarm ack confirmation negative
	MSAC1_INDI_STARTED,						// MSAC1 started indication
	MSAC1_INDI_STOPPED,						// MSAC1 stopped indication
	MSAC1_INDI_ABORT,						// MSAC1 abort indication (for internal use only)
	MSAC1_INDI_REJECT,						// MSAC1 reject indication
	MSAC1_INDI_FAULT,						// MSAC1 fault indication
} DPE_RESULT_TYPES;

// data for negative confirmation
// (CONF_READ_NEG, CONF_WRITE_NEG, CONF_DTRANS_NEG)
typedef struct
{
	unsigned long		C_Ref;				// communication reference
	unsigned char		ucErrorDecode;		// error decode
	unsigned char		ucErrorCode1;		// error code 1
	unsigned char		ucErrorCode2;		// error code 2
} DPE_CONF_NEG_DATA;

// data for simple confirmation/indication
// containing no further data
typedef struct
{
	unsigned long		C_Ref;				// communication reference
} DPE_CONF_INDI_SIMPLE;

// data for init positive confirmation
// (CONF_INIT_POS)
typedef struct
{
	unsigned long		C_Ref;						// communication reference
	unsigned char		ucFeaturesSupported1;		// features supported 1
	unsigned char		ucFeaturesSupported2;		// features supported 1
	unsigned char		ucProfileFeaturesSupported1;// profile features supported 1
	unsigned char		ucProfileFeaturesSupported2;// profile features supported 2
	unsigned int		uProfileIdentNumber;		// profile ident number
	DPE_ADD_ADDR_PARAM	AddAddrParam;				// additional address parameter
	unsigned char		ucMaxLenDataUnit;			// max len data unit
} DPE_CONF_INIT_POS_DATA;

// data for data positive confirmation
// (CONF_READ_POS, CONF_DTRANS_POS)
typedef struct
{
	unsigned long		C_Ref;				// communication reference
	unsigned char		Length;				// data length
	unsigned char		Data[255];			// data
} DPE_CONF_DATA_POS_DATA;

// data for write positive confirmation
// (CONF_WRITE_POS)
typedef struct
{
	unsigned long		C_Ref;				// communication reference
	unsigned char		Length;				// data length
} DPE_CONF_WRITE_POS_DATA;

// data for alarm ack confirmation
// (MSAC1_CONF_ALARM_ACK_POS, MSAC1_CONF_ALARM_ACK_NEG)
typedef struct
{
	unsigned long		C_Ref;				// communication reference
	unsigned char		ucSlotNumber;		// slot number
	unsigned char		ucAlarmType;		// alarm type
	unsigned char		ucSeqNumber;		// sequence number
} DPE_CONF_ALARM_ACK_DATA;

// data for abort indication
// (INDI_ABORT)
typedef struct
{
	unsigned long		C_Ref;				// communication reference
    bool				bLocallyGenerated;	// 1=locally generated
	unsigned char		ucSubnet;			// subnet
	unsigned char		ucReasonCode;		// reason code
	unsigned int		uAdditionalDetail;	// additional detail
} DPE_INDI_ABORT_DATA;

// data for reject indication
// (INDI_REJECT)
typedef struct
{
	unsigned long		C_Ref;				// communication reference
	unsigned char		ucReasonCode;		// reason code
} DPE_INDI_REJECT_DATA;

// MSAC1 data for reject and fault indication
// (MSAC1_INDI_REJECT, MSAC1_INDI_FAULT)
typedef struct
{
	unsigned long		C_Ref;				// communication reference
	unsigned char		ucStatus;			// status
} DPE_INDI_REJECT_FAULT_DATA;

// union containing all data
// for indications and confirmations
typedef union
{
	DPE_CONF_INIT_POS_DATA			MSAC2ConfInitPos;		// MSAC2 initiate confirmation positive
	DPE_CONF_NEG_DATA				MSAC2ConfInitNeg;		// MSAC2 initiate confirmation negative
	DPE_CONF_DATA_POS_DATA			MSACXConfReadPos;		// MSACX read confirmation positive
	DPE_CONF_NEG_DATA				MSACXConfReadNeg;		// MSACX read confirmation negative
	DPE_CONF_WRITE_POS_DATA			MSACXConfWritePos;		// MSACX write confirmation positive
	DPE_CONF_NEG_DATA				MSACXConfWriteNeg;		// MSACX write confirmation negative
	DPE_CONF_DATA_POS_DATA			MSAC2ConfDTransPos;		// MSAC2 data trans confirmation positive
	DPE_CONF_NEG_DATA				MSAC2ConfDTransNeg;		// MSAC2 data trans confirmation negative
	DPE_CONF_INDI_SIMPLE			MSAC2ConfReset;			// MSAC2 reset confirmation
	DPE_INDI_ABORT_DATA				MSAC2IndiAbort;			// MSAC2 abort indication
	DPE_CONF_INDI_SIMPLE			MSAC2IndiClosed;		// MSAC2 closed indication
	DPE_INDI_REJECT_DATA			MSAC2IndiReject;		// MSAC2 reject indication
	DPE_CONF_INDI_SIMPLE			MSAC2IndiFault;			// MSAC2 fault indication	
	DPE_CONF_INDI_SIMPLE			MSAC1ConfStart;			// MSAC1 start confirmation (for internal use only)
	DPE_CONF_INDI_SIMPLE			MSAC1ConfStop;			// MSAC1 stop confirmation (for internal use only)
	DPE_CONF_ALARM_ACK_DATA			MSAC1ConfAlarmAckPos;	// MSAC1 alarm ack confirmation positive
	DPE_CONF_ALARM_ACK_DATA			MSAC1ConfAlarmAckNeg;	// MSAC1 alarm ack confirmation negative
	DPE_CONF_INDI_SIMPLE			MSAC1IndiStarted;		// MSAC1 started indication
	DPE_CONF_INDI_SIMPLE			MSAC1IndiStopped;		// MSAC1 stopped indication
	DPE_CONF_INDI_SIMPLE			MSAC1IndiAbort;			// MSAC1 abort indication (for internal use only)
	DPE_INDI_REJECT_FAULT_DATA		MSAC1IndiReject;		// MSAC1 reject indication
	DPE_INDI_REJECT_FAULT_DATA		MSAC1IndiFault;			// MSAC1 fault indication
} DPE_CONF_INDI_DATA;

// definition of the result data
// in DPEReadResult
typedef struct
{
	DPE_RESULT_TYPES		Type;				// type of confirmation/indication
	DPE_CONF_INDI_DATA		Info;				// info field for confirmation/indication
} DPE_RESULT_INFO;

#endif		// __DPE_DEF_H__

#ifndef ALFSBL_SEC_H_
#define ALFSBL_SEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#define CSUDMA_BLOCK_LENGTH_MAX   (512UL)


#define PPK_BYTE_LENGTH  (64U)
#define SPK_BYTE_LENGTH  (64U)

#define HASH_BYTE_LENGTH (32U)

#define CSU_MSG_RAM_BASEADDR             (0XF8080000UL)

#define EFUSE_SEC_CTRL                   (SYSCTRL_S_EFUSE_GLB18)
#define EFUSE_AUTH_TYPE_OFFSET           (8U)
#define EFUSE_AUTH_TYPE_MASK             (0x03U << EFUSE_AUTH_TYPE_OFFSET)
#define EFUSE_AUTH_TYPE_HEADER_SET       (0x00U << EFUSE_AUTH_TYPE_OFFSET)
#define EFUSE_AUTH_TYPE_SM2              (0x01U << EFUSE_AUTH_TYPE_OFFSET)
#define EFUSE_AUTH_TYPE_ECC256           (0x02U << EFUSE_AUTH_TYPE_OFFSET)

#define EFUSE_ENC_TYPE_OFFSET            (10U)
#define EFUSE_ENC_TYPE_MASK              (0x03U << EFUSE_ENC_TYPE_OFFSET)
#define EFUSE_ENC_TYPE_HEADER_SET        (0x00U << EFUSE_ENC_TYPE_OFFSET)
#define EFUSE_ENC_TYPE_SM4               (0x01U << EFUSE_ENC_TYPE_OFFSET)
#define EFUSE_ENC_TYPE_AES256            (0x02U << EFUSE_ENC_TYPE_OFFSET)

#define EFUSE_PPK_HASH_TYPE_OFFSET       (12U)
#define EFUSE_PPK_HASH_TYPE_MASK         (0x03U << EFUSE_PPK_HASH_TYPE_OFFSET)
#define EFUSE_PPK_HASH_TYPE_HEADER_SET   (0x00U << EFUSE_PPK_HASH_TYPE_OFFSET)
#define EFUSE_PPK_HASH_TYPE_SM3          (0x01U << EFUSE_PPK_HASH_TYPE_OFFSET)
#define EFUSE_PPK_HASH_TYPE_SHA256       (0x02U << EFUSE_PPK_HASH_TYPE_OFFSET)

#define EFUSE_ENC_KEY0_INVALID_OFFSET    (15U)
#define EFUSE_ENC_KEY0_INVALID_MASK      (0x01U << EFUSE_ENC_KEY0_INVALID_OFFSET)

#define EFUSE_ENC_KEY1_INVALID_OFFSET    (17U)
#define EFUSE_ENC_KEY1_INVALID_MASK      (0x01U << EFUSE_ENC_KEY1_INVALID_OFFSET)

#define EFUSE_ENC_KEY_SOURCE_OFFSET      (18U)
#define EFUSE_ENC_KEY_SOURCE_MASK        (0x01U << EFUSE_ENC_KEY_SOURCE_OFFSET)

#define EFUSE_PPK0_INVALID_OFFSET        (21U)
#define EFUSE_PPK0_INVALID_MASK          (0x01U << EFUSE_PPK0_INVALID_OFFSET)

#define EFUSE_PPK1_INVALID_OFFSET        (23U)
#define EFUSE_PPK1_INVALID_MASK          (0x01U << EFUSE_PPK1_INVALID_OFFSET)

#define PPK_HASH_TYPE_NONE               (0x00U)
#define PPK_HASH_TYPE_SM3                (0x01U)
#define PPK_HASH_TYPE_SHA256             (0x02U)

/// bit[0]:     0--ECB, 1--CBC
#define SYM_ECB (0x00)
#define SYM_CBC (0x01)

/// bit[3]:     0--enc, 1--dec
#define SYM_ENCRYPT  (0x00)
#define SYM_DECRYPT  (0x08)

/// bit[6:4]:   001--128, 010--192, 100--256
#define SYM_128BIT  (0x10)
#define SYM_192BIT  (0x20)
#define SYM_256BIT  (0x40)

#define AES_BLOCK_BYTE 16  /// 128 bit
#define SM4_BLOCK_BYTE 16  /// 128 bit

/// msgram command type definition
#define CMD_AUTH    (0xCD01)
#define CMD_DMA     (0xCD02)
#define CMD_ENCRYPT (0xCD03)
#define CMD_HASH    (0xCD04)
#define CMD_SIGN    (0xCD05)
#define CMD_GENKEY  (0xCD06)
#define CMD_GETZ    (0xCD07)

#define CMD_ACK     (0xAC01)

/// msgram command options definition
#define OP_AUTH_NONE       (0x00)
#define OP_AUTH_ECC256     (0x61)
#define OP_AUTH_SM2        (0x62)

#define OP_ENCRYPT_AES256  (0x63)
#define OP_ENCRYPT_SM4     (0x64)
#define OP_ENCRYPT_NONE    (0x65)

#define OP_HASH_SHA256     (0b10)
#define OP_HASH_SM3        (0b11)
#define OP_HASH_NONE       (0b00)

#define OP_AUTH_PASS       (0x69)
#define OP_AUTH_FAIL       (0x6A)

#define OP_DMA_DONE        (0x6B)
#define OP_ENCRYPT_DONE    (0x6C)
#define OP_HASH_DONE       (0x6D)
#define OP_SIGN_DONE       (0x6E)
#define OP_GENKEY_DONE     (0x6F)
#define OP_GETZ_DONE       (0x71)

#define OP_BHDR_KEY        (0x6E)
#define OP_USER_KEY        (0x6F)

#define OP_CMD_ERROR       (0xE1)
#define OP_CSU_ERROR       (0xE2)
#define OP_CSU_TIMEOUT     (0xE3)


/// RETURN VALUE for encrytion/decryption
#define CSU_SECURE_SUCCESS                0x00
#define CSU_SECURE_FAIL                   0x01
#define CSU_SECURE_UNKNOWN_ERROR          0x02
#define CSU_SECURE_NOT_SUPPORT_YET_ERROR  0x03
#define CSU_SECURE_NOT_INITIALIZE_ERROR   0x04
#define CSU_SECURE_OBJECT_ERROR           0x05
#define CSU_SECURE_MEMORY_ERROR           0x06
#define CSU_SECURE_IN_DATA_LENGTH_ERROR   0x07
#define CSU_SECURE_IN_DATA_ERROR          0x08
#define CSU_SECURE_HASH_OBJECT_ERROR      0x09
#define CSU_SECURE_HASH_ERROR             0x0A
#define CSU_SECURE_HASH_NOT_EQUAL_ERROR   0x0B
#define CSU_SECURE_OUT_OF_RANGE_ERROR     0x0C
#define CSU_SECURE_PARAMETER_ERROR        0x0D

/**
 * System Interrupt ID
 */
#define APU2CSU_REQ_IRQN (30)
#define APU2CSU_ACK_IRQN (31)
#define CSU2APU_REQ_IRQN (32)
#define CSU2APU_ACK_IRQN (33)
#define RPU2CSU_REQ_IRQN (34)
#define RPU2CSU_ACK_IRQN (35)
#define CSU2RPU_REQ_IRQN (36)
#define CSU2RPU_ACK_IRQN (37)

/**
 * System IPC registers
 */
#define IPC_BASEADDR (0xF841F000)
#define APU2CSU_REQ_ADDR (IPC_BASEADDR + 0x400)
#define APU2CSU_ACK_ADDR (IPC_BASEADDR + 0x404)
#define CSU2APU_REQ_ADDR (IPC_BASEADDR + 0x408)
#define CSU2APU_ACK_ADDR (IPC_BASEADDR + 0x40C)
#define RPU2CSU_REQ_ADDR (IPC_BASEADDR + 0x410)
#define RPU2CSU_ACK_ADDR (IPC_BASEADDR + 0x414)
#define CSU2RPU_REQ_ADDR (IPC_BASEADDR + 0x418)
#define CSU2RPU_ACK_ADDR (IPC_BASEADDR + 0x41C)


#define HASH_TYPE_SHA256   (0x00U)
#define HASH_TYPE_SM3      (0x01U)

#define AUTH_TYPE_ECC256   (0x00U)
#define AUTH_TYPE_SM2      (0x01U)

#define CSUDMA_DST_INCR     (0x0 << 1)
#define CSUDMA_DST_NOINCR   (0x1 << 1)
#define CSUDMA_SRC_INCR     (0x0 << 0)
#define CSUDMA_SRC_NOINCR   (0x1 << 0)

#define FIRST_BLOCK         (0x1 << 2)
#define LAST_BLOCK          (0x2 << 2)
#define MIDDLE_BLOCK        (0x3 << 2)
#define WHOLE_BLOCK         (0x0 << 2)

#define BLOCK_LENGTH        (0x2048)

/// Error Code
#define SEC_ERROR_CHECKSUM_ERROR          (0x60U)
#define SEC_ERROR_SEC_PARAM_INVALID       (0x65U)
#define SEC_ERROR_INVALID_CSU_ACK         (0x66U)
#define SEC_ERROR_HASH_FAIL               (0x67U)
#define SEC_ERROR_AUTH_FAIL               (0x68U)
#define SEC_ERROR_NO_VALID_PPK            (0x69U)


typedef struct _SecureInfo_ {
	uint8_t  AuthType;       /// OP_AUTH_ECC256 / OP_AUTH_SM2 / OP_AUTH_NONE
	uint8_t  HashType;       /// OP_HASH_SHA256 / OP_HASH_SM3 / OP_HASH_NONE
	uint8_t  EncType;        /// OP_ENCRYPT_AES256 / OP_ENCRYPT_SM4 / OP_ENCRYPT_NONE
	uint8_t  EncMode;        /// SYM_ECB / SYM_CBC, only ecb supported now
	uint8_t  EncDir;         /// SYM_ENCRYPT / SYM_DECRYPT
	uint8_t  KeyMode;        /// OP_BHDR_KEY / OP_USER_KEY, bootheader key or user key
	uint8_t  CsuAddrIncMode; /// Bit1: Destination addr increment mode, Bit0: Source addr increment mode
	uint8_t  BlockMode;      /// Bit2: mark firt block, Bit3: mark last block, all zero: one whole block, all one: mark middle block
	union {
		uint32_t InputAddr;
		uint32_t IdaAddr;
	};
	union {
		uint32_t DataLength;
		uint32_t IdaLength;
	};
	union {
		uint32_t OutputAddr;
		uint32_t ZaOutAddr;
	};
	uint32_t HashDataAddr;
	//uint32_t DataLength;
	uint32_t KeyAddr;
	uint32_t IvAddr;
	uint32_t SignatureAddr;
	uint32_t PubKeyAddr;
	uint32_t PriKeyAddr;
	union {
		uint32_t HashOutAddr;
		uint32_t DigestAddr;
	};
} SecureInfo;



typedef struct _SecEncHashIODef_ {
	uint32_t  KeyAddr;
	uint32_t  IvAddr;
	uint32_t  InputAddr;
	uint32_t  OutputAddr;
	uint32_t  TotalLength;
	uint8_t   KeyMode;    /// bootheader key or user key
	uint8_t   EncMethod;  /// OP_ENCRYPT_AES256 / OP_ENCRYPT_SM4 / OP_ENCRYPT_NONE
	uint8_t   EncMode;    /// ecb or cbc, only ecb supported for now
	uint8_t   HashEnable; /// hash enable or not
	uint32_t  HashOutAddr;/// buffer for hash, if hash enabled
} SecEncHashIODef;


typedef union _OpModeDef_ {
	struct {
		uint32_t EcbCbc    :  1;
		uint32_t Reserved0 :  2;
		uint32_t EncDec    :  1;
		uint32_t AesLen    :  3;
		uint32_t Reserved1 : 25;
	};
	uint32_t Bits;
} OpModeDef;


typedef struct _DmaParamDef_ {
	OpModeDef OpMode;
	uint32_t  KeyAddr;
	uint32_t  IvAddr;
	uint32_t  InputAddr;
	uint32_t  TotalLength;
	uint32_t  OutputAddr;
	uint32_t  HashOutAddr;
} DmaParamDef;


typedef struct _AuthParamDef_ {
	uint32_t PubkeyAddr;
	uint32_t DigestAddr;
	uint32_t SignatureAddr;
} AuthParamDef;


typedef struct _HashParamDef_ {
	uint32_t DataInAddr;
	uint32_t DataLen;
	uint32_t HashOutAddr;
} HashParamDef;


typedef struct _MsgOptDef_ {
	uint8_t Low;
	uint8_t High;
} MsgOptDef;


typedef struct _SignatureParamDef_ {
	uint32_t PriKeyAddr;
	uint32_t DigestAddr;
	uint32_t SignatureAddr;
} SignatureParamDef;


typedef struct _KeyPairGenParamDef_ {
	uint32_t PubKeyAddr;
	uint32_t PriKeyAddr;
} KeyPairGenParamDef;


//typedef struct _Sm2GetZaParamDef_ {
//	uint32_t PubKeyAddr;
//	uint32_t IdaAddr;
//	uint32_t IdaLength;
//	uint32_t ZaOutAddr;
//} Sm2GetZaParamDef;


typedef struct _SecMsgDef_ {
	uint32_t Cmd;
	MsgOptDef Option0;
	MsgOptDef Option1;
	union {
		AuthParamDef       AuthParam;
		HashParamDef       HashParam;
		DmaParamDef        DmaParam;
		SignatureParamDef  SignatureParam;
		KeyPairGenParamDef KeyPairGenParam;
//		Sm2GetZaParamDef   Sm2GetZaParam;
		uint32_t           Data[7];
	};
} SecMsgDef;

typedef union _PubKey_ {
	uint32_t All[16];
	struct {
		uint32_t X[8];
		uint32_t Y[8];
	} Value;
} PubKey;

typedef struct _PriKey_ {
	uint32_t Value[8];
} PriKey;

typedef union _Signature_ {
	uint32_t All[16];
	struct {
		uint32_t R[8];
		uint32_t S[8];
	} Value;
} Signature;



typedef struct _AckDef_ {
	uint32_t  Cmd;
	MsgOptDef Option0;
} AckDef;

uint32_t AlFsbl_CalcCrc32(uint8_t *pBuffer, uint32_t Length);
uint32_t AlFsbl_ChecksumCheck(uint8_t *pBuffer, uint32_t Length, uint32_t Checksum);

uint32_t SecureIrqInit(void);
void     TriggerSecInterrupt(void);
void     MsgReceive(void);

uint32_t AlFsbl_CompareHash(uint8_t *pHash1, uint8_t *pHash2, uint32_t HashLen);


uint32_t AlFsbl_DecHash(SecureInfo *pSecureInfo);
uint32_t AlFsbl_Hash(SecureInfo *pSecureInfo);
uint32_t AlFsbl_Auth(SecureInfo *pSecureInfo);

uint32_t AlFsbl_CsuDmaCopy(uint32_t SrcAddr, uint32_t DestAddr, uint32_t DataByteLen, uint8_t AddrMode);

uint32_t AlFsbl_Signature(uint8_t AuthType, uint32_t PriKeyAddr, uint32_t DigestAddr, uint32_t SigAddr);
uint32_t AlFsbl_KeyPairGen(uint8_t AuthType, uint32_t PubKeyAddr, uint32_t PriKeyAddr);
//uint32_t AlFsbl_Sm2GetZa(uint32_t PubKeyAddr, uint32_t IdaAddr, uint32_t IdaLength, uint32_t ZaOutAddr);



//uint32_t cal_crc32(uint8_t *buf, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* ALFSBL_SEC_H_ */



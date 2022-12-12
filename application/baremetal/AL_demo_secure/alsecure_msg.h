/*
 * alsecure_msg_send.h
 *
 *  Created on: 2022年6月22日
 *      Author: chao.zhang
 */

#ifndef _ALSECURE_MSG_H_
#define _ALSECURE_MSG_H_

#include "al_types.h"
#include "alsecure_misc.h"

// para bit define
// mode bit0--0:ECB  1:CBC
#define SYM_ECB 0x00
#define SYM_CBC 0x01
// en_de bit3--0：en 1：de
#define SYM_ENCRYPT 0x00
#define SYM_DECRYPT 0x08
// bit len bit4~bit6--001:128  010:192  100:256
#define SYM_AES128 0x10
#define SYM_AES192 0x20
#define SYM_AES256 0x40

#define AES_BLOCK_BYTE 16 // 128bit
#define SM4_BLOCK_BYTE 16 // 128bit

//Is hash or not
#define OP_HASH_ON  0x1
#define OP_HASH_OFF 0x0

/* MSG RAM command type definition */
#define CMD_AUTH 0xCD01
#define CMD_DMA 0xCD02
#define CMD_ENCRYPT 0xCD03
#define CMD_HASH 0xCD04
#define CMD_ACK 0xAC01

/* MSG RAM auth command options definition */
#define OP_AUTH_ECC256 0x61
#define OP_AUTH_SM2 0x62
#define OP_ENCRYPT_AES256 0x63
#define OP_ENCRYPT_SM4 0x64
#define OP_ENCRYPT_NONE 0x65
#define OP_HASH_SHA256 (0b10)
#define OP_HASH_SM3 (0b11)
#define OP_HASH_NONE (0b00)
#define OP_AUTH_PASS 0x69
#define OP_AUTH_FAIL 0x6A
#define OP_DMA_DONE 0x6B
#define OP_ENCRYPT_DONE 0x6C
#define OP_HASH_DONE 0x6D
#define OP_CMD_ERROR 0xE1
#define OP_CSU_ERROR 0xE2
#define OP_CSU_TIMEOUT 0xE3
#define OP_BHDR_KEY 0x6E
#define OP_USER_KEY 0x6F

/* RETURN VALUE for encrytion/decryption */
#define AL_SECURE_SUCCESS 0x00
#define AL_SECURE_FAIL 0x01
#define AL_SECURE_UNKNOWN_ERROR 0x02
#define AL_SECURE_NOT_SUPPORT_YET_ERROR 0x03
#define AL_SECURE_NOT_INITIALIZE_ERROR 0x04
#define AL_SECURE_OBJECT_ERROR 0x05
#define AL_SECURE_MEMORY_ERROR 0x06
#define AL_SECURE_IN_DATA_LENGTH_ERROR 0x07
#define AL_SECURE_IN_DATA_ERROR 0x08
#define AL_SECURE_HASH_OBJECT_ERROR 0x09
#define AL_SECURE_HASH_ERROR 0x0A
#define AL_SECURE_HASH_NOT_EQUAL_ERROR 0x0B
#define AL_SECURE_OUT_OF_RANGE_ERROR 0x0C
#define AL_SECURE_PARAMETER_ERROR 0x0D
#define AL_SECURE_ERROR_ENCRYPT_METHOD  0x0E
#define AL_SECURE_ERROR_ENCRYPT_MODE  0x0F
#define AL_SECURE_ERROR_HASH_MODE 0x10
#define AL_SECURE_ERROR_KEY_MODE  0x11
#define AL_SECURE_ERROR_IV_VALUE  0x12
#define AL_SECURE_ERROR_KEY_VALUE  0x13
#define AL_SECURE_ERROR_ENCDEC_VALUE  0x14
#define AL_SECURE_ERROR_DMA_INC_MODE  0x15
#define AL_SECURE_ERROR_DATA_ALIGN  0x16
#define AL_SECURE_ERROR_BLOCK_MODE 0x17

/* DMA source/destination address increase mode */
#define OP_NONE_INCR (0b11)
#define OP_BOTH_INCR (0b00)
#define OP_SRC_INCR (0b10)
#define OP_DST_INCR (0b01)

/* DMA mode: first/middle/last/whole block */
#define OP_WHOLE_BLOCK (0b00)
#define OP_FIRST_BLOCK (0b01)
#define OP_MID_BLOCK (0b11)
#define OP_LAST_BLOCK (0b10)

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


/* Data struct for Cipher parameters */
typedef union OpMode {
  struct {
    u32 EcbCbc : 1;
    u32 Reserved0 : 2;
    u32 EncDec : 1;
    u32 AesLen : 3;
    u32 Reserved1 : 25;
  };
  u32 All;
} OpMode_t;

typedef struct CipherParam {
  OpMode_t OpMode;
  u32 pbKey;
  u32 pbIV;
  u32 pbInput;
  u32 TotalLength;
  u32 pbOutput;
} CipherParam_t;

typedef struct {
  u32 Pubkey;
  u32 Digest;
  u32 Signature;
} AuthParam_t;

typedef struct {
  CipherParam_t CipherParam;
  u32 HashOut;
} DmaParam_t;

typedef struct {
  u32 DataIn;
  u32 DataLength;
  u32 HashOut;
} HashParam_t;

/* MSG command data type  */
typedef struct CsuMsgOpt {
  u8 Low;
  u8 High : 2;
  u8 Extend : 2;
  u8 Resved : 4;
} MsgOpt_t;

typedef struct Msg {
  u32 Cmd;
  MsgOpt_t Option0;
  MsgOpt_t Option1;
  union {
    AuthParam_t AuthParam;
    DmaParam_t DmaParam;
    HashParam_t HashParam;
    u32 Data[7];
  } MsgData;
} Msg_t;

typedef struct Ack {
  u32 Cmd;
  MsgOpt_t Option0;
} Ack_t;

extern volatile u32 MsgFlag;

u32 Msg_Send(Msg_t *Msg);
u32 Msg_SetupIrq();
u32 Msg_CheckAckValid(Ack_t *Ack);

#endif /* _ALSECURE_MSG_H_ */

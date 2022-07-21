/*****************************************************************************/
/**
*
* @file xil_assert.h
*
* @addtogroup common_assert_apis Assert APIs and Macros
*
* The xil_assert.h file contains assert related functions and macros.
* Assert APIs/Macros specifies that a application program satisfies certain
* conditions at particular points in its execution. These function can be
* used by application programs to ensure that, application code is satisfying
* certain conditions.
*
******************************************************************************/
#ifndef XIL_ASSERT_H
#define XIL_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xdmaps_hw.h"

#define XIL_ASSERT_NONE     0U
#define XIL_ASSERT_OCCURRED 1U
#define XNULL NULL

extern u32 Xil_AssertStatus;
extern s32 Xil_AssertWait;
extern void Xil_Assert(const char8 *File, s32 Line);

void XNullHandler(void *NullParameter);

/**
 * This data type defines a callback to be invoked when an
 * assert occurs. The callback is invoked only when asserts are enabled
 */
typedef void (*Xil_AssertCallback) (const char8 *File, s32 Line);
static Xil_AssertCallback Xil_AssertCallbackRoutine = NULL;

/***************** Macros (Inline Functions) Definitions *********************/

#ifndef NDEBUG

/*****************************************************************************/
/**
* @brief    This assert macro is to be used for void functions. This in
*           conjunction with the Xil_AssertWait boolean can be used to
*           accommodate tests so that asserts which fail allow execution to
*           continue.
*
* @param    Expression: expression to be evaluated. If it evaluates to
*           false, the assert occurs.
*
* @return   Returns void unless the Xil_AssertWait variable is true, in which
*           case no return is made and an infinite loop is entered.
*
******************************************************************************/
#define Xil_AssertVoid(Expression)                \
{                                                  \
    if (Expression) {                              \
        Xil_AssertStatus = XIL_ASSERT_NONE;       \
    } else {                                       \
        Xil_Assert(__FILE__, __LINE__);            \
        Xil_AssertStatus = XIL_ASSERT_OCCURRED;   \
        return;                                    \
    }                                              \
}

/*****************************************************************************/
/**
* @brief    This assert macro is to be used for functions that do return a
*           value. This in conjunction with the Xil_AssertWait boolean can be
*           used to accommodate tests so that asserts which fail allow execution
*           to continue.
*
* @param    Expression: expression to be evaluated. If it evaluates to false,
*           the assert occurs.
*
* @return   Returns 0 unless the Xil_AssertWait variable is true, in which
* 	        case no return is made and an infinite loop is entered.
*
******************************************************************************/
#define Xil_AssertNonvoid(Expression)             \
{                                                  \
    if (Expression) {                              \
        Xil_AssertStatus = XIL_ASSERT_NONE;       \
    } else {                                       \
        Xil_Assert(__FILE__, __LINE__);            \
        Xil_AssertStatus = XIL_ASSERT_OCCURRED;   \
        return 0;                                  \
    }                                              \
}

/*****************************************************************************/
/**
* @brief     Always assert. This assert macro is to be used for void functions.
*            Use for instances where an assert should always occur.
*
* @return    Returns void unless the Xil_AssertWait variable is true, in which
*	         case no return is made and an infinite loop is entered.
*
******************************************************************************/
#define Xil_AssertVoidAlways()                   \
{                                                  \
   Xil_Assert(__FILE__, __LINE__);                 \
   Xil_AssertStatus = XIL_ASSERT_OCCURRED;        \
   return;                                         \
}

/*****************************************************************************/
/**
* @brief   Always assert. This assert macro is to be used for functions that
*          do return a value. Use for instances where an assert should always
*          occur.
*
* @return Returns void unless the Xil_AssertWait variable is true, in which
*	      case no return is made and an infinite loop is entered.
*
******************************************************************************/
#define Xil_AssertNonvoidAlways()                \
{                                                  \
   Xil_Assert(__FILE__, __LINE__);                 \
   Xil_AssertStatus = XIL_ASSERT_OCCURRED;        \
   return 0;                                       \
}

#else

#define Xil_AssertVoid(Expression)
#define Xil_AssertVoidAlways()
#define Xil_AssertNonvoid(Expression)
#define Xil_AssertNonvoidAlways()

#endif

/************************** Function Prototypes ******************************/

void Xil_AssertSetCallback(Xil_AssertCallback Routine);

#ifdef __cplusplus
}
#endif

#endif

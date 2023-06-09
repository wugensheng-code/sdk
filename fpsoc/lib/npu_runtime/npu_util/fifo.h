#pragma once
#include "npu_defines.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @struct fifo_t
 * @brief  C style static fifo.
 */
typedef struct
{
    int   elem_size; //!< elements size which will be pushed into fifo
    int   elem_num;  //!< size of fifo
    int   wrptr;
    int   rdptr;
    AL_VOID* mem;       //!< visual aligned address which point to the very begging of elements
} fifo_t;

/*!
 * @brief  Create a fifo for a struct(or something else).
 *
 *            This function is not a STL-like version.
 *
 * @param [in]  elem_size: Size of the elements which will be pushed in.
 * @param [in]  elem_num : Number of elements.
 *
 * @return  The pointer of the fifo.
 */
fifo_t* create_fifo(int elem_size, int elem_num);

/*!
 * @brief Push a element into fifo from its pointer.
 *
 * @param [in]  v: The fifo which will be pushed a new element.
 * @param [in]  data: The pointer of new element.
 *
 * @return statue value, 0 success, other value failure.
 */
int push_fifo_data(fifo_t* v, any_data_t data);

/*!
 * @brief Push a element into fifo from its pointer.
 *
 * @param [in]  v: The fifo which will be pushed a new element.
 * @param [in]  data: The pointer of new element.
 *
 * @return statue value, 0 success, other value failure.
 */
any_data_t pop_fifo_data(fifo_t* v);

/*!
 * @brief Push a element into fifo from its pointer.
 *
 * @param [in]  v: The fifo which will be pushed a new element.
 * @param [in]  data: The pointer of new element.
 *
 * @return statue value, 0 success, other value failure.
 */
int fifo_free_room(fifo_t* v);

/*!
 * @brief Push a element into fifo from its pointer.
 *
 * @param [in]  v: The fifo which will be pushed a new element.
 * @param [in]  data: The pointer of new element.
 *
 * @return statue value, 0 success, other value failure.
 */
int fifo_item_num (fifo_t* v);

/*!
 * @brief Push a element into fifo from its pointer.
 *
 * @param [in]  v: The fifo which will be pushed a new element.
 * @param [in]  data: The pointer of new element.
 *
 * @return statue value, 0 success, other value failure.
 */
int fifo_is_full (fifo_t* v);

/*!
 * @brief Set a element via its index.
 *
 * @param [in]  v: The fifo.
 * @param [in]  index: The index of the element.
 * @param [in]  data: The pointer of new element.
 *
 * @return statue value, 0 success, other value failure.
 */
int set_fifo_data(fifo_t* v, int index, any_data_t data);

/*!
 * @brief Get a element via its index.
 *
 * @param [in]  v: The fifo.
 * @param [in]  index: The index of the element.
 *
 * @return  The pointer of the elements.
 */
any_data_t get_fifo_data(fifo_t* v, int index);

#ifdef __cplusplus
}
#endif /* __cplusplus */

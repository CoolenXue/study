/******************************************************************************
 *                                                                            *
 * This program is distributed in the hope that it will be useful, but        *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE. This file and program are licensed    *
 * under the GNU Lesser General Public License Version 3, 29 June 2007.       *
 * The complete license can be accessed from the following location:          *
 * http://opensource.org/licenses/lgpl-3.0.html                               *
 *                                                                            *
 * Author: Yun Li (yunli.open@gmail.com)                                      *
 *   Date: 08/03/2010                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date     Version  Name          Description
  -------- -------  ------------  --------------------------------------------

  -------- -------  ------------  --------------------------------------------

 ******************************************************************************/

#ifndef __BITMAP_H
#define __BITMAP_H

#include "primitive.h"

// Summary:
//   Supported bits is calculated by "CONFIG_MAX_BITMAP_ROW * CONFIG_MAX_BIT_PER_ROW".
//   The max bit supported by this module is 1024 with below configuration:
//       typedef u32_t task_bitmap_row_t;
//       CONFIG_MAX_BITMAP_ROW = 32;
//       CONFIG_MAX_BIT_PER_ROW = 32;
//   The min bits supported by this module is 8 with below configuration:
//       typedef u8_t task_bitmap_row_t;
//       CONFIG_MAX_BITMAP_ROW = 1;
//       CONFIG_MAX_BIT_PER_ROW = 8;

#define CONFIG_MAX_BITMAP_ROW   4
#define CONFIG_MAX_BIT_PER_ROW  32

#define BITS_SUPPORTED          (CONFIG_MAX_BITMAP_ROW*CONFIG_MAX_BIT_PER_ROW)
#define LAST_BIT                (BITS_SUPPORTED - 1)

#define INVALID_BIT             ((bit_t)-1)

typedef u32_t task_bitmap_row_t;
typedef u32_t bit_t;

typedef struct {
    task_bitmap_row_t buffer_ [CONFIG_MAX_BITMAP_ROW];
    task_bitmap_row_t row_bitmap_;
} task_bitmap_t, *task_bitmap_handle_t;

#ifdef  __cplusplus
extern "C"
{
#endif

void task_bitmap_init (task_bitmap_handle_t _handle);

void task_bitmap_bit_set (task_bitmap_handle_t _handle, bit_t _bit);
void task_bitmap_bit_clear (task_bitmap_handle_t _handle, bit_t _bit);

bit_t task_bitmap_lowest_bit_get (const task_bitmap_handle_t _handle);
bool task_bitmap_is_empty (const task_bitmap_handle_t _handle);

#ifdef __cplusplus
}
#endif

#endif


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
 *   Date: 08/29/2009                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date     Version  Name          Description
  -------- -------  ------------  --------------------------------------------

  -------- -------  ------------  --------------------------------------------

 ******************************************************************************/
 
#ifndef __MBLOCK_H
#define __MBLOCK_H

#include "error.h"
#include "module.h"
#include "primitive.h"

typedef struct {
    address_t next_;  // next start address of memory block
    msize_t size_;  // size of this block in g_alignment_bytes unit
} mblock_t;

typedef struct {
    address_t start_;
    address_t end_;
    msize_t alignment_in_bits_;
} heap_info_t;

#ifdef  __cplusplus
extern "C" {
#endif

void heap_info_get (heap_info_t *_p_info);
error_t heap_init (address_t _start, address_t _end, msize_t _alignment_in_bits);
void* heap_alloc (msize_t _size);
error_t heap_free (void* _p_buf, msize_t _size);
void heap_dump ();
error_t module_heap (system_state_t _state);

#ifdef __cplusplus
}
#endif

#endif


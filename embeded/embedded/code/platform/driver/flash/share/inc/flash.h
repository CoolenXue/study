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
 *   Date: 09/06/2010                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date     Version  Name          Description
  -------- -------  ------------  --------------------------------------------

  -------- -------  ------------  --------------------------------------------

 ******************************************************************************/
 
#ifndef __FLASH_H
#define __FLASH_H

#include "primitive.h"
#include "error.h"

#define OPTION_FLASH_ERASE      (((int)(MODULE_FLASH) << 8) + 1)
#define OPTION_FLASH_BURN       (((int)(MODULE_FLASH) << 8) + 2)

typedef enum {
    CHIP_ERASE,
    SECTOR_ERASE
} erasure_type_t;

typedef struct {
    erasure_type_t type_;
    u32_t sector_;    // optional parameter
} nor_flash_erasure_arg_t;

typedef enum {
    BYTE_BURN,
    SECTOR_BURN
} nor_flash_burn_type_t;

typedef struct {
    nor_flash_burn_type_t type_;
    union {
        address_t offset_;
        u32_t sector_;
    } address_;
    void *buffer_;
    usize_t size_;
} flash_burn_arg_t;

typedef struct {
    device_t common_;
    bool is_opened_;
    address_t base_addr_;
    usize_t sector_size_;
    usize_t sector_size_in_bits_;
    usize_t sector_count_;
} device_nor_flash_t, *nor_flash_handle_t;

#ifdef  __cplusplus
extern "C" {
#endif

error_t nor_flash_register (const char _name [], nor_flash_handle_t _handle, 
    address_t _base_addr, usize_t _sector_size, usize_t _sector_size_in_bits, 
    usize_t _sector_count);

#ifdef __cplusplus
}
#endif

#endif


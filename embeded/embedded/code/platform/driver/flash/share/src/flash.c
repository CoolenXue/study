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
  
  Date        Version  Name          Description
  ----------  -------  ------------  -------------------------------------------

  ----------  -------  ------------  -------------------------------------------

 ******************************************************************************/

#include "device.h"
#include "errflash.h"
#include "flash.h"
#include "interrupt.h"

error_t nor_flash_register (const char _name [], nor_flash_handle_t _handle, 
    address_t _base_addr, usize_t _sector_size, usize_t _sector_size_in_bits, 
    usize_t _sector_count)
{
    if (null == _name) {
        // a device has no name?
        return ERROR_T (ERROR_FLASH_REGISTER_INVNAME);
    }
    if (null == _handle) {
        return ERROR_T (ERROR_FLASH_REGISTER_INVHANDLE);
    }
    
    _handle->common_.interrupt_vector_ = INTERRUPT_NONE;
    _handle->common_.interrupt_handler_ = null;
    _handle->base_addr_ = _base_addr;
    _handle->sector_size_ = _sector_size;
    _handle->sector_size_in_bits_ = _sector_size_in_bits;
    _handle->sector_count_ = _sector_count;
    return device_register (_name, &_handle->common_);
}


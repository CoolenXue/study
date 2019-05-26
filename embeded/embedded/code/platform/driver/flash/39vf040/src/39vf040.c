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
 *   Date: 09/05/2010                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date        Version  Name          Description
  ----------  -------  ------------  -------------------------------------------

  ----------  -------  ------------  -------------------------------------------

 ******************************************************************************/

#include "39vf040.h"
#include "device.h"
#include "errflash.h"
#include "flash.h"

static error_t flash_open (device_handle_t _handle, open_mode_t _mode)
{
    nor_flash_handle_t handle = (nor_flash_handle_t)_handle;

    UNUSED (_mode);
    
    if (handle->is_opened_) {
        return ERROR_T (ERROR_FLASH_OPEN_OPENED);
    }

    handle->is_opened_ = true;
    return 0;
}

static error_t flash_close (device_handle_t _handle)
{
    nor_flash_handle_t handle = (nor_flash_handle_t)_handle;
    
    handle->is_opened_ = false;
    return 0;
}

static void flash_toggle_bit_check (address_t _addr)
{
    byte_t pre_value;
    byte_t cur_value;
    unsigned long timeout = 0;

    pre_value = *(byte_t *)(_addr);
    pre_value = pre_value & 0x40;
    
    while (timeout < 0x07FFFFFF) {
        cur_value = *(byte_t *)(_addr);
        cur_value = cur_value & 0x40;
        if (pre_value == cur_value)
            break;
        pre_value = cur_value;
        timeout ++;
    }
}

static error_t flash_control (device_handle_t _handle,
    control_option_t _option, int _int_arg, void *_ptr_arg)
{
    nor_flash_handle_t handle = (nor_flash_handle_t)_handle;

    UNUSED (_int_arg);
    
    switch (_option)
    {
    case OPTION_FLASH_ERASE:
        {
            nor_flash_erasure_arg_t *p_arg = (nor_flash_erasure_arg_t *)_ptr_arg;
            
            if (CHIP_ERASE == p_arg->type_) {
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0xAA;
                *(byte_t *) (handle->base_addr_ + 0x2AAA) = 0x55;
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0x80;
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0xAA;
                *(byte_t *) (handle->base_addr_ + 0x2AAA) = 0x55;
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0x10;
            }
            else if (SECTOR_ERASE == p_arg->type_) {
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0xAA;
                *(byte_t *) (handle->base_addr_ + 0x2AAA) = 0x55;
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0x80;
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0xAA;
                *(byte_t *) (handle->base_addr_ + 0x2AAA) = 0x55;
                *(byte_t *) (handle->base_addr_ + p_arg->sector_) = 0x30;
            }
            flash_toggle_bit_check (handle->base_addr_);
        }
        break;
    case OPTION_FLASH_BURN:
        {
            flash_burn_arg_t *p_arg = (flash_burn_arg_t *)_ptr_arg;
            address_t addr = handle->base_addr_;
            usize_t left = p_arg->size_;
            byte_t *p_buf = (byte_t *)p_arg->buffer_;
                
            if (BYTE_BURN == p_arg->type_) {
                addr += p_arg->address_.offset_;
            }
            else if (SECTOR_BURN == p_arg->type_) {
                addr += p_arg->address_.sector_ << handle->sector_size_in_bits_;
            }

            while (left -- > 0) {
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0xAA;
                *(byte_t *) (handle->base_addr_ + 0x2AAA) = 0x55;
                *(byte_t *) (handle->base_addr_ + 0x5555) = 0xA0;
                *(byte_t *) (addr) = *p_buf ++;
            }
            flash_toggle_bit_check (handle->base_addr_);
        }
        break;
    default:
        return ERROR_T (ERROR_FLASH_CONTROL_INVOPT);
    }
    return 0;
}

error_t flash_39vf040_driver_install (const char _name[])
{
    device_operation_t opt = {
        .open_ = flash_open, 
        .close_ = flash_close,
        .control_ = flash_control,
    };
    return driver_install (_name, &opt, 0);
}


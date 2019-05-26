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
 *   Date: 12/05/2010                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date     Version  Name          Description
  -------- -------  ------------  --------------------------------------------

  -------- -------  ------------  --------------------------------------------

 ******************************************************************************/

#include "main.h"
#include "device.h"
#include "console.h"
#include "timer.h"

#define DURATION_ONE_SECOND     1000

static void log_timer_event (const char *_name)
{
    if (is_in_interrupt ()) {
        console_print ("Info: timer \"%s\" is expired [in interrupt]\n", _name);
    }
    else {
        console_print ("Info: timer \"%s\" is expired [in \"%s\" task]\n", _name, 
            task_self ()->name_);
    }
}

static void callback_timer (timer_handle_t _handle, void *_arg)
{
    UNUSED (_arg);
    
    (void) timer_restart (_handle);
    log_timer_event (_handle->name_);
}

static void task_test (const char _name [], void *_p_arg)
{
    timer_handle_t handle = (timer_handle_t)_p_arg;

    UNUSED (_name);
   
    (void) timer_start (handle, DURATION_ONE_SECOND, callback_timer, 0);
    (void) task_sleep (11000);
    timer_dump ();
    multitasking_stop ();
}

error_t module_testapp (system_state_t _state)
{
    static device_handle_t ctrlc_handle;
    static task_handle_t handle;
    STACK_DECLARE (stack, 2048);
    static timer_handle_t timer;
    
    if (STATE_INITIALIZING == _state) {
        (void) timer_alloc (&timer, "One Second", TIMER_TYPE_INTERRUPT);
        
        (void) task_create (&handle, "Test", 16, stack, sizeof (stack));
        (void) task_start (handle, task_test, timer);
        
        (void) device_open (&ctrlc_handle, "/dev/ui/ctrlc", 0);
    }
    else if (STATE_DESTROYING == _state) {
        (void) device_close (ctrlc_handle);
        (void) task_delete (handle);
        (void) timer_free (timer);
    }
    return 0;
}

int module_registration_entry (int argc, char *argv [])
{
    UNUSED (argc);
    UNUSED (argv);
    
    (void) module_register ("Interrupt", MODULE_INTERRUPT, CPU_LEVEL, module_interrupt);
    (void) module_register ("Device", MODULE_DEVICE, DRIVER_LEVEL, module_device);
    (void) module_register ("Timer", MODULE_TIMER, OS_LEVEL, module_timer);
    (void) module_register ("Task", MODULE_TASK, OS_LEVEL, module_task);
    (void) module_register ("TestApp", MODULE_TESTAPP, APPLICATION_LEVEL3, module_testapp);
    return 0;
}


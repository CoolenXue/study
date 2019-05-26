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
 *   Date: 08/26/2010                                                         *
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
#include "mutex.h"
#include "console.h"

static void task_deadlock (const char _name [], void *_p_arg)
{
    mutex_handle_t mutex = (mutex_handle_t)_p_arg;

    console_print ("%s: going to take lock\n", _name);
    (void) mutex_lock (mutex, WAIT_FOREVER);
    console_print ("%s: lock is taken\n", _name);
    console_print ("%s: going to take twice\n", _name);
    (void) mutex_lock (mutex, WAIT_FOREVER);
    multitasking_stop ();
}

error_t module_testapp (system_state_t _state)
{
    static task_handle_t task;
    STACK_DECLARE (stack, 1024);
    static mutex_handle_t mutex;
    static device_handle_t ctrlc_handle;
    
    if (STATE_INITIALIZING == _state) {
        (void) mutex_create (&mutex, "Test");
        
        (void) task_create (&task, "Task Low", 16, stack, sizeof (stack));
        (void) task_start (task, task_deadlock, mutex);
        
        (void) device_open (&ctrlc_handle, "/dev/ui/ctrlc", 0);
    }
    else if (STATE_DESTROYING == _state) {
        (void) device_close (ctrlc_handle);
        (void) task_delete (task);
        (void) mutex_delete (mutex);
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
    (void) module_register ("Mutex", MODULE_MUTEX, OS_LEVEL, module_mutex);
    (void) module_register ("TestApp", MODULE_TESTAPP, APPLICATION_LEVEL3, module_testapp);
    return 0;
}


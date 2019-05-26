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
 *   Date: 08/21/2010                                                         *
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
#include "event.h"
#include "console.h"

#define EVENT   0x01

static task_handle_t g_task_high;
static task_handle_t g_task_middle;
static task_handle_t g_task_low;

STACK_DECLARE (g_stack_for_high, 1024);
STACK_DECLARE (g_stack_for_middle, 1024);
STACK_DECLARE (g_stack_for_low, 1024);

static void task_high (const char _name [], void *_p_arg)
{
    mutex_handle_t mutex = (mutex_handle_t)_p_arg;

    console_print ("%s: yeild CPU\n", _name);
    (void) task_sleep (0);
    console_print ("%s: yeild CPU again\n", _name);
    (void) task_sleep (0);
    console_print ("%s: trying to lock\n", _name);
    (void) mutex_lock (mutex, WAIT_FOREVER);
    console_print ("%s: locked\n", _name);
    (void) mutex_unlock (mutex);
    console_print ("%s: unlocked\n", _name);
}

static void task_middle (const char _name [], void *_p_arg)
{
    event_set_t received;

    UNUSED (_p_arg);

    console_print ("%s: receiving event\n", _name);
    (void) event_receive (EVENT, &received, WAIT_FOREVER, EVENT_WAIT_ANY
        | EVENT_RETURN_EXPECTED);
    console_print ("%s: event received\n", _name);
}

static void task_low (const char _name [], void *_p_arg)
{
    mutex_handle_t mutex = (mutex_handle_t)_p_arg;

    console_print ("%s: trying to lock\n", _name);
    (void) mutex_lock (mutex, WAIT_FOREVER);
    console_print ("%s: locked\n", _name);
    console_print ("%s: sending event\n", _name);
    (void) event_send (g_task_middle, EVENT);
    console_print ("%s: event sent\n", _name);
    (void) mutex_unlock (mutex);
    console_print ("%s: unlocked\n", _name);
    multitasking_stop ();
}

error_t module_testapp (system_state_t _state)
{
    static mutex_handle_t mutex;
    static device_handle_t ctrlc_handle;
    
    if (STATE_INITIALIZING == _state) {
        (void) mutex_create (&mutex, "Test");
        
        (void) task_create (&g_task_low, "Low", 16, g_stack_for_low, 
            sizeof (g_stack_for_low));
        (void) task_start (g_task_low, task_low, mutex);
        
        (void) task_create (&g_task_middle, "Middle", 13, g_stack_for_middle, 
            sizeof (g_stack_for_middle));
        (void) task_start (g_task_middle, task_middle, mutex);
        
        (void) task_create (&g_task_high, "High", 11, g_stack_for_high, 
            sizeof (g_stack_for_high));
        (void) task_start (g_task_high, task_high, mutex);
        
        (void) device_open (&ctrlc_handle, "/dev/ui/ctrlc", 0);
    }
    else if (STATE_DESTROYING == _state) {
        (void) device_close (ctrlc_handle);
        (void) task_delete (g_task_high);
        (void) task_delete (g_task_middle);
        (void) task_delete (g_task_low);
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


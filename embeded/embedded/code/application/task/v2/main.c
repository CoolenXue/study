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
 *   Date: 08/14/2010                                                         *
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
#include "taskvar.h"
#include "device.h"
#include "console.h"

//lint -esym(728, g_total)

static char *g_hello;
static int g_count;
static int g_total;

static void task_entry (const char _name [], void *_p_arg)
{
    UNUSED (_p_arg);
    //lint -e{740}
    (void) task_variable_add ((value_t *)&g_hello);
    //lint -e{740}
    (void) task_variable_add ((value_t *)&g_count);
    
    g_hello = malloc (32);
    if (null == g_hello) {
        console_print ("Error: malloc () failture\n");
        return;
    }
    g_count = 5;
    
    (void) snprintf (g_hello, 32, "Hello %s\n", _name);
    
    while (g_count -- > 0) {
        interrupt_level_t old_level;
        
        console_print ("%u: %s", g_count, g_hello);
        (void) fflush (stdout);
        
        old_level = global_interrupt_disable ();
        g_total ++;
        global_interrupt_enable (old_level);
        
        (void) task_sleep (300);
    }

    free (g_hello);
    if (10 == g_total) {
        // for shuting down the system
        multitasking_stop ();
    }
}

error_t module_testapp (system_state_t _state)
{
    static task_handle_t task0;
    static task_handle_t task1;
    static device_handle_t ctrlc_handle;
    STACK_DECLARE (stack0, 1024);
    STACK_DECLARE (stack1, 1024);
    
    if (STATE_INITIALIZING == _state) {
        (void) task_create (&task0, "Yun", 11, stack0, sizeof (stack0));
        (void) task_start (task0, task_entry, 0);
        
        (void) task_create (&task1, "Fang", 16, stack1, sizeof (stack1));
        (void) task_start (task1, task_entry, 0);
        
        (void) device_open (&ctrlc_handle, "/dev/ui/ctrlc", 0);
    }
    else if (STATE_DESTROYING == _state) {
        (void) device_close (ctrlc_handle);
        (void) task_delete (task1);
        (void) task_delete (task0);
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


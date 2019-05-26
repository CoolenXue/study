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
 *   Date: 12/02/2010                                                         *
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
#include "mpool.h"
#include "console.h"

//lint -e754

#define BUFFER_COUNT        32

typedef struct {
    char buf_ [32];
} buffer_t;


static void task_test (const char _name [], void *_p_arg)
{
    error_t error;
    void *p_buf1, *p_buf2, *p_buf3, *p_buf4, *p_buf5;
    MPOOL_MEMORY_DECLARE (pool_node, pool_buffer, buffer_t, BUFFER_COUNT);
    mpool_handle_t handle;

    UNUSED (_name);
    UNUSED (_p_arg);
   
    error = mpool_create ("Test", &handle, pool_node, pool_buffer, 
        sizeof (buffer_t), sizeof (pool_buffer)/sizeof (pool_buffer [0]));
    if (0 != error) {
        console_print ("Error: mpool_create() returns %s!\n", errstr (error));
        multitasking_stop ();
    }
    
    console_print ("\n");
    console_print ("===========\n");
    console_print ("Before Test ->\n");
    console_print ("===========\n");
    mpool_dump ();
    
    console_print ("===============================\n");
    console_print ("Test Case 1): allocate a buffer ->\n");
    console_print ("===============================\n");
    p_buf1 = mpool_buffer_alloc (handle);
    console_print ("    Allocated Addr: %p\n\n", p_buf1);
    mpool_dump ();
    
    console_print ("=====================================\n");
    console_print ("Test Case 2): allocate 4 more buffers ->\n");
    console_print ("=====================================\n");
    p_buf2 = mpool_buffer_alloc (handle);
    p_buf3 = mpool_buffer_alloc (handle);
    p_buf4 = mpool_buffer_alloc (handle);
    p_buf5 = mpool_buffer_alloc (handle);
    console_print ("    Allocated Addr: %p\n", p_buf2);
    console_print ("    Allocated Addr: %p\n", p_buf3);
    console_print ("    Allocated Addr: %p\n", p_buf4);
    console_print ("    Allocated Addr: %p\n\n", p_buf5);
    mpool_dump ();
    
    console_print ("==============================\n");
    console_print ("Test Case 3): free all buffers ->\n");
    console_print ("==============================\n");
    (void) mpool_buffer_free (handle, p_buf1);
    (void) mpool_buffer_free (handle, p_buf2);
    (void) mpool_buffer_free (handle, p_buf3);
    (void) mpool_buffer_free (handle, p_buf4);
    (void) mpool_buffer_free (handle, p_buf5);
    mpool_dump ();

    (void) mpool_delete (handle);
    multitasking_stop ();
}

error_t module_testapp (system_state_t _state)
{
    static task_handle_t handle;
    STACK_DECLARE (stack, 1024);
    
    if (STATE_INITIALIZING == _state) {
        (void) task_create (&handle, "Test", 16, stack, sizeof (stack));
        (void) task_start (handle, task_test, 0);
    }
    else if (STATE_DESTROYING == _state) {
        (void) task_delete (handle);
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
    (void) module_register ("Mpool", MODULE_MPOOL, OS_LEVEL, module_mpool);
    (void) module_register ("TestApp", MODULE_TESTAPP, APPLICATION_LEVEL3, module_testapp);
    return 0;
}


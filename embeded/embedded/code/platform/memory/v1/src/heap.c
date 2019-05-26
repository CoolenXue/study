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
 *   Date: 11/24/2010                                                         *
 *                                                                            *
 ******************************************************************************/

/******************************************************************************
  REVISION HISTORY
  ================
  
  Date        Version  Name          Description
  ----------  -------  ------------  -------------------------------------------
                                     
  ----------  -------  ------------  -------------------------------------------

 ******************************************************************************/

#include "alignment.h"
#include "heap.h"
#include "errheap.h"
#include "console.h"
#include "interrupt.h"
#include "task.h"

static bool g_initialized;
static mblock_t g_mblock_free;
static address_t g_heap_addr_start;
static address_t g_heap_addr_end;
static msize_t g_heap_size;
// minimal allocation size
static msize_t g_min_alloc_size;
// the alignment for each allocation:
//     g_alignment_bytes = (1 << g_alignment_in_bits);
static msize_t g_alignment_bytes;
static msize_t g_alignment_in_bits;

error_t heap_init (address_t _start, address_t _end, msize_t _alignment_in_bits)
{
    mblock_t* p_mblock;

    if (_start > _end) {
        return ERROR_T (ERROR_HEAP_INIT_INVADDR);
    }
    if (g_initialized) {
        return 0;
    }
    g_alignment_in_bits = _alignment_in_bits;
    g_alignment_bytes = ((msize_t)1 << g_alignment_in_bits);
    if (sizeof (int) > g_alignment_bytes) {
        return ERROR_T (ERROR_HEAP_INIT_INVALIGN);
    }
    
    // initialize the g_mblock_free, it holds first free block
    g_mblock_free.next_ = ALIGN (_start, g_alignment_bytes);
    // the length is in g_alignment_bytes unit
    g_mblock_free.size_ = (_end - g_mblock_free.next_) >> 
        g_alignment_in_bits;
    // initialize the free block and its size
    p_mblock = (mblock_t *)g_mblock_free.next_;
    p_mblock->next_ = 0;
    p_mblock->size_ = g_mblock_free.size_;
    // save the start and end address for later use
    g_heap_addr_start = g_mblock_free.next_;
    g_heap_addr_end = g_mblock_free.next_ + 
        (g_mblock_free.size_ << g_alignment_in_bits);
    g_heap_size = g_mblock_free.size_;
    // minimal allocation size should be bigger than the size of mblock_t
    g_min_alloc_size = (sizeof (mblock_t) + g_alignment_bytes - 1) >>
        g_alignment_in_bits;
    
    g_initialized = true;
    return 0;
}

void* heap_alloc (msize_t _size)
{
    register mblock_t *p_pre, *p_next, *p_superfluous;
    msize_t size_required, size_superfluous;
    interrupt_level_t level;

    if (!g_initialized || is_in_interrupt () || 0 == _size) {
        return null;
    }
    
    // convert the _size into g_alignment_bytes unit
    size_required = ((_size + g_alignment_bytes - 1) >> g_alignment_in_bits);
    if (g_min_alloc_size > size_required) {
        size_required = g_min_alloc_size;
    }
    level = global_interrupt_disable ();
    if (g_mblock_free.size_ < size_required) {
        global_interrupt_enable (level);
        return null;
    }
    p_pre = p_next = &g_mblock_free;
    for (;;) {
        p_next = (mblock_t *)p_next->next_;
        if (0 == p_next) {
            // till now we have traversed all the block and didn't find a block
            // for this request
            global_interrupt_enable (level);
            return null;
        }
        // if this block size cannot meet the request, continue next block
        if (p_next->size_ < size_required) {
            p_pre = p_next;
            continue;
        }
        // block size is bigger or equal to size_required, get the remaining
        // free size
        size_superfluous = p_next->size_ - size_required;
        if (size_superfluous <= g_min_alloc_size) {
            size_required = p_next->size_;
            p_superfluous = (mblock_t *)p_next->next_;
            break;
        }
        // put the superfluous block into the free list
        p_superfluous = (mblock_t *)((address_t)p_next + 
            (size_required << g_alignment_in_bits));
        p_superfluous->next_ = p_next->next_;
        p_superfluous->size_ = size_superfluous;
        break;
    }
    p_pre->next_ = (address_t)p_superfluous;
    g_mblock_free.size_ -= size_required;
    global_interrupt_enable (level);
    return p_next;
}

error_t heap_free (void* _p_buf, msize_t _size)
{
    register address_t p_buf = (address_t)_p_buf;
    register mblock_t* p_pre,*p_next;
    interrupt_level_t level;
    msize_t size;

    if (!g_initialized) {
        return ERROR_T (ERROR_HEAP_FREE_NOTINIT);
    }
    if (is_in_interrupt () && STATE_UP == system_state ()) {
        return ERROR_T (ERROR_HEAP_ALLOC_INVCONTEXT);
    }
    if (0 == _size) {
        return ERROR_T (ERROR_HEAP_FREE_INVSIZE);
    }
    if (0 == p_buf || ((p_buf  & (g_alignment_bytes - 1)) != 0) ||
        p_buf  < g_heap_addr_start ||
        p_buf  > g_heap_addr_end || _size > g_heap_size) {
        return ERROR_T (ERROR_HEAP_FREE_INVBUF);
    }
    
    size = ((_size + g_alignment_bytes - 1) >> g_alignment_in_bits);
    if (g_min_alloc_size > size) {
        size = g_min_alloc_size;
    }
    level = global_interrupt_disable ();
    g_mblock_free.size_ += size;
    if (0 == g_mblock_free.next_) {
        g_mblock_free.next_ = p_buf ;
        p_next = (mblock_t *)p_buf ;
        p_next->size_ = size;
        p_next->next_ = 0;
        global_interrupt_enable (level);
        return 0;
    }
    else {
        p_pre = &g_mblock_free;
        p_next = (mblock_t *)g_mblock_free.next_;
    }
    // find the right position of the list
    while (p_pre->next_ < p_buf ) {
        p_pre = p_next;
        p_next = (mblock_t *)p_next->next_;
        if (null == p_next) {
            break;
        }
    }
    // merge with the previously adjacent block if needed
    if ((((p_pre->size_ << g_alignment_in_bits) + (address_t)p_pre) == p_buf ) 
        && (p_pre != &g_mblock_free)) {
        p_pre->size_ += size;
    }
    else {
        p_pre->next_ = p_buf ;
        p_pre = (mblock_t *)p_buf;
        p_pre->size_ = size;
        p_pre->next_ = (address_t)p_next;
    }
    if (0 == p_next) {
        // this is the last block no more mergence is needed
        global_interrupt_enable (level);
        return 0;
    }
    // merge with the following adjacent block if needed
    if (((p_pre->size_ << g_alignment_in_bits) + (address_t)p_pre) == 
        (address_t)p_next) {
        p_pre->size_ += p_next->size_;
        p_pre->next_ = p_next->next_;
    }
    global_interrupt_enable (level);
    return 0;
}

void heap_dump ()
{
    register mblock_t* p_mblock = &g_mblock_free;
    msize_t size;
    
    if (!g_initialized || is_in_interrupt ()) {
        return;
    }
    
    scheduler_lock ();
    console_print ("\n");
    console_print ("Free Block(s)\n");
    console_print ("-------------\n");
    p_mblock = (mblock_t *)p_mblock->next_;
    if (0 == p_mblock) {
        console_print ("None!\n");
        scheduler_unlock ();
        return;
    }
    do {
        console_print ("  Block Start Addr: %x, size: %x (%u)\n",
            (address_t)p_mblock, (p_mblock->size_ << g_alignment_in_bits),
            (p_mblock->size_ << g_alignment_in_bits));
    } while ((p_mblock = (mblock_t *)p_mblock->next_) != 0);
    console_print ("\n");
    console_print ("Summary\n");
    console_print ("-------\n");
    console_print ("  Alignment: %u\n", g_alignment_bytes);
    console_print ("  Heap Addr: %x\n", g_heap_addr_start);
    size = (g_heap_size << g_alignment_in_bits);
    console_print ("      Total: %x (%u)\n", size, size);
    size = ((g_heap_size - g_mblock_free.size_) << g_alignment_in_bits);
    console_print ("       Used: %x (%u)\n", size, size);
    size = (g_mblock_free.size_ << g_alignment_in_bits);
    console_print ("       Free: %x (%u)\n", size, size);
    console_print ("\n");
    scheduler_unlock ();
}

error_t module_heap (system_state_t _state)
{
    if (STATE_INITIALIZING == _state) {
        heap_info_t heap_info;
        heap_info_get (&heap_info);
        return heap_init (heap_info.start_, heap_info.end_, 
            heap_info.alignment_in_bits_);
    }
    else if (STATE_DESTROYING == _state) {
        msize_t size = ((g_heap_size - g_mblock_free.size_) << g_alignment_in_bits);
        if (0 != size) {
            console_print ("Error: %d bytes of memory are not freed");
        }
    }
    return 0;
}


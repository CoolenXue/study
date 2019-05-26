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
 *   Date: 11/25/2010                                                         *
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

static const address_t ADDRESS_MARK = 0x24891513;
static const msize_t SIZE_MARK = 0x18132A49;

static bool g_initialized;
static mblock_t g_mblock_free;
static address_t g_heap_addr_start;
static address_t g_heap_addr_end;
static msize_t g_heap_size;
// minimal allocation size
static msize_t g_min_alloc_size;
static msize_t g_mhead_size;
// the alignment for each allocation:
//     g_alignment_bytes = (1 << g_alignment_in_bits);
static msize_t g_alignment_bytes;
static msize_t g_alignment_in_bits;

static inline void mblock_integrate (register mblock_t* _p_mblock)
{
    _p_mblock->next_not_ = ADDRESS_MARK ^ _p_mblock->next_;
    _p_mblock->size_not_ = SIZE_MARK ^ _p_mblock->size_;
}

static inline bool mblock_integrity_check (register const mblock_t *_p_mblock)
{
    if (_p_mblock->next_not_ != (ADDRESS_MARK ^ _p_mblock->next_)) {
        return false;
    }
    if (_p_mblock->size_not_ != (SIZE_MARK ^ _p_mblock->size_)) {
        return false;
    }
    return true;
}

static inline void mhead_integrate (register mhead_t* _p_mhead)
{
    _p_mhead->size_not_ = SIZE_MARK ^ _p_mhead->size_;
}

static inline bool mhead_integrity_check (register const mhead_t* _p_mhead)
{
    if (_p_mhead->size_not_ != (SIZE_MARK ^ _p_mhead->size_)) {
        return false;
    }
    return true;
}

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
    
    g_mhead_size = (sizeof (mhead_t) + g_alignment_bytes - 1) >> 
        g_alignment_in_bits;
    // initialize the g_mblock_free, it holds first free block
    g_mblock_free.next_ = ALIGN (_start, g_alignment_bytes);
    // the length is in g_alignment_bytes unit
    g_mblock_free.size_ = (_end - g_mblock_free.next_) >> 
        g_alignment_in_bits;
    mblock_integrate (&g_mblock_free);
    // initialize the free block and its size
    p_mblock = (mblock_t *)g_mblock_free.next_;
    p_mblock->next_ = 0;
    p_mblock->size_ = g_mblock_free.size_;
    mblock_integrate (p_mblock);
    // save the start and end address for later use
    g_heap_addr_start = g_mblock_free.next_;
    g_heap_addr_end = g_mblock_free.next_ + 
        (g_mblock_free.size_ << g_alignment_in_bits);
    g_heap_size = g_mblock_free.size_;
    //lint -e{506} minimal allocation size should be bigger than the size of mblock_t
    g_min_alloc_size = sizeof (mhead_t) < sizeof (mblock_t) ?
        (sizeof (mblock_t) + g_alignment_bytes - 1) >> g_alignment_in_bits :
        g_mhead_size;
        
    g_initialized = true;
    return 0;
}

void* heap_alloc (msize_t _size)
{
    register mblock_t *p_pre, *p_next, *p_superfluous;
    msize_t size_required, size_superfluous;
    interrupt_level_t level;
    mhead_t *p_mhead;

    if (!g_initialized || is_in_interrupt () || 0 == _size) {
        return null;
    }
    
    // convert the _size into g_alignment_bytes unit
    size_required = ((_size + g_alignment_bytes - 1) >> g_alignment_in_bits);
    size_required += g_mhead_size;
    if (g_min_alloc_size > size_required) {
        size_required = g_min_alloc_size;
    }
    level = global_interrupt_disable ();
    // check whether have enough memory for this allocation request to fast failure
    if (!mblock_integrity_check (&g_mblock_free) || 
        (g_mblock_free.size_ < size_required)) {
        global_interrupt_enable (level);
        return null;
    }
    p_pre = p_next = &g_mblock_free;
    for (;;) {
        p_next = (mblock_t *)p_next->next_;
        if (0 == p_next || !mblock_integrity_check (p_next)) {
            // till now we have traversed all the block and didn't find a
            // block for this request
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
        mblock_integrate (p_superfluous);
        break;
    }
    p_pre->next_ = (address_t)p_superfluous;
    mblock_integrate (p_pre);
    g_mblock_free.size_ -= size_required;
    mblock_integrate (&g_mblock_free);
    global_interrupt_enable (level);
    //lint -e{826}
    p_mhead = ptr2mhead (p_next);
    p_mhead->size_ = size_required;
    mhead_integrate (p_mhead);
    return ptr2buf (p_next);
}

error_t heap_free (const void* _p_buf)
{
    register mblock_t* p_pre,*p_next;
    interrupt_level_t level;
    //lint -e{826}
    register address_t p_ptr = (address_t) buf2ptr (_p_buf);
    //lint -e{826}
    mhead_t* p_mhead = ptr2mhead (p_ptr);

    if (!g_initialized) {
        return ERROR_T (ERROR_HEAP_FREE_NOTINIT);
    }
    if (is_in_interrupt () && STATE_UP == system_state ()) {
        return ERROR_T (ERROR_HEAP_ALLOC_INVCONTEXT);
    }
    //lint -e{774}
    if (0 == p_ptr || ((p_ptr & (g_alignment_bytes - 1)) != 0) ||
        (p_ptr < g_heap_addr_start) || (p_ptr > g_heap_addr_end)) {
        return ERROR_T (ERROR_HEAP_FREE_INVBUF);
    }
    if (!mhead_integrity_check (p_mhead)) {
        return ERROR_T (ERROR_HEAP_FREE_INVMHEAD);
    }
    
    level = global_interrupt_disable ();
    if (!mblock_integrity_check (&g_mblock_free)) {
        global_interrupt_enable (level);
        return ERROR_T (ERROR_HEAP_FREE_INVMBLOCK1);
    }
    g_mblock_free.size_ += p_mhead->size_;
    mblock_integrate (&g_mblock_free);
    if (0 == g_mblock_free.next_) {
        g_mblock_free.next_ = p_ptr;
        p_next = (mblock_t *)p_ptr;
        p_next->size_ = p_mhead->size_;
        p_next->next_ = 0;
        mblock_integrate (p_next);
        global_interrupt_enable (level);
        return 0;
    }
    else {
        p_pre = &g_mblock_free;
        p_next = (mblock_t *)g_mblock_free.next_;
    }
    if (!mblock_integrity_check (p_pre) || 
        !mblock_integrity_check (p_next)) {
        global_interrupt_enable (level);
        return ERROR_T (ERROR_HEAP_FREE_INVMBLOCK2);
    }
    // find the right position of the list
    while (p_pre->next_ < p_ptr) {
        p_pre = p_next;
        p_next = (mblock_t *)p_next->next_;
        if (null == p_next) {
            break;
        }
        if (!mblock_integrity_check (p_next)) {
            global_interrupt_enable (level);
            return ERROR_T (ERROR_HEAP_FREE_INVMBLOCK3);
        }
    }
    // merge with the previously adjacent block if needed
    if ((((p_pre->size_ << g_alignment_in_bits) + (address_t)p_pre) == p_ptr) 
        && (p_pre != &g_mblock_free)) {
        p_pre->size_ += p_mhead->size_;
        mblock_integrate (p_pre);
    }
    else {
        p_pre->next_ = p_ptr;
        mblock_integrate (p_pre);
        p_pre = (mblock_t *)p_ptr;
        p_pre->size_ = p_mhead->size_;
        p_pre->next_ = (address_t)p_next;
        mblock_integrate (p_pre);
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
        mblock_integrate (p_pre);
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
    if (!mblock_integrity_check (p_mblock)) {
        goto error;
    }
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
        if (!mblock_integrity_check (p_mblock)) {
            goto error;
        }
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
    return;
    
error:
    console_print ("\nPANIC: Invalid mblock detected (address is %x)\n", p_mblock);
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


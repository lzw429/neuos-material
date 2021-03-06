/*
 * File to show mm functional and test mm
 *
 */

#include <linux/kernel.h>

#define invalidate() \
    __asm__ volatile("mov %%eax, %%cr3"::"a" (0)) // 冲刷 TLB

unsigned long put_page(unsigned long page, unsigned long address);

void testoom() {
    for(int i = 0; i < 20 * 1024 * 1024; i += 4096)
        do_no_page(0, i);
    // This should not return!
    return ;
}

void test_put_page() {
    char *b = 0x100000;
    calc_mem();
    //put_page(0x200000, 0x100000);
    calc_mem();
    *b = 'k';
    while(1);
}

// Helper function to convert linear address to PTE
// return physical address on success
// return NULL(0) on failed
unsigned long *linear_to_pte(unsigned long addr) {
    // get the Page Directory Entry first
    // This variable will be used to refer to pte later
    // (for saving memory XD)
    unsigned long *pde = (unsigned long *)((addr >> 20) & 0xffc);
    // Page dir not exist
    // Or the address is not inside the page table address range(<=4KB)
    if(!(*pde & 1) || pde > 0x1000) {
        return 0;
    }
    // Now it is page table address :P
    pde = (unsigned long *)(*pde & 0xfffff000);
    // Page table address + page_table index = PTE
    //
    // Remember: x >> 12 & 0x3ff != x >> 10 & 0xffc (后者比前者二进制末尾多了两个0)
    return pde + ((addr >> 12) & 0x3ff);
}

void disable_linear(unsigned long addr) {
    // Your code here
    // Hint: Modify the correct page table entry
    // then you can make the page Not Present

    // Maybe you need a helper function for get
    // the page table entry for a specific address

    unsigned long *pte = linear_to_pte(addr);
    // Disable it
    *pte = 0;
    // Then RELOAD the TLB
    invalidate();
    return ;
}

void mm_read_only(unsigned long addr) {
    unsigned long *pte = linear_to_pte(addr);
    printk("Before: 0x%x\n", *pte);
    *pte = *pte & 0xfffffffd;
    printk("After: 0x%x\n", *pte);
    invalidate();
    return ;
}

void mm_print_pageinfo(unsigned long addr) {
    unsigned long *pte = linear_to_pte(addr);
    printk("Linear addr: 0x%x, PTE addr = 0x%x. Flags [ ", addr, pte);
    if(*pte & 0x1) printk("P ");
    if(*pte & 0x2) printk("R/W ");
    else printk("RO ");
    if(*pte & 0x4) printk("U/S ");
    else printk("S ");
    printk("]\n");
    printk("Phyaddr = %x\n", (*pte & 0xfffff000));
}


int mmtest_main(void) {
    printk("Running Memory function tests\n");
    printk("Calling page fault");

    // ** 实验 5 Step 1 ** //
    // 使用 mm_test.c 中的相关函数，在本函数中引发一次缺页中断
    // 一种可行的方式是，确保某一页不可用，再尝试访问该页
    // ** 请在此处键入你的代码 ** //

}

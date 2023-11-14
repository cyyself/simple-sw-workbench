#include "mmu.h"
#include "uart.h"

struct simple_mmu simplemm;

void mmu_clear_mem(void *addr, unsigned long size) {
    unsigned long *ptr = (unsigned long *)addr;
    while (size && ((unsigned long)addr % 8)) {
        *(unsigned char *)addr = 0;
        addr ++;
        size --;
    }
    while (size) {
        *(unsigned long*)ptr = 0;
        ptr ++;
        size -= 8;
    }
}

unsigned long mmu_new_page_table_page() {
    simplemm.page_table_alloc_ptr -= 4096;
    mmu_clear_mem((void*)simplemm.page_table_alloc_ptr, 4096);
    return simplemm.page_table_alloc_ptr;
}

void mmu_map_new(unsigned long base_addr, unsigned long vaddr, unsigned long paddr, int idx) {
    // assume we have all page table pages which pa = va
    // idx: 0: 4KB, 1: 2MB, 2: 1GB
    for (int i=2;i>=idx;i--) {
        unsigned long *pte = ((unsigned long *)base_addr)+((vaddr>>(12+9*i))&0x1ff);
        if (i == idx) {
            if (*pte & SV39_PTE_BIT_V) print_s("Error! Page already exist\n");
            else {
                *pte =  SV39_PTE_BIT_V | SV39_PTE_BIT_R | SV39_PTE_BIT_W | 
                        SV39_PTE_BIT_X | SV39_PTE_BIT_A | SV39_PTE_BIT_D |
                        SV39_SET_PA(paddr); // Don't set U bit as mstatus.SUM is prohibited by default
            }
        }
        else {
            if (!(*pte & SV39_PTE_BIT_V)) {
                // allocate new page
                unsigned long new_page = mmu_new_page_table_page();
                *pte = SV39_PTE_BIT_V | SV39_SET_PA(new_page);
            }
            base_addr = SV39_GET_PA(*pte);
        }
    }
    asm volatile("fence rw,rw");
    asm volatile("sfence.vma");
}

void mmu_pmp_allow_all() {
#ifdef ARCH_HAS_PMP
    asm volatile("csrw pmpaddr0, %0" : : "r" (0xfffffffffffffffful)); // napot all
    asm volatile("csrw pmpcfg0, %0" : : "r" ((3<<3) | 7)); // 7->rwx (3<<3)->a:napot
#endif
}

void mmu_init() {
    unsigned long tmp;
    simplemm.mem_range = 0x88000000; // 128MB
    simplemm.page_table_alloc_ptr = 0x88000000;
    simplemm.page_table_ptr = (unsigned long)mmu_new_page_table_page();    
    // map 0x80000000 + 1GB
    print_s("mmu map new begin!\n");
    mmu_map_new(simplemm.page_table_ptr, 0x80000000, 0x80000000, 2);
    mmu_map_new(simplemm.page_table_ptr, 0x40000000, 0x40000000, 2);
    mmu_map_new(simplemm.page_table_ptr, 0x00000000, 0x80000000, 2);
    print_s("mmu map done!\n");
    // pmp allow all
    mmu_pmp_allow_all();
    // clear satp
    asm volatile("csrw satp, %0" : : "r" (0));
    // set mpp to s-mode and mprov=1
    asm volatile("csrc mstatus, %0" : : "r" (0x1800)); // clear mpp to zero
    asm volatile("csrs mstatus, %0" : : "r" (0x20800));
    // set satp
    asm volatile("csrw satp, %0" : : "r" ( SATP_SET_PPN(simplemm.page_table_ptr) | SATP_SET_MODE(SATP_SV39) ));
    asm volatile("csrr %0, satp" : "=r" (tmp));
    asm volatile("fence rw,rw");
    asm volatile("fence.i");
    print_s("mmu init ok!\n");
}
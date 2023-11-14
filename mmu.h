struct simple_mmu {
    unsigned long mem_range;
    unsigned long page_table_ptr;
    unsigned long page_table_alloc_ptr;
};

struct sv39_pte {
    unsigned long V : 1; // valid
    unsigned long R : 1; // read
    unsigned long W : 1; // write
    unsigned long X : 1; // execute
    unsigned long U : 1; // user
    unsigned long G : 1; // global
    unsigned long A : 1; // access
    unsigned long D : 1; // dirty
    unsigned long RSW : 2; // reserved for use by supervisor softwar
    unsigned long PPN0 : 9;
    unsigned long PPN1 : 9;
    unsigned long PPN2 : 26;
    unsigned long reserved : 7;
    unsigned long PBMT : 2; // Svpbmt is not implemented, return 0
    unsigned long N : 1;
};

#define SV39_PTE_BIT_V 0x01
#define SV39_PTE_BIT_R 0x02
#define SV39_PTE_BIT_W 0x04
#define SV39_PTE_BIT_X 0x08
#define SV39_PTE_BIT_U 0x10
#define SV39_PTE_BIT_G 0x20
#define SV39_PTE_BIT_A 0x40
#define SV39_PTE_BIT_D 0x80

#define SV39_PPN0(pte) ( ((pte) >> 10) & 0x1ff)
#define SV39_PPN1(pte) ( ((pte) >> 19) & 0x1ff)
#define SV39_PPN2(pte) ( ((pte) >> 28) & 0x3ffffff)

#define SV39_SET_PA(pa) ( ((pa) & 0xfffffffffff000) >> 2)

#define SV39_GET_PA(pte) ( ((pte) & 0x3ffffffffffc00) << 2)

typedef enum {
    SATP_BARE = 0,
    SATP_SV39 = 8
} satp_mode;

#define SATP_SET_ASID(asid) ( (asid) << 44)
#define SATP_SET_MODE(mode) (((mode)*1ul) << 60)
#define SATP_SET_PPN(ppn)  ((ppn) >> 12)

void mmu_clear_mem(void *addr, unsigned long size);

unsigned long mmu_new_page_table_page();

void mmu_map_new(unsigned long base_addr, unsigned long vaddr, unsigned long paddr, int idx);

void mmu_pmp_allow_all();

void mmu_init();
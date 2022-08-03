/*
 * memmap.c
 * Memory mapper.
 *
 * 24 jul 2022 -- 00:45 -03
 * Notes:
 */

#include "risc.h"

static const char* THIS_FILE = "memmap.c";

void risc_map_memory(risc_vm_t* pCpu,
    risc_device_t* pDevice, VMWORD pBase, VMWORD pLimit, VMWORD pRemap) {
    if(pCpu->n_memregions >= RISC_TOTAL_MEMORY_REGIONS)
        risc_panic(pCpu,
            "unable to map memory:\n    device=%s (0x%x) base=0x%x limit=0x%x remap=0x%x\n"
            "not enough memory region slots",
            THIS_FILE, __LINE__,
            pDevice->name, pDevice, pBase, pLimit, pRemap);
    pCpu->memregions[pCpu->n_memregions].device = pDevice;
    pCpu->memregions[pCpu->n_memregions].base = pBase;
    pCpu->memregions[pCpu->n_memregions].limit = pLimit;
    pCpu->memregions[pCpu->n_memregions].remap = pRemap;
    pCpu->n_memregions++;
    RISC_LOG_INFO(pCpu, "mapping mem region, dev=%s (0x%x) base=0x%x limit=0x%x remap=0x%x",
        THIS_FILE, __LINE__,
        pDevice->name? pDevice->name: "NULL", pDevice, pBase, pLimit, pRemap);
}

risc_memreg_t* find_region(risc_vm_t* pCpu, VMWORD pAddr) {
    for(int32_t i = pCpu->n_memregions; i >= 0; i--) {
        if(pAddr >= pCpu->memregions[i].base && pAddr <= pCpu->memregions[i].limit) {
            return &pCpu->memregions[i];
        }
    }
    risc_panic(pCpu, "no memory region found for address: 0x%04x",
        THIS_FILE, __LINE__, pAddr);
    return NULL;
}

VMWORD risc_get_word(risc_vm_t* pCpu, VMWORD pAddr) {
    risc_memreg_t* reg = find_region(pCpu, pAddr);
    if(reg->device)
        return reg->device->read_word(pAddr - reg->remap);
    else
        return *((VMWORD*)(pCpu->ram + pAddr - reg->remap));
}

uint8_t risc_get_byte(risc_vm_t* pCpu, VMWORD pAddr) {
    risc_memreg_t* reg = find_region(pCpu, pAddr);
    if(reg->device)
        return reg->device->read_byte(pAddr - reg->remap);
    else
        return *(pCpu->ram + pAddr - reg->remap);
}

void risc_set_word(risc_vm_t* pCpu, VMWORD pVal, VMWORD pAddr) {
    risc_memreg_t* reg = find_region(pCpu, pAddr);
    if(reg->device)
        reg->device->write_word(pVal, pAddr - reg->remap);
    else
        *((VMWORD*)(pCpu->ram + pAddr - reg->remap)) = pVal;
}

void risc_set_byte(risc_vm_t* pCpu, uint8_t pVal, VMWORD pAddr) {
    risc_memreg_t* reg = find_region(pCpu, pAddr);
    if(reg->device)
        reg->device->write_byte(pVal, pAddr - reg->remap);
    else
        *(pCpu->ram + pAddr - reg->remap) = pVal;
}

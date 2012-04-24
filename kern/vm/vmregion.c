#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <array.h>
#include <addrspace.h>
#include <vm.h>
#include <vm/region.h>
#include <vm/swap.h>
#include <machine/coremap.h>

DEFARRAY_BYTYPE( vm_page_array, struct vm_page, /* no inline */ );

struct vm_region *
vm_region_create( size_t npages ) {
	int 			res;
	struct vm_region	*vmr;
	unsigned		i;

	//attempt to create the vm_region
	vmr = kmalloc( sizeof( struct vm_region ) );
	if( vmr == NULL )
		return NULL;

	
	//create the vm_pages.
	vmr->vmr_pages = vm_page_array_create();
	if( vmr->vmr_pages == NULL ) {
		kfree( vmr );
		return NULL;
	}

	//set the base address to point to an invalid virtual address.
	vmr->vmr_base = INVALID_VADDR;

	//adjust the array to hold npages.
	res = vm_page_array_setsize( vmr->vmr_pages, npages );
	if( res ) {
		vm_page_array_destroy( vmr->vmr_pages );
		kfree( vmr );
		return NULL;
	}

	//initialize all the pages to NULL.
	for( i = 0; i < npages; ++i ) {
		vm_page_array_set( vmr->vmr_pages, i, NULL );
	}

	return vmr;
}


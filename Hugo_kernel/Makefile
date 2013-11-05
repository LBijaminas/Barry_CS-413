BOOT_LOADER_DIR=boot_loader
KERNEL_DIR=kernel

image: .PHONY
	cat $(BOOT_LOADER_DIR)/boot_sect.bin $(KERNEL_DIR)/kernel.bin > os-image

.PHONY: kernel

kernel: 
	$(MAKE) -C $(KERNEL_DIR)

clean:
	rm boot_loader/*.o boot_loader/*.bin  kernel/*.o kernel/*.bin os-image 

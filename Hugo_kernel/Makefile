BOOT_LOADER_DIR=boot_loader
KERNEL_DIR=kernel

.PHONY: kernel

kernel: 
	$(MAKE) -C $(KERNEL_DIR)

image: .PHONY
	cat $(BOOT_LOADER_DIR)/boot_sect.bin $(KERNEL_DIR)/kernel.bin > os-image

clean:
	rm $(KERNEL_DIR)/*.o $(KERNEL_DIR)/*.bin os-image 

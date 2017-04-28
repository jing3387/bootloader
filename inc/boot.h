struct memory_map {
	UINTN memory_map_size;
	EFI_MEMORY_DESCRIPTOR *memory_map;
	UINTN map_key;
	UINTN descriptor_size;
	UINT32 descriptor_version;
};

EFI_STATUS get_memory_map(EFI_SYSTEM_TABLE *, struct memory_map *);

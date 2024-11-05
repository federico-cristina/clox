clox_add_unit_test(chunk
	SOURCES "test_chunk.c"
	DEPENDS vm
)

clox_add_unit_test(opcode
	SOURCES "test_opcode.c"
	DEPENDS vm
)

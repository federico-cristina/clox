clox_add_unit_test(buffer
	SOURCES "test_buffer.c"
	DEPENDS vm
)

clox_add_unit_test(opcode
	SOURCES "test_opcode.c"
	DEPENDS vm
)

clox_add_unit_test(opcode
	SOURCES "test_opcode.c"
	DEPENDS vm
)

clox_add_unit_test(code-block
	SOURCES "test_code_block.c"
	DEPENDS vm
	TEST
)

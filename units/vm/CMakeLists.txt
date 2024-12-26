clox_add_unit_test(code
	SOURCES "test_code.c"
	DEPENDS vm
)

clox_add_unit_test(code-block
	SOURCES "test_code_block.c"
	DEPENDS vm
	TEST
)

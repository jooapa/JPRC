#include "ATRC.Test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_read_variable(C_PATRC_FD fd) {
    const char* varname = "test_variable2";
    const char* value = ReadVariable(fd, varname);
    if (value == NULL || strcmp(value, "") == 0) {
        printf("[FAIL] ReadVariable: Failed to read variable '%s'\n", varname);
    }
    else {
        printf("[PASS] ReadVariable: Value of '%s' is '%s'\n", varname, value);
    }
}

void test_block_operations(C_PATRC_FD fd) {
    const char* blockname = "test_block";
    if (AddBlock(fd, blockname)) {
        printf("[PASS] AddBlock: Block '%s' added successfully\n", blockname);
    }
    else {
        printf("[FAIL] AddBlock: Failed to add block '%s'\n", blockname);
    }

    if (DoesExistBlock(fd, blockname)) {
        printf("[PASS] DoesExistBlock: Block '%s' exists\n", blockname);
    }
    else {
        printf("[FAIL] DoesExistBlock: Block '%s' does not exist\n", blockname);
    }

    if (RemoveBlock(fd, blockname)) {
        printf("[PASS] RemoveBlock: Block '%s' removed successfully\n", blockname);
    }
    else {
        printf("[FAIL] RemoveBlock: Failed to remove block '%s'\n", blockname);
    }
}

void test_variable_operations(C_PATRC_FD fd) {
    const char* varname = "test_variable";
    const char* value = "test_value";
    

    if (AddVariable(fd, varname, value)) {
        printf("[PASS] AddVariable: Variable '%s' added successfully\n", varname);
    }
    else {
        printf("[FAIL] AddVariable: Failed to add variable '%s'\n", varname);
    }

    if (DoesExistVariable(fd, varname)) {
        printf("[PASS] DoesExistVariable: Variable '%s' exists\n", varname);
    }
    else {
        printf("[FAIL] DoesExistVariable: Variable '%s' does not exist\n", varname);
    }

    const char* new_value = "modified_value";
    if (ModifyVariable(fd, varname, new_value)) {
        printf("[PASS] ModifyVariable: Variable '%s' modified successfully\n", varname);
    }
    else {
        printf("[FAIL] ModifyVariable: Failed to modify variable '%s'\n", varname);
    }

    if (RemoveVariable(fd, varname)) {
        printf("[PASS] RemoveVariable: Variable '%s' removed successfully\n", varname);
        
    }
    else {
        printf("[FAIL] RemoveVariable: Failed to remove variable '%s'\n", varname);
    }
}

void test_key_operations(C_PATRC_FD fd) {
    const char* block = "test_block";
    const char* key = "test_key";
    const char* value = "test_value";

    AddBlock(fd, block);

    if (AddKey(fd, block, key, value)) {
        printf("[PASS] AddKey: Key '%s' added to block '%s' successfully\n", key, block);
    }
    else {
        printf("[FAIL] AddKey: Failed to add key '%s' to block '%s'\n", key, block);
    }

    if (DoesExistKey(fd, block, key)) {
        printf("[PASS] DoesExistKey: Key '%s' exists in block '%s'\n", key, block);
    }
    else {
        printf("[FAIL] DoesExistKey: Key '%s' does not exist in block '%s'\n", key, block);
    }

    const char* new_value = "modified_value";
    if (ModifyKey(fd, block, key, new_value)) {
        printf("[PASS] ModifyKey: Key '%s' in block '%s' modified successfully\n", key, block);
    }
    else {
        printf("[FAIL] ModifyKey: Failed to modify key '%s' in block '%s'\n", key, block);
    }

    if (RemoveKey(fd, block, key)) {
        printf("[PASS] RemoveKey: Key '%s' removed from block '%s' successfully\n", key, block);
    }
    else {
        printf("[FAIL] RemoveKey: Failed to remove key '%s' from block '%s'\n", key, block);
    }

    RemoveBlock(fd, block);
}

void test_insert(C_PATRC_FD fd) {
    
	const char* line2 = ReadVariable(fd, "test");
	const char* args2[] = { "test", "test", NULL };
	char* res = InsertVar_S(line2, args2);
    if (res == NULL) {
		printf("InsertVar_S: Failed to insert variables\n");
		return;
    }
    printf("InsertVar_S: '%s'\n", res);
    free(res);
}

int main(void) {
    /*
    printf("Running ATRC API tests...\n");

    C_PATRC_FD fd = Create_Empty_ATRC_FD();
    if (fd == NULL) {
        printf("[FAIL] Create_Empty_ATRC_FD: Failed to create ATRC_FD\n");
        return 1;
    }
    printf("[PASS] Create_Empty_ATRC_FD: Successfully created ATRC_FD\n");

    // Test file reading
    if (Read(fd, "test.atrc", ATRC_READ_ONLY)) {
        printf("[PASS] Read: Successfully read file 'test.atrc'\n");
    }
    else {
        printf("[FAIL] Read: Failed to read file 'test.atrc'\n");
        Destroy_ATRC_FD(fd);
		return 1;
    }

    Destroy_ATRC_FD(fd);
    return 0;
    
    // Test Variable Operations
    test_variable_operations(fd);

    // Test Block Operations
    test_block_operations(fd);

    // Test Key Operations
    test_key_operations(fd);

    // Test Reading Variables
    test_read_variable(fd);

	// Test Insert
    test_insert(fd);

    // Clean up
    Destroy_ATRC_FD(fd);
    printf("[PASS] Destroy_ATRC_FD: Successfully destroyed ATRC_FD\n");

    printf("Tests completed.\n");









    C_PATRC_FD filedata = Create_Empty_ATRC_FD();
    if (filedata == NULL) {
        printf("[FAIL] Create_Empty_ATRC_FD: Failed to create ATRC_FD\n");
        return 1;
    }

    if (!Read(filedata, "test.atrc", ATRC_READ_ONLY)) {
        printf("[FAIL] Read: Failed to read file 'test.atrc'\n");
        Destroy_ATRC_FD(filedata);
        return 1;
    }

    const char* varname = "test_variable";
    const char* value = ReadVariable(filedata, varname);
    if(value == NULL){
        printf("[FAIL] ReadVariable: Failed to read variable '%s'\n", varname);
    } else {
        printf("[PASS] ReadVariable: Value of '%s' is '%s'\n", varname, value);
    }

    const char* blockname = "test_block";
    if (AddBlock(filedata, blockname)) {
        printf("[PASS] AddBlock: Block '%s' added successfully\n", blockname);
    } else {
        printf("[FAIL] AddBlock: Failed to add block '%s'\n", blockname);
    }

    if (DoesExistBlock(filedata, blockname)) {
        printf("[PASS] DoesExistBlock: Block '%s' exists\n", blockname);
    } else {
        printf("[FAIL] DoesExistBlock: Block '%s' does not exist\n", blockname);
    }

    if (RemoveBlock(filedata, blockname)) {
        printf("[PASS] RemoveBlock: Block '%s' removed successfully\n", blockname);
    } else {
        printf("[FAIL] RemoveBlock: Failed to remove block '%s'\n", blockname);
    }

    Destroy_ATRC_FD(filedata);
    */

	cpp_main();

    return 0;
}

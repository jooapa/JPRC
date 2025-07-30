package main

import (
	"fmt"
	"log"

	"atrc"
)

func main() {
	fmt.Println("ATRC Go Wrapper Example")
	fmt.Println("========================")

	// Example 1: Create an empty ATRC file
	fmt.Println("\n1. Creating empty ATRC file...")
	atrcFile, err := atrc.NewEmpty()
	if err != nil {
		log.Fatalf("Failed to create empty ATRC file: %v", err)
	}
	defer atrcFile.Close()

	// Example 2: Add some variables
	fmt.Println("\n2. Adding variables...")
	if !atrcFile.AddVariable("app_name", "MyApplication") {
		log.Println("Failed to add app_name variable")
	}
	if !atrcFile.AddVariable("version", "1.0.0") {
		log.Println("Failed to add version variable")
	}
	if !atrcFile.AddVariable("debug", "true") {
		log.Println("Failed to add debug variable")
	}

	// Example 3: Add a block with keys
	fmt.Println("\n3. Adding block with keys...")
	if !atrcFile.AddBlock("database") {
		log.Println("Failed to add database block")
	}
	if !atrcFile.AddKey("database", "host", "localhost") {
		log.Println("Failed to add host key")
	}
	if !atrcFile.AddKey("database", "port", "5432") {
		log.Println("Failed to add port key")
	}
	if !atrcFile.AddKey("database", "name", "myapp_db") {
		log.Println("Failed to add name key")
	}

	// Example 4: Read variables
	fmt.Println("\n4. Reading variables...")
	if appName, exists := atrcFile.ReadVariable("app_name"); exists {
		fmt.Printf("App Name: %s\n", appName)
	} else {
		fmt.Println("App name not found")
	}

	if version, exists := atrcFile.ReadVariable("version"); exists {
		fmt.Printf("Version: %s\n", version)
	} else {
		fmt.Println("Version not found")
	}

	// Example 5: Read keys from block
	fmt.Println("\n5. Reading keys from database block...")
	if host, exists := atrcFile.ReadKey("database", "host"); exists {
		fmt.Printf("Database Host: %s\n", host)
	} else {
		fmt.Println("Database host not found")
	}

	if port, exists := atrcFile.ReadKey("database", "port"); exists {
		fmt.Printf("Database Port: %s\n", port)
	} else {
		fmt.Println("Database port not found")
	}

	// Example 6: Check existence
	fmt.Println("\n6. Checking existence...")
	fmt.Printf("Variable 'app_name' exists: %t\n", atrcFile.DoesExistVariable("app_name"))
	fmt.Printf("Variable 'non_existent' exists: %t\n", atrcFile.DoesExistVariable("non_existent"))
	fmt.Printf("Block 'database' exists: %t\n", atrcFile.DoesExistBlock("database"))
	fmt.Printf("Key 'host' in 'database' exists: %t\n", atrcFile.DoesExistKey("database", "host"))

	// Example 7: Modify values
	fmt.Println("\n7. Modifying values...")
	if atrcFile.ModifyVariable("version", "1.1.0") {
		fmt.Println("Version updated to 1.1.0")
	}
	if atrcFile.ModifyKey("database", "port", "3306") {
		fmt.Println("Database port updated to 3306")
	}

	// Example 8: Using standard library functions
	fmt.Println("\n8. Using standard library functions...")

	// Convert strings using ATRC's native conversion functions
	debugValue, _ := atrcFile.ReadVariable("debug")
	isDebug := atrc.ToBool(debugValue)
	fmt.Printf("Debug mode: %t\n", isDebug)

	portValue, _ := atrcFile.ReadKey("database", "port")
	portNumber := atrc.ToUint64(portValue)
	fmt.Printf("Port as number: %d\n", portNumber)

	// Parse a list
	listValue := "item1, item2, item3, item4"
	items := atrc.ToList(listValue, ',')
	fmt.Printf("List items: %v\n", items)

	// Parse mathematical expression
	mathExpr := "2 + 3 * 4"
	result := atrc.ParseMathExp(mathExpr)
	fmt.Printf("Math expression '%s' = %f\n", mathExpr, result)

	// Example 9: Variable injection
	fmt.Println("\n9. Variable injection...")
	template := "Welcome %*% to version %*%!"
	args := []string{"User", "1.1.0"}
	if injected, err := atrcFile.InsertVarS(template, args); err == nil {
		fmt.Printf("Injected: %s\n", injected)
	} else {
		fmt.Printf("Failed to inject variables: %v\n", err)
	}

	// Example 10: Copy ATRC file
	fmt.Println("\n10. Copying ATRC file...")
	copiedFile, err := atrcFile.Copy()
	if err != nil {
		log.Printf("Failed to copy ATRC file: %v", err)
	} else {
		defer copiedFile.Close()
		fmt.Println("ATRC file copied successfully")

		// Verify the copy has the same data
		if copiedAppName, exists := copiedFile.ReadVariable("app_name"); exists {
			fmt.Printf("Copied file app name: %s\n", copiedAppName)
		}
	}

	fmt.Println("\nExample completed successfully!")
}

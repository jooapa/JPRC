package atrc

/*
#cgo CFLAGS: -IATRC/include
#cgo LDFLAGS: -LATRC/lib -lATRC
#cgo windows LDFLAGS: -LATRC/lib -lATRC

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "ATRC.h"
*/
import "C"
import (
	"fmt"
	"runtime"
	"strconv"
	"strings"
	"unsafe"
)

// ReadMode represents the different read modes for ATRC files
type ReadMode int

const (
	// ReadOnly opens the file in read-only mode
	ReadOnly ReadMode = iota
	// CreateRead creates the file if it doesn't exist, then opens for reading
	CreateRead
	// ForceRead forces reading of the file even if there are issues
	ForceRead
)

// ATRC represents a managed wrapper for an ATRC file descriptor
type ATRC struct {
	fd *C.ATRC_FD
}

// NewEmpty creates a new empty ATRC file (no file path associated)
func NewEmpty() (*ATRC, error) {
	fd := C.Create_Empty_ATRC_FD()
	if fd == nil {
		return nil, fmt.Errorf("failed to create empty ATRC file descriptor")
	}

	atrc := &ATRC{fd: fd}
	runtime.SetFinalizer(atrc, (*ATRC).finalize)
	return atrc, nil
}

// New creates a new ATRC file using a file path and read mode
func New(filename string, mode ReadMode) (*ATRC, error) {
	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))

	fd := C.Create_ATRC_FD(cFilename, C.ReadMode(mode))
	if fd == nil {
		return nil, fmt.Errorf("failed to create ATRC file descriptor for file '%s'", filename)
	}

	atrc := &ATRC{fd: fd}
	runtime.SetFinalizer(atrc, (*ATRC).finalize)
	return atrc, nil
}

// Close frees the ATRC file descriptor and cleans up resources
func (a *ATRC) Close() {
	if a.fd != nil {
		C.Destroy_ATRC_FD(a.fd)
		a.fd = nil
		runtime.SetFinalizer(a, nil)
	}
}

// finalize is called by the garbage collector to ensure cleanup
func (a *ATRC) finalize() {
	a.Close()
}

// SetAutoSave sets whether the ATRC file should automatically save changes
func (a *ATRC) SetAutoSave(autoSave bool) {
	if a.fd != nil {
		a.fd.AutoSave = C.bool(autoSave)
	}
}

// GetAutoSave retrieves the current autosave status of the ATRC file
func (a *ATRC) GetAutoSave() bool {
	if a.fd != nil {
		return bool(a.fd.AutoSave)
	}
	return false
}

// SetWriteCheck sets whether the ATRC file should perform write checks
func (a *ATRC) SetWriteCheck(writeCheck bool) {
	if a.fd != nil {
		a.fd.Writecheck = C.bool(writeCheck)
	}
}

// GetWriteCheck retrieves the current write check status of the ATRC file
func (a *ATRC) GetWriteCheck() bool {
	if a.fd != nil {
		return bool(a.fd.Writecheck)
	}
	return false
}

// Read reads an ATRC file from the specified filename with the given read mode
func (a *ATRC) Read(filename string, mode ReadMode) bool {
	if a.fd == nil {
		return false
	}

	cFilename := C.CString(filename)
	defer C.free(unsafe.Pointer(cFilename))

	return bool(C.Read(a.fd, cFilename, C.ReadMode(mode)))
}

// ReadVariable reads a variable from the ATRC file
// If the variable does not exist, it returns an empty string and false
func (a *ATRC) ReadVariable(name string) (string, bool) {
	if a.fd == nil {
		return "", false
	}

	cName := C.CString(name)
	defer C.free(unsafe.Pointer(cName))

	ptr := C.ReadVariable(a.fd, cName)
	if ptr == nil {
		return "", false
	}
	defer C.ATRC_FREE_MEMORY(unsafe.Pointer(ptr))

	return C.GoString(ptr), true
}

// ReadKey reads a key from a specified block in the ATRC file
// If the key does not exist, it returns an empty string and false
func (a *ATRC) ReadKey(block, key string) (string, bool) {
	if a.fd == nil {
		return "", false
	}

	cBlock := C.CString(block)
	cKey := C.CString(key)
	defer C.free(unsafe.Pointer(cBlock))
	defer C.free(unsafe.Pointer(cKey))

	ptr := C.ReadKey(a.fd, cBlock, cKey)
	if ptr == nil {
		return "", false
	}
	defer C.ATRC_FREE_MEMORY(unsafe.Pointer(ptr))

	return C.GoString(ptr), true
}

// DoesExistBlock checks if a block exists in the ATRC file
func (a *ATRC) DoesExistBlock(block string) bool {
	if a.fd == nil {
		return false
	}

	cBlock := C.CString(block)
	defer C.free(unsafe.Pointer(cBlock))

	return bool(C.DoesExistBlock(a.fd, cBlock))
}

// DoesExistVariable checks if a variable exists in the ATRC file
func (a *ATRC) DoesExistVariable(name string) bool {
	if a.fd == nil {
		return false
	}

	cName := C.CString(name)
	defer C.free(unsafe.Pointer(cName))

	return bool(C.DoesExistVariable(a.fd, cName))
}

// DoesExistKey checks if a key exists in a specified block of the ATRC file
func (a *ATRC) DoesExistKey(block, key string) bool {
	if a.fd == nil {
		return false
	}

	cBlock := C.CString(block)
	cKey := C.CString(key)
	defer C.free(unsafe.Pointer(cBlock))
	defer C.free(unsafe.Pointer(cKey))

	return bool(C.DoesExistKey(a.fd, cBlock, cKey))
}

// IsPublic checks if a variable is public in the ATRC file
func (a *ATRC) IsPublic(varname string) bool {
	if a.fd == nil {
		return false
	}

	cVarname := C.CString(varname)
	defer C.free(unsafe.Pointer(cVarname))

	return bool(C.IsPublic(a.fd, cVarname))
}

// InsertVarS inserts variables into the specified line with the given arguments
func (a *ATRC) InsertVarS(line string, args []string) (string, error) {
	if a.fd == nil {
		return "", fmt.Errorf("invalid file descriptor")
	}

	cLine := C.CString(line)
	defer C.free(unsafe.Pointer(cLine))

	// Create null-terminated array of C strings
	cArgs := make([]*C.char, len(args)+1)
	for i, arg := range args {
		cArgs[i] = C.CString(arg)
		defer C.free(unsafe.Pointer(cArgs[i]))
	}
	cArgs[len(args)] = nil // Null terminator

	// Create array pointer
	var argsPtr **C.char
	if len(cArgs) > 0 {
		argsPtr = &cArgs[0]
	}

	ptr := C.InsertVar_S(cLine, argsPtr)
	if ptr == nil {
		return "", fmt.Errorf("failed to insert variables")
	}
	defer C.ATRC_FREE_MEMORY(unsafe.Pointer(ptr))

	return C.GoString(ptr), nil
}

// AddBlock adds a new block to the ATRC file
func (a *ATRC) AddBlock(blockname string) bool {
	if a.fd == nil {
		return false
	}

	cBlockname := C.CString(blockname)
	defer C.free(unsafe.Pointer(cBlockname))

	return bool(C.AddBlock(a.fd, cBlockname))
}

// RemoveBlock removes a block from the ATRC file
func (a *ATRC) RemoveBlock(blockname string) bool {
	if a.fd == nil {
		return false
	}

	cBlockname := C.CString(blockname)
	defer C.free(unsafe.Pointer(cBlockname))

	return bool(C.RemoveBlock(a.fd, cBlockname))
}

// AddVariable adds a new variable to the ATRC file
func (a *ATRC) AddVariable(varname, value string) bool {
	if a.fd == nil {
		return false
	}

	cVarname := C.CString(varname)
	cValue := C.CString(value)
	defer C.free(unsafe.Pointer(cVarname))
	defer C.free(unsafe.Pointer(cValue))

	return bool(C.AddVariable(a.fd, cVarname, cValue))
}

// RemoveVariable removes a variable from the ATRC file
func (a *ATRC) RemoveVariable(varname string) bool {
	if a.fd == nil {
		return false
	}

	cVarname := C.CString(varname)
	defer C.free(unsafe.Pointer(cVarname))

	return bool(C.RemoveVariable(a.fd, cVarname))
}

// ModifyVariable modifies the value of an existing variable in the ATRC file
func (a *ATRC) ModifyVariable(varname, value string) bool {
	if a.fd == nil {
		return false
	}

	cVarname := C.CString(varname)
	cValue := C.CString(value)
	defer C.free(unsafe.Pointer(cVarname))
	defer C.free(unsafe.Pointer(cValue))

	return bool(C.ModifyVariable(a.fd, cVarname, cValue))
}

// AddKey adds a new key to a specified block in the ATRC file
func (a *ATRC) AddKey(block, key, value string) bool {
	if a.fd == nil {
		return false
	}

	cBlock := C.CString(block)
	cKey := C.CString(key)
	cValue := C.CString(value)
	defer C.free(unsafe.Pointer(cBlock))
	defer C.free(unsafe.Pointer(cKey))
	defer C.free(unsafe.Pointer(cValue))

	return bool(C.AddKey(a.fd, cBlock, cKey, cValue))
}

// RemoveKey removes a key from a specified block in the ATRC file
func (a *ATRC) RemoveKey(block, key string) bool {
	if a.fd == nil {
		return false
	}

	cBlock := C.CString(block)
	cKey := C.CString(key)
	defer C.free(unsafe.Pointer(cBlock))
	defer C.free(unsafe.Pointer(cKey))

	return bool(C.RemoveKey(a.fd, cBlock, cKey))
}

// ModifyKey modifies the value of an existing key in a specified block in the ATRC file
func (a *ATRC) ModifyKey(block, key, newValue string) bool {
	if a.fd == nil {
		return false
	}

	cBlock := C.CString(block)
	cKey := C.CString(key)
	cNewValue := C.CString(newValue)
	defer C.free(unsafe.Pointer(cBlock))
	defer C.free(unsafe.Pointer(cKey))
	defer C.free(unsafe.Pointer(cNewValue))

	return bool(C.ModifyKey(a.fd, cBlock, cKey, cNewValue))
}

// WriteCommentToTop writes a comment to the top of the ATRC file
func (a *ATRC) WriteCommentToTop(comment string) bool {
	if a.fd == nil {
		return false
	}

	cComment := C.CString(comment)
	defer C.free(unsafe.Pointer(cComment))

	return bool(C.WriteCommentToTop(a.fd, cComment))
}

// WriteCommentToBottom writes a comment to the bottom of the ATRC file
func (a *ATRC) WriteCommentToBottom(comment string) bool {
	if a.fd == nil {
		return false
	}

	cComment := C.CString(comment)
	defer C.free(unsafe.Pointer(cComment))

	return bool(C.WriteCommentToBottom(a.fd, cComment))
}

// GetEnumValue retrieves the value of an enum from a specified block and key in the ATRC file
// If the block or key does not exist, it returns 0 and false
func (a *ATRC) GetEnumValue(block, key string) (uint64, bool) {
	if a.fd == nil {
		return 0, false
	}

	cBlock := C.CString(block)
	cKey := C.CString(key)
	defer C.free(unsafe.Pointer(cBlock))
	defer C.free(unsafe.Pointer(cKey))

	value := uint64(C.GetEnumValue(a.fd, cBlock, cKey))
	// Check if this is the error value (would be implementation dependent)
	return value, true
}

// Copy creates a copy of an ATRC file descriptor
func (a *ATRC) Copy() (*ATRC, error) {
	if a.fd == nil {
		return nil, fmt.Errorf("cannot copy from invalid file descriptor")
	}

	newFd := C.Copy_ATRC_FD(a.fd)
	if newFd == nil {
		return nil, fmt.Errorf("failed to copy ATRC file descriptor")
	}

	newAtrc := &ATRC{fd: newFd}
	runtime.SetFinalizer(newAtrc, (*ATRC).finalize)
	return newAtrc, nil
}

// STD provides ATRC standard library functions for common operations
type STD struct{}

// NewSTD creates a new instance of the ATRC standard library
func NewSTD() *STD {
	return &STD{}
}

// ToList converts a string to a slice of strings, splitting by the specified separator
func (s *STD) ToList(value string, separator rune) []string {
	if value == "" {
		return []string{}
	}

	parts := strings.Split(value, string(separator))
	result := make([]string, len(parts))
	for i, part := range parts {
		result[i] = strings.TrimSpace(part)
	}
	return result
}

// ToBool converts a string to a boolean value using the native ATRC library
func (s *STD) ToBool(value string) bool {
	cValue := C.CString(value)
	defer C.free(unsafe.Pointer(cValue))

	return bool(C.atrc_to_bool(cValue))
}

// ToUint64 converts a string to an unsigned 64-bit integer using the native ATRC library
func (s *STD) ToUint64(value string) uint64 {
	cValue := C.CString(value)
	defer C.free(unsafe.Pointer(cValue))

	return uint64(C.atrc_to_uint64_t(cValue))
}

// ToInt64 converts a string to a signed 64-bit integer using the native ATRC library
func (s *STD) ToInt64(value string) int64 {
	cValue := C.CString(value)
	defer C.free(unsafe.Pointer(cValue))

	return int64(C.atrc_to_int64_t(cValue))
}

// ToDouble converts a string to a double using the native ATRC library
func (s *STD) ToDouble(value string) float64 {
	cValue := C.CString(value)
	defer C.free(unsafe.Pointer(cValue))

	return float64(C.atrc_to_double(cValue))
}

// ParseMathExp parses a mathematical expression from a string and returns the result as a float64
func (s *STD) ParseMathExp(value string) float64 {
	cValue := C.CString(value)
	defer C.free(unsafe.Pointer(cValue))

	return float64(C.atrc_math_exp(cValue))
}

// Convenience functions for direct use without STD instance

// ToBool converts a string to a boolean value
func ToBool(value string) bool {
	return NewSTD().ToBool(value)
}

// ToUint64 converts a string to an unsigned 64-bit integer
func ToUint64(value string) uint64 {
	return NewSTD().ToUint64(value)
}

// ToInt64 converts a string to a signed 64-bit integer
func ToInt64(value string) int64 {
	return NewSTD().ToInt64(value)
}

// ToDouble converts a string to a double
func ToDouble(value string) float64 {
	return NewSTD().ToDouble(value)
}

// ParseMathExp parses a mathematical expression from a string
func ParseMathExp(value string) float64 {
	return NewSTD().ParseMathExp(value)
}

// ToList converts a string to a slice of strings, splitting by comma by default
func ToList(value string, separator ...rune) []string {
	sep := ','
	if len(separator) > 0 {
		sep = separator[0]
	}
	return NewSTD().ToList(value, sep)
}

// Helper functions for common conversions

// ToInt converts a string to int using Go's standard library
func ToInt(value string) (int, error) {
	return strconv.Atoi(strings.TrimSpace(value))
}

// ToFloat converts a string to float64 using Go's standard library
func ToFloat(value string) (float64, error) {
	return strconv.ParseFloat(strings.TrimSpace(value), 64)
}

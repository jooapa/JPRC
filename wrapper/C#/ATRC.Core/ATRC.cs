using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace ATRC {
    public static class ATRC {
        [StructLayout(LayoutKind.Sequential)]
        public struct Variable {
            public IntPtr Name;
            public IntPtr Value;
            public bool IsPublic;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Key {
            public IntPtr Name;
            public IntPtr Value;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct Block {
            public IntPtr Name;
            public IntPtr Keys;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct ATRCFileData {
            public IntPtr Variables;
            public IntPtr Blocks;
            public IntPtr Filename;
            public bool AutoSave;
            
        }

        // DLL Imports for C++ Functions
        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Read(string path);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ReadVariable(IntPtr filedata, string varname);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr ReadKey(IntPtr filedata, string block, string key);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool DoesExistBlock(IntPtr filedata, string block);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool DoesExistVariable(IntPtr filedata, string varname);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool DoesExistKey(IntPtr filedata, string block, string key);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool IsPublic(IntPtr filedata, string varname);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void InsertVar(IntPtr filedata, string line, IntPtr args);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddBlock(IntPtr filedata, string blockname);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RemoveBlock(IntPtr filedata, string blockname);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddVariable(IntPtr filedata, string varname, string value);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RemoveVariable(IntPtr filedata, string varname);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ModifyVariable(IntPtr filedata, string varname, string value);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddKey(IntPtr filedata, string block, string key, string value);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RemoveKey(IntPtr filedata, string block, string key);

        [DllImport("ATRC.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void ModifyKey(IntPtr filedata, string block, string key, string value);

        // Helper methods for marshaling
        public static string MarshalString(IntPtr ptr) {
            return Marshal.PtrToStringAnsi(ptr);
        }

        public static string GetVariable(IntPtr filedata, string varname) {
            IntPtr resultPtr = ReadVariable(filedata, varname);
            return MarshalString(resultPtr);
        }

        public static string GetKey(IntPtr filedata, string block, string key) {
            IntPtr resultPtr = ReadKey(filedata, block, key);
            return MarshalString(resultPtr);
        }
    }
}

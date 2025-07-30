using System;
using System.Runtime.InteropServices;

namespace ATRC;

/// <summary>
/// Managed wrapper for an ATRC file descriptor that automatically handles memory cleanup.
/// </summary>
public class ATRC : IDisposable
{

    private IntPtr _fd = IntPtr.Zero;
    private bool _disposed;

    /// <summary>
    /// Initializes a new empty ATRC file (no file path associated).
    /// </summary>
    public ATRC()
    {
        _fd = ATRC_Native.Create_Empty_ATRC_FD();
        if (_fd == IntPtr.Zero)
            throw new InvalidOperationException("Failed to create empty ATRC file descriptor.");
    }

    /// <summary>
    /// Initializes a new ATRC file using a file path and read mode.
    /// </summary>
    /// <param name="filename">Path to the file.</param>
    /// <param name="mode">Read mode (e.g., ReadOnly, CreateRead, ForceRead).</param>
    public ATRC(string filename, ReadMode mode)
    {
        _fd = ATRC_Native.Create_ATRC_FD(filename, mode);
        if (_fd == IntPtr.Zero)
            throw new InvalidOperationException($"Failed to create ATRC file descriptor for file '{filename}'.");
    }


    public string? ReadVariable(string name)
    {
        string? res = string.Empty;
        if (_fd != IntPtr.Zero)
        {
            IntPtr ptr = ATRC_Native.ReadVariable(_fd, name);
            if (ptr != IntPtr.Zero)
            {
                if(ptr == IntPtr.Zero)
                {
                    throw new InvalidOperationException($"Failed to read variable '{name}' from ATRC file.");
                }
                res = Marshal.PtrToStringUTF8(ptr);
                ATRC.ATRC_FREE_MEMORY(ptr); // Assuming the native lib allocates and expects you to free
            }

        }

        return res;
    }

    public static void ATRC_FREE_MEMORY(IntPtr ptr)
    {
        if (ptr != IntPtr.Zero)
        {
            ATRC_Native.ATRC_FREE_MEMORY(ptr);
        }
    }

    /// <summary>
    /// Gets the raw native pointer to the ATRC_FD structure.
    /// Use with caution.
    /// </summary>
    public IntPtr Handle => _fd;

    /// <summary>
    /// Frees unmanaged resources.
    /// </summary>
    public void Dispose()
    {
        Dispose(disposing: true);
        GC.SuppressFinalize(this);
    }

    private void Dispose(bool disposing)
    {
        if (!_disposed)
        {
            if (_fd != IntPtr.Zero)
            {
                ATRC_Native.Destroy_ATRC_FD(_fd);
                _fd = IntPtr.Zero;
            }

            _disposed = true;
        }
    }

    ~ATRC()
    {
        Dispose(disposing: false);
    }
}

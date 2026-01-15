A **file descriptor** is an integer that uniquely identifies an open file, socket, or other I/O resource within a process. 
It abstracts the underlying file system, providing a consistent interface for reading from and writing to resources.

Each process in Unix/Linux maintains a **file descriptor table**, which is an array-like structure that stores references to the resources (files, sockets, etc.) 
the process has opened. This is **local to a process**.

The ***system-wide file table** is a kernel data structure that holds some metadata about all open files, including information about their location, access mode, current position (file offset), and more. Each entry in the file table corresponds to a file descriptor, providing shared information across multiple processes.

The **inode (Index Node)** is a data structure that contains metadata about a file, such as its size, permissions, ownership, timestamps, and pointers to the data blocks on disk.

An inode does not store the file name but rather points to the actual file’s data blocks on the disk. Multiple processes may reference the same inode, allowing them to access the same file data. The inode table is stored on disk and contains all the inode structures for the files managed by the system.

### Dictionary

1. A network interface is the boundary between the kernel’s networking stack and the physical hardware that transmits data over the network.
2. A socket is a software abstraction for communication, while a network interface is the boundary where the kernel sends data to the physical network hardware.
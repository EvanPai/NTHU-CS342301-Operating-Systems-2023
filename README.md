# NTHU-CS342301-Operating-Systems-2023

## MP1 - System Call
### Task
Implement four I/O system calls in NachOS.
- OpenFileId Open(char *name);
- int Write(char *buffer, int size, OpenFileId id);
- int Read(char *buffer, int size, OpenFileId id);
- int Close(OpenFileId id);

## MP2 - Multi-Programming
### Task
Implement page table for NachOS.

## MP3 - CPU scheduling
### Task
- mplement a multilevel feedback queue scheduler with aging mechanism.
- Add a command line argument -ep for nachos to initialize priority of process.
- Add a debugging flag z and use the DEBUG('z', expr) macro (defined in debug.h) to print messages.

## MP4 - File System
The two version of MP4 show the implementation of using linked scheme and index scheme for file system.
### Task
- Implement five system calls to to support file I/O system call.
  - int Create(char *name, int size);
  - OpenFileId Open(char *name);
  - int Read(char *buf, int size, OpenFileId id);
  - int Write(char *buf, int size, OpenFileId id);
  - int Close(OpenFileId id);
- Enhance the FS to let it support up to 32KB file size
- Implement the subdirectory structure.
- Support up to 64 files/subdirectories per directory.

### Bonus
In the NachOS-4.0_MP4_index, the bonus below were implemented. 
- Enhance the NachOS to support even larger file size.
  - Extend the disk from 128KB to 64MB.
  - Support up to 64 MB single file.
- Multi-level header size.
  - Show that smaller file can have smaller header size.
  - Implement at least 3 different size of headers for different size of files.

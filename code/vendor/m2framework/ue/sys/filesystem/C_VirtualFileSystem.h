#pragma once
#include <cstdint>

#include <Common.h>
#include <ue/sys/filesystem.h>

#pragma optimize("", off)
namespace ue::sys::filesystem {
	class C_VirtualFileSystem { // class size 148 bytes
    private:
        char pad[144]; // class size minus 4 bytes for vtable

	public:
		// ctor missing

        /*0*/   virtual ~C_VirtualFileSystem() = default;
       // /*0*/   virtual void Test() = 0;
        /*4*/   virtual int32_t Init(ue::sys::filesystem::E_VFS_VerboseMode) = 0;
        /*8*/   virtual int32_t Done() = 0;
        /*12*/  virtual int32_t SetWriteDir(char const*) = 0;
        /*16*/  virtual int32_t SetWriteDirVFS(char const*) = 0;
        /*20*/  virtual int32_t GetWriteDir() = 0;
        /*24*/  virtual int32_t PushWriteDir() = 0;
        /*28*/  virtual int32_t PopWriteDir() = 0;
        /*32*/  virtual const char* GetRealDir(char const*) = 0;
        /*36*/  virtual const char* GetRealDir(char const*, ue::sys::utils::C_String &) = 0;
        /*40*/  virtual int32_t GetMountPoint(char const*) = 0;
        /*44*/  virtual const char* GetMountPoint(char const*, ue::sys::utils::C_String &) = 0;
        /*48*/  virtual int32_t GetRealPath(char const*) = 0;
        /*52*/  virtual const char* GetRealPath(char const*, ue::sys::utils::C_String &) = 0;
        /*56*/  virtual int32_t Mount(char const*, char const*, int32_t) = 0;
        /*60*/  virtual int32_t Mount(char const*, int32_t) = 0;
        /*64*/  virtual int32_t Mount2(char const*, char const*, char const*) = 0;
        /*68*/  virtual bool Unmount(char const*) = 0;
        /*72*/  virtual int32_t UnmountAll() = 0;
        /*76*/  virtual int32_t GetLastModificationTime(char const*) = 0;
        /*80*/  virtual int32_t SetLastModificationTimeVFS(char const*, long long) = 0;
        /*84*/  virtual int32_t SetLastModificationTimeSWD(char const*, long long) = 0;
        /*88*/  virtual int32_t DeleteSWD(char const*) = 0;
        /*92*/  virtual int32_t DeleteVFS(char const*) = 0;
        /*96*/  virtual int32_t IsReadOnly(char const*) = 0;
        /*100*/ virtual int32_t SetReadOnly(char const*, bool) = 0;
        /*104*/ virtual int32_t MkDirSWD(char const*) = 0;
        /*108*/ virtual int32_t MkDirVFS(char const*) = 0;
        /*112*/ virtual int32_t VFSPathToSWDAndFile(char const*, ue::sys::utils::C_String &, ue::sys::utils::C_String &) = 0;
        /*116*/ virtual int32_t EnumFiles(char const*) = 0;
        /*120*/ virtual int32_t EnumFilesWildcard(char const*, char const*, int32_t) = 0;
        /*124*/ virtual int32_t GetSearchPath(void) = 0;
        /*128*/ virtual int32_t FreeList(void *) = 0;
        /*132*/ virtual bool IsEmpty(char const*) = 0;
        /*136*/ virtual bool IsDirectory(char const*) = 0;
        /*140*/ virtual bool IsDirectoryFromEnumFiles(char const*) = 0;
        /*144*/ virtual bool Exists(char const*) = 0;
        /*148*/ virtual C_File* Open(char const*, uint32_t) = 0;
        /*152*/ virtual int32_t DeprecatedSearchAndOpen(char const*, uint32_t) = 0;
        /*160*/ virtual int32_t SearchForPathFromRoot(ue::sys::utils::C_HashName const&, char const*) = 0;
        /*156*/ virtual int32_t SearchForPathFromRoot(char const*) = 0;
        /*164*/ virtual int32_t GetFileNameFromHash(ue::sys::utils::C_HashName const&) = 0;
        /*168*/ virtual void ReportMissingFile(char const*) = 0; // DEV only -> nullsub
        /*172*/ virtual int32_t SearchForFileFromRoot(char const*) = 0;
        /*176*/ virtual int32_t GetDirectoryIndexForPath(char const*) = 0;
        /*180*/ virtual int32_t Close(ue::sys::filesystem::I_File *) = 0;
        /*184*/ virtual int32_t InitHashTable() = 0;
        /*188*/ virtual int32_t DeinitHashTable() = 0;
        /*192*/ virtual void nullsub_3() = 0;
        /*196*/ virtual int32_t SetHashTableFileName(char const*) = 0;
        /*200*/ virtual void nullsub_5() = 0;
        /*204*/ virtual void nullsub_6() = 0;
        /*208*/ virtual uint32_t AddFileToHashTable(char const*, uint32_t) = 0;
        /*212*/ virtual int32_t AddNewFileToHashTable(char const*) = 0;
        /*216*/ virtual void nullsub_9() = 0;
        /*220*/ virtual int32_t GenerateUniqueFileName(char const*) = 0;
        /*224*/ virtual C_VirtualFileSystem* TimeToString(long long, bool) = 0; // returns not implemented exception
        /*228*/ virtual int32_t CheckValidFileName(char const*) = 0;
        /*232*/ virtual void nullsub_10() = 0;
        /*236*/ virtual int32_t CreateHashTable(char *, uint32_t) = 0;
        /*240*/ virtual int32_t CreateAndSaveHashTable(bool) = 0;
        /*244*/ virtual int64_t FolderSize(char const*) = 0;
        /*248*/ virtual void nullsub_13() = 0;
        /*252*/ virtual void nullsub_14() = 0;
        /*256*/ virtual void nullsub_15() = 0;
        /*260*/ virtual int32_t Init(ue::sys::filesystem::E_VFS_VerboseMode, ue::sys::filesystem::S_Init const*) = 0;
	};
}
#pragma optimize("", on)

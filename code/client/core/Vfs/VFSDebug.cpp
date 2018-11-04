
#include <Hooking.h>

#include <ue/sys/filesystem.h>

static uint32_t*(*Construct_VFS_Orig)(uint32_t*);
static char(*VFS_Init_Orig)(uint32_t*);

#if FILESYSTEM_VERBOSE
#define VERBOSE printf(__FUNCTION__"\n");
#else
#define VERBOSE _noop
#endif

#pragma optimize("", off)
class C_VirtualFileSystemCache_Wrapper : public ue::sys::filesystem::C_VirtualFileSystemCache
{
public:
    ue::sys::filesystem::C_VirtualFileSystemCache* dev_ref;

    C_VirtualFileSystemCache_Wrapper(uint32_t *ptr)
    {
        printf(__FUNCTION__": Inst 0x%p\n", ptr);
        dev_ref = reinterpret_cast<ue::sys::filesystem::C_VirtualFileSystemCache*>(ptr);

        auto vtableAddr = *(uintptr_t*)(uintptr_t)this;
        auto vtableMemberAddr = (15 * 4) + vtableAddr; // offset 15 to patch
        nio::writeVP<uint32_t>(vtableMemberAddr, 0x00696D90); // ;
    }

    ue::sys::filesystem::C_VirtualFileSystemCache* GetDeviceRef() {
        return dev_ref;
    }

    virtual ~C_VirtualFileSystemCache_Wrapper() {
        VERBOSE
            dev_ref->~C_VirtualFileSystemCache();
    }

    virtual int32_t Init(ue::sys::filesystem::E_VFS_VerboseMode a1) {
        VERBOSE
        return dev_ref->Init(a1);
    }

    virtual int32_t Done() {
        VERBOSE
        return dev_ref->Done();
    }

    virtual int32_t SetWriteDir(char const* a1) {
        VERBOSE
        return dev_ref->SetWriteDir(a1);
    }

    virtual int32_t SetWriteDirVFS(char const* a1) {
        VERBOSE
        return dev_ref->SetWriteDir(a1);
    }

    virtual int32_t GetWriteDir() {
        VERBOSE
        return dev_ref->GetWriteDir();
    }

    virtual int32_t PushWriteDir() {
        VERBOSE
        return dev_ref->PushWriteDir();
    }

    virtual int32_t PopWriteDir() {
        VERBOSE
        return dev_ref->PopWriteDir();
    }

    virtual const char* GetRealDir(char const* a1) {
        VERBOSE
        return dev_ref->GetRealDir(a1);
    }

    virtual const char* GetRealDir(char const* a1, ue::sys::utils::C_String& a2) {
        VERBOSE
        return dev_ref->GetRealDir(a1, a2);
    }

    virtual int32_t GetMountPoint(char const* a1) {
        VERBOSE
        return dev_ref->GetMountPoint(a1);
    }

    virtual const char* GetMountPoint(char const* a1, ue::sys::utils::C_String& a2) {
        VERBOSE
        return dev_ref->GetMountPoint(a1, a2);
    }

    virtual int32_t GetRealPath(char const* a1) {
        VERBOSE
        return dev_ref->GetRealPath(a1);
    }

    virtual const char* GetRealPath(char const* a1, ue::sys::utils::C_String& a2) {
        VERBOSE
        return dev_ref->GetRealPath(a1, a2);
    }

    virtual int32_t Mount(char const* a1, char const* a2, int32_t a3) {
        VERBOSE
        return dev_ref->Mount(a1, a2, a3);
    }

    virtual int32_t Mount(char const* a1, int32_t a2) {
        /*_asm {
            mov eax, a1;
            mov ecx, dev_ref;
        };

        printf(__FILE__ ":" __FUNCTION__ ": Mount %s \"%s\"\n", (const char*)a2, a1);*/
        return nio::this_call<int32_t>(0x696D90, (uintptr_t)this, a1, a2);// (a1, a2);
    }

    virtual int32_t Mount2(char const* a1, char const* a2, char const* a3) {
        VERBOSE
        return dev_ref->Mount2(a1, a2, a3);
    }

    virtual bool Unmount(char const* a1) {
        VERBOSE
        return dev_ref->Unmount(a1);
    }

    virtual int32_t UnmountAll() {
        VERBOSE
        return dev_ref->UnmountAll();
    }

    virtual int32_t GetLastModificationTime(char const* a1) {
        VERBOSE
        return dev_ref->GetLastModificationTime(a1);
    }

    virtual int32_t SetLastModificationTimeVFS(char const* a1, long long a2) {
        VERBOSE
        return dev_ref->SetLastModificationTimeVFS(a1, a2);
    }

    virtual int32_t SetLastModificationTimeSWD(char const* a1, long long a2) {
        VERBOSE
        return dev_ref->SetLastModificationTimeSWD(a1, a2);
    }

    virtual int32_t DeleteSWD(char const* a1) {
        VERBOSE
        return dev_ref->DeleteSWD(a1);
    }

    virtual int32_t DeleteVFS(char const* a1) {
        VERBOSE
        return dev_ref->DeleteVFS(a1);
    }

    virtual int32_t IsReadOnly(char const* a1) {
        VERBOSE
        return dev_ref->IsReadOnly(a1);
    }

    virtual int32_t SetReadOnly(char const* a1, bool a2) {
        VERBOSE
        return dev_ref->SetReadOnly(a1, a2);
    }

    virtual int32_t MkDirSWD(char const* a1) {
        VERBOSE
        return dev_ref->MkDirSWD(a1);
    }

    virtual int32_t MkDirVFS(char const* a1) {
        VERBOSE
        return dev_ref->MkDirVFS(a1);
    }

    virtual int32_t VFSPathToSWDAndFile(char const* a1, ue::sys::utils::C_String& a2, ue::sys::utils::C_String& a3) {
        VERBOSE
        return dev_ref->VFSPathToSWDAndFile(a1, a2, a3);
    }

    virtual int32_t EnumFiles(char const* a1) {
        VERBOSE
        return dev_ref->EnumFiles(a1);
    }

    virtual int32_t EnumFilesWildcard(char const* a1, char const* a2, int32_t a3) {
        VERBOSE
        return dev_ref->EnumFilesWildcard(a1, a2, a3);
    }

    virtual int32_t GetSearchPath() {
        VERBOSE
        return dev_ref->GetSearchPath();
    }

    virtual int32_t FreeList(void* a1) {
        VERBOSE
        return dev_ref->FreeList(a1);
    }

    virtual bool IsEmpty(char const* a1) {
        VERBOSE
        return dev_ref->IsEmpty(a1);
    }

    virtual bool IsDirectory(char const* a1) {
        VERBOSE
        return dev_ref->IsDirectory(a1);
    }

    virtual bool IsDirectoryFromEnumFiles(char const* a1) {
        VERBOSE
        return dev_ref->IsDirectoryFromEnumFiles(a1);
    }

    virtual bool Exists(char const* a1) {
        VERBOSE
        return dev_ref->Exists(a1);
    }

    virtual ue::sys::filesystem::C_File* Open(char const* a1, uint32_t a2) {
        VERBOSE
        return dev_ref->Open(a1, a2);
    }

    virtual int32_t DeprecatedSearchAndOpen(char const* a1, uint32_t a2) {
        VERBOSE
        return dev_ref->DeprecatedSearchAndOpen(a1, a2);
    }

    virtual int32_t SearchForPathFromRoot(ue::sys::utils::C_HashName const& a1, char const* a2) {
        VERBOSE
        return dev_ref->SearchForPathFromRoot(a1, a2);
    }

    virtual int32_t SearchForPathFromRoot(char const* a1) {
        VERBOSE
        return dev_ref->SearchForPathFromRoot(a1);
    }

    virtual int32_t GetFileNameFromHash(ue::sys::utils::C_HashName const& a1) {
        VERBOSE
        return dev_ref->GetFileNameFromHash(a1);
    }

    virtual void ReportMissingFile(char const* a1) { // DEV only -> nullsub
        VERBOSE
        return dev_ref->ReportMissingFile(a1);
    }

    virtual int32_t SearchForFileFromRoot(char const* a1) {
        VERBOSE
        return dev_ref->SearchForFileFromRoot(a1);
    }

    virtual int32_t GetDirectoryIndexForPath(char const* a1) {
        VERBOSE
        return dev_ref->GetDirectoryIndexForPath(a1);
    }

    virtual int32_t Close(ue::sys::filesystem::I_File* a1) {
        VERBOSE
        return dev_ref->Close(a1);
    }

    virtual int32_t InitHashTable() {
        VERBOSE
        return dev_ref->InitHashTable();
    }

    virtual int32_t DeinitHashTable() {
        VERBOSE
        return dev_ref->DeinitHashTable();
    }

    virtual void nullsub_3() {}
    virtual void nullsub_5() {}
    virtual void nullsub_6() {}

    virtual int32_t SetHashTableFileName(char const* a1) {
        VERBOSE
        return dev_ref->SetHashTableFileName(a1);
    }

    virtual uint32_t AddFileToHashTable(char const* a1, uint32_t a2) {
        VERBOSE
        return dev_ref->AddFileToHashTable(a1, a2);
    }

    virtual int32_t AddNewFileToHashTable(char const* a1) {
        VERBOSE
        return dev_ref->AddNewFileToHashTable(a1);
    }

    virtual void nullsub_9() {}

    virtual int32_t GenerateUniqueFileName(char const* a1) {
        VERBOSE
        return dev_ref->GenerateUniqueFileName(a1);
    }

    virtual C_VirtualFileSystem* TimeToString(long long a1, bool a2) { // returns not implemented exception
        VERBOSE
        return dev_ref->TimeToString(a1, a2);
    }

    virtual int32_t CheckValidFileName(char const* a1) {
        VERBOSE
        return dev_ref->CheckValidFileName(a1);
    }

    virtual void nullsub_10() {}

    virtual int32_t CreateHashTable(char* a1, uint32_t a2) {
        VERBOSE
        return dev_ref->CreateHashTable(a1, a2);
    }

    virtual int32_t CreateAndSaveHashTable(bool a1) {
        VERBOSE
        return dev_ref->CreateAndSaveHashTable(a1);
    }

    virtual int64_t FolderSize(char const* a1) {
        VERBOSE
        return dev_ref->FolderSize(a1);
    }

    virtual void nullsub_13() {}
    virtual void nullsub_14() {}
    virtual void nullsub_15() {}

    virtual int32_t Init(ue::sys::filesystem::E_VFS_VerboseMode a1, ue::sys::filesystem::S_Init const* a2) {
        VERBOSE
        return dev_ref->Init(a1, a2);
    }
};
#pragma optimize("", on)


uint32_t *Construct_VFS(uint32_t *ptr)
{
    auto dev = Construct_VFS_Orig(ptr);

   // auto vt = reinterpret_cast<DWORD_PTR*>(reinterpret_cast<DWORD_PTR*>(a1)[0]);

    auto inst = new C_VirtualFileSystemCache_Wrapper(dev);
 
    return (uint32_t*)inst;
}

#if 0
static nomad::base_function init([]()
{
   // *(uint16_t*)0x69A358 = sizeof(C_VirtualFileSystemCache_Wrapper);

    nio::replace_call(&Construct_VFS_Orig, 0x69A36A, Construct_VFS);

    nio::replace_call(&VFS_Init_Orig, 0x4FABCC, (LPVOID)&[](uint32_t* a1) {
        auto res = VFS_Init_Orig(a1);

        auto vfsWrapper = (C_VirtualFileSystemCache_Wrapper*)0x1BDC918;
        auto refDevice = vfsWrapper->GetDeviceRef();
        *(uint32_t*)0x1BDC918 = 0x1BDC918;

        return a1;
    });
});
#endif

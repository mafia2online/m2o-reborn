#if 0
//@ M2O_EXPORT_REGION_START
#define M2O_VERSION_CREATE(major, minor, patch) (((major)<<16) | ((minor)<<8) | (patch))
#define M2O_VERSION_GET_MAJOR(version) (((version)>>16)&0xFF)
#define M2O_VERSION_GET_MINOR(version) (((version)>>8)&0xFF)
#define M2O_VERSION_GET_PATCH(version) ((version)&0xFF)

typedef union vec3_t {
    struct {
        float x;
        float y;
        float z;
    };

    float xyz[3];
} vec3_t;
//@ M2O_EXPORT_REGION_END
#endif

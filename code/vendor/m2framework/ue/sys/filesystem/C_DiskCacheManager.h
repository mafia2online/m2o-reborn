#pragma once
#include <cstdint>

namespace ue::sys::filesystem {
  
  class C_DiskCacheManager {
  public:
    virtual ~C_DiskCacheManager() = 0;
  
    virtual void NotifyIOOperationBegin() = 0;
    virtual void NotifyIOOperationEnd() = 0;
    virtual int32_t AllowBackgroundIOOperation() = 0;
    virtual void Initialize(uint32_t) = 0;
  };
  
}

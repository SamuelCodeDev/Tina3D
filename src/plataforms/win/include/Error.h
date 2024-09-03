#ifndef ERROR_H
#define ERROR_H

#include "Types.h"

namespace Tina
{
    class Error final
    {
    private:
        int32 hrCode;             
        string funcName;            
        string fileName;            
        int lineNum;                
        
    public:
        Error() noexcept;
        Error(const int32 hr, const string_view & func, const string_view & file, const int32 line) noexcept;
        string_view ToString() const;
    };
}

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                               \
{                                                                      \
    int32 hr = (x);                                                    \
    if(FAILED(hr)) { throw Error(hr, __func__, __FILE__, __LINE__); }  \
}
#endif

#endif
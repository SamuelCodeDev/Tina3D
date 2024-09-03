#include "Error.h"
#include <comdef.h>
#include <format>
using std::format;

namespace Tina
{
    Error::Error() noexcept : hrCode{}, lineNum{-1}
    {
    }

    Error::Error(const int32 hr, const string_view & func, const string_view & file, const int32 line) noexcept
        : hrCode{hr}, funcName{func}, lineNum{line}
    {
        auto pos = file.find_last_of('\\');

        if (pos != string::npos)
            fileName = file.substr(pos + 1);
    }

    string_view Error::ToString() const
    {
        _com_error err(hrCode);
        
        return format("{} falhou em {}, linha {}: \n{}", 
                        funcName, fileName, lineNum, err.ErrorMessage());
    }
}
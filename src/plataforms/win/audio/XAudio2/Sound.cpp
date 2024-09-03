#include "Sound.h"

constexpr DWORD fourccRIFF = 'FFIR';
constexpr DWORD fourccDATA = 'atad';
constexpr DWORD fourccFMT  = ' tmf';
constexpr DWORD fourccWAVE = 'EVAW';

#undef UNICODE

namespace Tina
{
    Sound::Sound(const string_view fileName, const uint32 nTracks) noexcept :
        format{},
        buffer{},
        volume{1.0f},
        frequency{1.0f},
        tracks{nTracks},
        index{}
    {
        voices = new IXAudio2SourceVoice*[tracks] {nullptr};

        HANDLE hFile {
            CreateFile(fileName.data(), 
            GENERIC_READ, 
            FILE_SHARE_READ, 
            nullptr, 
            OPEN_EXISTING, 
            0, 
            nullptr)
        };

        DWORD dwChunkSize{};
        DWORD dwChunkPosition{};
        FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);

        DWORD filetype;
        ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
        if (filetype != fourccWAVE)
            return;

        FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
        ReadChunkData(hFile, &format, dwChunkSize, dwChunkPosition);

        FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
        BYTE * pDataBuffer = new BYTE[dwChunkSize];
        ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

        buffer.AudioBytes = dwChunkSize;
        buffer.pAudioData = pDataBuffer;
        buffer.Flags = XAUDIO2_END_OF_STREAM;

        CloseHandle(hFile);
        delete [] pDataBuffer;
    }

    Sound::~Sound() noexcept
    {
        delete[] buffer.pAudioData;    
        delete[] voices;
    }

    HRESULT Sound::FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition)
    {
        HRESULT hr { S_OK };
        if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, nullptr, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());

        DWORD dwChunkType{};
        DWORD dwChunkDataSize{};
        DWORD dwRIFFDataSize{};
        DWORD dwFileType{};
        DWORD bytesRead{};
        DWORD dwOffset{};

        while (hr == S_OK)
        {
            DWORD dwRead{};
            if(ReadFile( hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
                hr = HRESULT_FROM_WIN32(GetLastError());

            if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
                hr = HRESULT_FROM_WIN32(GetLastError());

            switch (dwChunkType)
            {
            case fourccRIFF:
                dwRIFFDataSize = dwChunkDataSize;
                dwChunkDataSize = 4;
                if(ReadFile( hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
                    hr = HRESULT_FROM_WIN32(GetLastError());
                break;

            default:
                if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT))
                    return HRESULT_FROM_WIN32(GetLastError());            
            }

            dwOffset += sizeof(DWORD) * 2;
            
            if (dwChunkType == fourcc)
            {
                dwChunkSize = dwChunkDataSize;
                dwChunkDataPosition = dwOffset;
                return S_OK;
            }

            dwOffset += dwChunkDataSize;
            
            if (bytesRead >= dwRIFFDataSize) return S_FALSE;
        }

        return S_OK;
    }

    HRESULT Sound::ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset)
    {
        HRESULT hr = S_OK;
        if(INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, nullptr, FILE_BEGIN))
            return HRESULT_FROM_WIN32(GetLastError());
        
        DWORD dwRead;
        if(ReadFile(hFile, buffer, buffersize, &dwRead, nullptr) == 0)
            hr = HRESULT_FROM_WIN32(GetLastError());
            
        return hr;
    }
}
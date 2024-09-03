#ifndef SOUND_H
#define SOUND_H

#include "Types.h"
#include <xaudio2.h> 

namespace Tina
{
    class Sound final
    {
    private:
        WAVEFORMATEXTENSIBLE format;
        XAUDIO2_BUFFER       buffer;
        float                volume;
        float                frequency;

        IXAudio2SourceVoice  ** voices;
        uint32                 tracks;
        uint32                 index;

        HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD & dwChunkSize, DWORD & dwChunkDataPosition);
        HRESULT ReadChunkData(HANDLE hFile, void * buffer, DWORD buffersize, DWORD bufferoffset);

        friend class Audio;

    public:
        Sound(const string_view fileName, const uint32 nTracks) noexcept;
        ~Sound() noexcept;
    };
}
#endif
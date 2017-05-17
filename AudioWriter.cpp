//
// Created by gordonleem on 5/8/17.
//

#include <iomanip>

#include "AudioWriter.h"

void SoxDatWriter::writeHeader(const AudioGenerator &audio, unsigned int sampleRate)
{
    getFile() << "; File: " << getFilename() << std::endl;
    getFile() << "; Sample Rate " << sampleRate << std::endl;
    getFile() << "; Channels 1" << std::endl;
}

void SoxDatWriter::writeData(const AudioGenerator &audio, unsigned int sampleRate)
{
    getFile() << std::fixed;
    getFile() << std::setprecision(15); // 150db of dynamic range should exceed

    double offset(1.0/double(sampleRate));
    for(double i(0); i < audio.getDuration(); i += offset)
    {
        getFile() << i << ' ' << audio[i] << std::endl;
    }
}

void WaveWriter::WriteFourCC(const std::string &str)
{
    for(int i(0); i<4; i++)
    {
        uint8_t c(i < str.length() ? str[i] : 0);
        WriteInt8(c);
    }
}
void WaveWriter::WriteInt16(uint16_t val)
{
    WriteInt8(val & 0xFF);
    WriteInt8(val >> 8);
}
void WaveWriter::WriteInt32(uint32_t val)
{
    WriteInt16(val & 0xFFFF);
    WriteInt16(val >> 16);
}
void WaveWriter::WriteFloat32(float val)
{
    // Evil, evil stuff
    // Assumptions:
    // * that both floating point and integer number representations have the same 'endianess'
    // * that 'float' is an IEEE 754 32 bit floating point number

    uint32_t *ptr = reinterpret_cast<uint32_t *>(&val);
    WriteInt32(*ptr);

}
void WaveWriter::WriteFloat32(double val)
{
    WriteFloat32(static_cast<float>(val));
}

void WaveWriter::writeHeader(const AudioGenerator &audio, unsigned int sampleRate)
{
    uint16_t sampleSize(m_pcm ? 2 : 4);
    uint32_t samples(GetSampleCount(audio, sampleRate));

    // Assumes mono signal
    WriteFourCC("RIFF");                        // Magic number
    WriteInt32(samples * sampleSize + 40);      // File size
    WriteFourCC("WAVE");                        // File type
    WriteFourCC("fmt ");                         // fmt chunk
    WriteInt32(16);                             // Format header size
    WriteInt16(m_pcm ? 1 : 3);                  // Audio format (1 = PCM, 3 = 32 bit float)
    WriteInt16(1);                              // Channels
    WriteInt32(sampleRate);                     // Sample Rate
    WriteInt32(sampleRate * sampleSize * 8);    // Bit rate
    WriteInt16(sampleSize);                     // Sample size
    WriteInt16(sampleSize * 8);                 // Bits per sample
    WriteFourCC("data");                        // Data header
    WriteInt32(samples * sampleSize);           // Data size
}

void WaveWriter::writeData(const AudioGenerator &audio, unsigned int sampleRate)
{
    uint32_t samples(GetSampleCount(audio, sampleRate));
    double offset(1.0/double(sampleRate));
    for(uint32_t i(0); i < samples; i ++)
    {
        double sample(audio[i * offset]);
        if(m_pcm)
        {
            uint16_t val(65535);
            val *= (sample + 1) / 2;
            WriteInt16(val);
        }
        else
        {
            WriteFloat32(sample);
        }
    }
}

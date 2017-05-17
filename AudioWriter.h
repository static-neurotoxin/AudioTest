//
// Created by gordonleem on 5/8/17.
//
#pragma once
#ifndef AUDIOTEST_AUDIOWRITER_H
#define AUDIOTEST_AUDIOWRITER_H

#include <fstream>
#include <ios>
#include <iostream>
#include <list>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "AudioGenerator.h"

class AudioWriter
{
public:
    AudioWriter(const boost::filesystem::path &filename) : m_filename(filename) {}
    AudioWriter() = delete;
    AudioWriter(const AudioWriter &) = delete;

    inline virtual std::ios_base::openmode getMode()
    {
        return std::ios_base::out | std::ios_base::trunc;
    }

    virtual void writeHeader (const AudioGenerator &audio, unsigned int sampleRate) = 0;
    virtual void writeData   (const AudioGenerator &audio, unsigned int sampleRate) = 0;
    virtual void writeTrailer(const AudioGenerator &audio, unsigned int sampleRate) = 0;
    inline virtual void write(const AudioGenerator &audio, unsigned int sampleRate)
    {
        m_file.open(m_filename, getMode());
        if(m_file)
        {
            writeHeader(audio, sampleRate);
            writeData(audio, sampleRate);
            writeTrailer(audio, sampleRate);
        }
    }

    // Accessors
protected:
    inline const boost::filesystem::path &getFilename() const {return m_filename;}
    inline boost::filesystem::ofstream   &getFile()           {return m_file;}

private:
    boost::filesystem::path     m_filename;
    boost::filesystem::ofstream m_file;
};

class SoxDatWriter : public AudioWriter
{
public:
    SoxDatWriter(const boost::filesystem::path &filename) : AudioWriter(filename) {}

    virtual void writeHeader        (const AudioGenerator &audio, unsigned int sampleRate);
    virtual void writeData          (const AudioGenerator &audio, unsigned int sampleRate);
    inline virtual void writeTrailer(const AudioGenerator &audio, unsigned int sampleRate) {}
};

class WaveWriter : public AudioWriter
{
public:
    WaveWriter(const boost::filesystem::path &filename, bool pcm = false) : AudioWriter(filename), m_pcm(pcm) {}
    inline virtual std::ios_base::openmode getMode()
    {
        return std::ios_base::out | std::ios_base::trunc | std::ios_base::binary;
    }

    virtual void writeHeader        (const AudioGenerator &audio, unsigned int sampleRate);
    virtual void writeData          (const AudioGenerator &audio, unsigned int sampleRate);
    inline virtual void writeTrailer(const AudioGenerator &audio, unsigned int sampleRate) {}

protected:
    // Ensure that various code paths get the same sample count.
    inline uint32_t GetSampleCount(const AudioGenerator &audio, unsigned int sampleRate)
    {
        return static_cast<uint32_t >(sampleRate * audio.getDuration());
    }

    void WriteInt8(uint8_t val) {getFile() << val;}
    void WriteFourCC(const std::string &str);
    void WriteInt16(uint16_t val);
    void WriteInt32(uint32_t val);
    void WriteFloat32(float val);
    void WriteFloat32(double val);

    bool m_pcm;
};


#endif //AUDIOTEST_AUDIOWRITER_H

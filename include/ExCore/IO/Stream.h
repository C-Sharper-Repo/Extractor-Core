#pragma once
#include <string>
#include <cstdint>
#include <stdio.h>

class Stream {
public:
    static constexpr uint8_t READ_FLAG = 0x1;
    static constexpr uint8_t WRITE_FLAG = 0x2;

    Stream() : _position(0), _length(0), _flags(0), _capacity(0) {}
    Stream(const uint8_t flags, const size_t length = 0, const size_t capacity = 0) : _position(0), _length(length), _flags(flags), _capacity(capacity) {}
    virtual ~Stream() {}

    bool isEOF() const { return _position >= _length; }

    virtual size_t tell() const { return _position; }
    virtual size_t size() const { return _length; }
    virtual size_t capacity() const { return _capacity; }

    virtual bool isOpen() const = 0;
    virtual bool canWrite() const { return bool(_flags & WRITE_FLAG) && isOpen(); }
    virtual bool canRead()  const { return bool(_flags & READ_FLAG)  && isOpen(); }

    virtual size_t read(void* buffer, size_t elementSize, size_t count) const = 0;
    virtual size_t write(const void* buffer, const size_t elementSize, const size_t count) const = 0;

    virtual size_t read(void* buffer, size_t size) const { 
        return read(buffer, 1, size); 
    }

    virtual size_t write(const void* buffer, const size_t size) const {
        return write(buffer, 1, size);
    }

    virtual size_t writeZero(const size_t count) const {
        if (!canWrite()) { return _position; }
        void* buffer = _malloca(count);
        if (!buffer) { return _position; }

        memset(buffer, 0, count);
        size_t ret = write(buffer, 1, count);
        _freea(buffer);
        return ret;
    }

    template<typename T>
    size_t writeValue(const T& value, const size_t count = 1) const {
        if (!canWrite() || count < 1) { return _position; }

        void* buffer = _malloca(count * sizeof(T));
        if (!buffer) { return _position; }

        std::fill_n(reinterpret_cast<T*>(buffer), count, value);
       
        size_t ret = write(buffer, 1, count * sizeof(T));
        _freea(buffer);
        return ret;
    }

    virtual bool flush() const = 0;
    virtual bool close() const = 0;

    virtual size_t seek(int64_t offset, int origin) const = 0;

    virtual bool tryReserve(const size_t bytes) const { return true; }

    void copyTo(const Stream& other) const {
        size_t curPos = _position;
        size_t remain = _length - _position;

        if (remain < 1) { return; }

        size_t curPosTgt = other._position;
        size_t remainTgt = other._length - other._position;

        if (remainTgt < remain && !other.tryReserve(((remain - remainTgt) + other._position))) {
            return;
        }

        void* data = _malloca(remain);
        if (!data) { return; }

        this->read(data, remain);
        other.write(data, remain);

        _freea(data);
        this->seek(curPos, SEEK_SET);
    }

protected:

    mutable uint8_t _flags;
    mutable size_t _position;
    mutable size_t _length;
    mutable size_t _capacity;
};
#include <ExCore/IO/FileStream.h>
#include <Windows.h>

#include <io.h>
#define F_OK 0
#define access _access

uint8_t getIOFlags(const char* mode) {
    if (mode && strlen(mode)) {
        switch (mode[0]) {
            case 'r': return Stream::READ_FLAG;
            case 'w': return Stream::WRITE_FLAG;
        }
    }
    return 0;
}

FileStream::FileStream() : Stream(), _file(nullptr), _filepath() {}
FileStream::FileStream(const char* filepath) :Stream(), _file(nullptr), _filepath(filepath) { }
FileStream::FileStream(const wchar_t* filepath) : Stream(), _file(nullptr), _filepath() {
    if (filepath) {
        int len = WideCharToMultiByte(CP_UTF8, 0, filepath, -1, NULL, 0, NULL, NULL);
        char* temp = reinterpret_cast<char*>(_malloca(len + 1));
        if (temp) {
            temp[len] = 0;
            WideCharToMultiByte(CP_UTF8, 0, filepath, -1, temp, len, NULL, NULL);
            _filepath.reserve(len);
            _filepath.assign(temp);
            _freea(temp);
        }
    }
}

FileStream::FileStream(const char* filepath, const char* mode, const int shared) : Stream(getIOFlags(mode)), _file(nullptr),  _filepath() {
    open(filepath, mode, shared);
}

FileStream::FileStream(const wchar_t* filepath, const char* mode, const int shared) : Stream(getIOFlags(mode)), _file(nullptr), _filepath() {
    open(filepath, mode, shared);
}
FileStream::~FileStream() { close(); }

const std::string& FileStream::getFilePath() const { return _filepath; }

bool FileStream::isOpen() const { return _file != nullptr; }
bool FileStream::open(const char* filepath, const char* mode, const int shared) const {
    if (isOpen()) { return false; }

    _length = 0;
    _capacity = 0;
    _filepath.clear();
    _filepath.append(filepath);

    if (!mode) { return false; }
    return open(mode, shared);
}

bool FileStream::open(const wchar_t* filepath, const char* mode, const int shared) const {
    int len = WideCharToMultiByte(CP_UTF8, 0, filepath, -1, NULL, 0, NULL, NULL);
    char* temp = reinterpret_cast<char*>(_malloca(len + 1));
    if (!temp) { return false; }
    temp[len] = 0;

    WideCharToMultiByte(CP_UTF8, 0, filepath, -1, temp, len, NULL, NULL);
    const bool ret = open(temp, mode);
    _freea(temp);
    return ret;
}

bool FileStream::open(const char* mode, const int shared) const {
    if (isOpen()) { return false; }

    _flags &= uint8_t(~0x3);
    _flags |= getIOFlags(mode);

    const bool isRead = (mode[0] == 'r');
    _length = 0;
    _capacity = 0;
    _position = 0;

    if (isRead) {
        if (access(_filepath.c_str(), F_OK) == 0) {
            fopen_s(&_file, _filepath.c_str(), mode);
            if (_file) {
                _fseeki64_nolock(_file, 0, SEEK_END);
                _length = _ftelli64_nolock(_file);
                _capacity = _length;
                _fseeki64_nolock(_file, 0, SEEK_SET);
                return true;
            }
        }
        return false;
    }

    if (shared) {
        _file = _fsopen(_filepath.c_str(), mode, shared);
        return bool(_file);
    }

    fopen_s(&_file, _filepath.c_str(), mode);
    return bool(_file);
}

bool FileStream::flush() const {
    if (!_file) { return false; }
    _fflush_nolock(_file);
    return true;
}

bool FileStream::close() const {
    _position = 0;
    _file = 0;

    if (_file) {
        _fflush_nolock(_file);
        _fclose_nolock(_file);
        _file = nullptr;
        return true;
    }
    return false;
}

size_t FileStream::read(void* buffer, size_t elementSize, size_t count) const {
    if (!_file || !canRead()) { return 0; }
    _position += count * elementSize;
    _position = _position > _length ? _length : _position;
    return _fread_nolock(buffer, elementSize, count, _file);
}

size_t FileStream::write(const void* buffer, size_t elementSize, const size_t count) const {
    if (!_file || !canWrite()) { return 0; }

    size_t size = count * elementSize;
    _position += size;
    _length = _position > _length ? _position : _length;
    _capacity = _length;
    return _fwrite_nolock(buffer, elementSize, count, _file);
}
size_t FileStream::seek(int64_t offset, int origin) const {
    if (!_file) { return _position; }

    if (origin == SEEK_SET) {
        _position = offset;
        _fseeki64_nolock(_file, offset, SEEK_SET);
        return _position;
    }
    else if (origin == SEEK_CUR) {
        _position += offset;
        _fseeki64_nolock(_file, offset, SEEK_CUR);
        return _position;
    }

    _position = _length - offset;
    _fseeki64_nolock(_file, offset, SEEK_END);
    return _position;
}
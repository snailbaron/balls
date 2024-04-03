#pragma once

#include <exception>
#include <source_location>
#include <sstream>
#include <stacktrace>
#include <string>

#include <SDL.h>

template <class T>
concept Streamable = requires (std::ostream& output, const T& x)
{
    output << x;
};

class Error : public std::exception {
public:
    Error(std::source_location sl = std::source_location::current())
    {
        auto stream = std::ostringstream{};
        stream <<
            std::stacktrace::current() << "\n" <<
            sl.file_name() << " (" << sl.function_name() << "): " <<
            sl.line() << ":" << sl.column() << ": ";
        _message = std::move(stream).str();
    }

    const char* what() const noexcept override
    {
        return _message.c_str();
    }

    template <Streamable T>
    Error& operator<<(const T& x) &
    {
        append(x);
        return *this;
    }

    template <Streamable T>
    Error&& operator<<(const T& x) &&
    {
        append(x);
        return std::move(*this);
    }

private:
    template <Streamable T>
    void append(const T& x)
    {
        auto stream = std::ostringstream{std::move(_message), std::ios::app};
        stream << x;
        _message = std::move(stream).str();
    }

    std::string _message;
};

template <class T>
T* checkSdl(T* ptr, std::source_location sl = std::source_location::current())
{
    if (ptr == nullptr) {
        throw Error{sl} << SDL_GetError();
    }
    return ptr;
}

inline void checkSdl(
    int returnCode, std::source_location sl = std::source_location::current())
{
    if (returnCode != 0) {
        throw Error{sl} << SDL_GetError();
    }
}
#include "sdl.hpp"

#include "error.hpp"

namespace sdl {

namespace {

template <class T>
T* checkSdl(T* ptr, std::source_location sl = std::source_location::current())
{
    if (ptr == nullptr) {
        throw Error{sl} << SDL_GetError();
    }
    return ptr;
}

void checkSdl(
    int returnCode, std::source_location sl = std::source_location::current())
{
    if (returnCode != 0) {
        throw Error{sl} << SDL_GetError();
    }
}

template <class T>
T* checkTtf(T* ptr, std::source_location sl = std::source_location::current())
{
    if (ptr == nullptr) {
        throw Error{sl} << TTF_GetError();
    }
    return ptr;
}

void checkTtf(
    int returnCode, std::source_location sl = std::source_location::current())
{
    if (returnCode != 0) {
        throw Error{sl} << TTF_GetError();
    }
}

} // namespace

Init::Init(uint32_t flags)
{
    checkSdl(SDL_Init(flags));
    checkTtf(TTF_Init());
}

Init::~Init()
{
    TTF_Quit();
    SDL_Quit();
}

Window::Window(const char* title, int w, int h, SDL_WindowFlags flags)
{
    create(title, w, h, flags);
}

void Window::create(const char* title, int w, int h, SDL_WindowFlags flags)
{
    _ptr.reset(checkSdl(SDL_CreateWindow(title, w, h, flags)));
}

void Window::destroy()
{
    _ptr.reset();
}

SDL_Window* Window::ptr()
{
    return _ptr.get();
}

const SDL_Window* Window::ptr() const
{
    return _ptr.get();
}

Renderer::Renderer(Window& window, const char* name, uint32_t flags)
{
    create(window, name, flags);
}

void Renderer::create(Window& window, const char* name, uint32_t flags)
{
    _ptr.reset(checkSdl(SDL_CreateRenderer(window.ptr(), name, flags)));
}

void Renderer::destroy()
{
    _ptr.reset();
}

SDL_Renderer* Renderer::ptr()
{
    return _ptr.get();
}

const SDL_Renderer* Renderer::ptr() const
{
    return _ptr.get();
}

void Renderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    checkSdl(SDL_SetRenderDrawColor(ptr(), r, g, b, a));
}

void Renderer::setDrawColor(const SDL_Color& color)
{
    setDrawColor(color.r, color.g, color.b, color.a);
}

void Renderer::clear()
{
    checkSdl(SDL_RenderClear(ptr()));
}

void Renderer::present()
{
    checkSdl(SDL_RenderPresent(ptr()));
}

Size Renderer::size()
{
    auto size = Size{};
    checkSdl(SDL_GetRenderOutputSize(ptr(), &size.w, &size.h));
    return size;
}

void Renderer::fillRect(const SDL_FRect& rect)
{
    checkSdl(SDL_RenderFillRect(ptr(), &rect));
}

void Renderer::renderRect(const SDL_FRect& rect)
{
    checkSdl(SDL_RenderRect(ptr(), &rect));
}

Texture Renderer::createTextureFromSurface(Surface& surface)
{
    return Texture{checkSdl(SDL_CreateTextureFromSurface(ptr(), surface.ptr()))};
}

Texture Renderer::createTextureFromSurface(Surface&& surface)
{
    return Texture{checkSdl(SDL_CreateTextureFromSurface(ptr(), surface.ptr()))};
}

Surface::Surface(SDL_Surface* ptr)
{
    _ptr.reset(ptr);
}

SDL_Surface* Surface::ptr()
{
    return _ptr.get();
}

const SDL_Surface* Surface::ptr() const
{
    return _ptr.get();
}

Texture::Texture(SDL_Texture* ptr)
{
    _ptr.reset(ptr);
}

void hideCursor()
{
    checkSdl(SDL_HideCursor());
}

void showCursor()
{
    checkSdl(SDL_ShowCursor());
}

Font::Font(const char* file, int ptsize)
{
    open(file, ptsize);
}

void Font::open(const char* file, int ptsize)
{
    _ptr.reset(checkTtf(TTF_OpenFont(file, ptsize)));
}

void Font::close()
{
    _ptr.reset();
}

TTF_Font* Font::ptr()
{
    return _ptr.get();
}

const TTF_Font* Font::ptr() const
{
    return _ptr.get();
}

Surface renderUtf8Blended(Font& font, const char* text, const SDL_Color& fg)
{
    return Surface{checkTtf(TTF_RenderUTF8_Blended(font.ptr(), text, fg))};
}

} // namespace sdl
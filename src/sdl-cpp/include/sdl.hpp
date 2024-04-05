#pragma once

#include <SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cstdint>
#include <memory>

namespace sdl {

class Init {
public:
    Init(uint32_t flags);
    ~Init();

    Init(const Init&) = delete;
    Init(Init&&) = delete;
    Init& operator=(const Init&) = delete;
    Init& operator=(Init&&) = delete;
};

struct Size {
    int w = 0;
    int h = 0;
};

class Window {
public:
    Window() = default;
    Window(const char* title, int w, int h, SDL_WindowFlags flags);

    void create(const char* title, int w, int h, SDL_WindowFlags flags);
    void destroy();

    SDL_Window* ptr();
    const SDL_Window* ptr() const;

private:
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> _ptr {
        nullptr, SDL_DestroyWindow};
};

class Surface {
public:
    Surface() = default;
    explicit Surface(SDL_Surface* ptr);

    SDL_Surface* ptr();
    const SDL_Surface* ptr() const;

private:
    std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> _ptr{
        nullptr, SDL_DestroySurface};
};

class Texture {
public:
    Texture() = default;
    explicit Texture(SDL_Texture* ptr);

private:
    std::unique_ptr<SDL_Texture, void(*)(SDL_Texture*)> _ptr{
        nullptr, SDL_DestroyTexture};
};

class Renderer {
public:
    Renderer() = default;
    Renderer(Window& window, const char* name, uint32_t flags);

    void create(Window& window, const char* name, uint32_t flags);
    void destroy();

    SDL_Renderer* ptr();
    const SDL_Renderer* ptr() const;

    void setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    void setDrawColor(const SDL_Color& color);
    void clear();
    void present();

    Size size();
    void fillRect(const SDL_FRect& rect);
    void renderRect(const SDL_FRect& rect);
    Texture createTextureFromSurface(Surface& surface);
    Texture createTextureFromSurface(Surface&& surface);

private:
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _ptr{
        nullptr, SDL_DestroyRenderer};
};

void hideCursor();
void showCursor();

class Font {
public:
    Font() = default;
    Font(const char* file, int ptsize);

    void open(const char* file, int ptsize);
    void close();

    TTF_Font* ptr();
    const TTF_Font* ptr() const;

private:
    std::unique_ptr<TTF_Font, void(*)(TTF_Font*)> _ptr{
        nullptr, TTF_CloseFont};
};

Surface renderUtf8Blended(Font& font, const char* text, const SDL_Color& fg);

} // namespace sdl
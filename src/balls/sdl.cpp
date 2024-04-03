#include "sdl.hpp"

#include "error.hpp"

namespace sdl {

Renderer::Renderer(SDL_Renderer* ptr)
    : _ptr(ptr, SDL_DestroyRenderer)
{ }

void Renderer::reset(SDL_Renderer* ptr)
{
    _ptr.reset(ptr);
}

void Renderer::setDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    checkSdl(SDL_SetRenderDrawColor(ptr(), r, g, b, a));
}

void Renderer::clear()
{
    checkSdl(SDL_RenderClear(ptr()));
}

void Renderer::present()
{
    checkSdl(SDL_RenderPresent(ptr()));
}

void Renderer::fillRect(const SDL_FRect& rect)
{
    checkSdl(SDL_RenderFillRect(ptr(), &rect));
}

} // namespace sdl
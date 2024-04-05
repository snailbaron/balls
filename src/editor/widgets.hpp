#pragma once

#include "geometry.hpp"
#include "sdl.hpp"

#include <array>
#include <concepts>
#include <memory>
#include <functional>
#include <utility>
#include <vector>

#include <iostream>

class Widget {
public:
    enum class State {
        Idle,
        Hovered,
        Pressed,
        Lost,
    };

    virtual ~Widget() = 0 {}

    virtual bool contains(float /*x*/, float /*y*/) const { return false; }
    virtual void act() const {}

    virtual void render(sdl::Renderer& renderer) const = 0;

    const State& state() const
    {
        return _state;
    }

    void hover()
    {
        _state = stateTransitions[std::to_underlying(_state)][0];
        onHover();
    }

    void unhover()
    {
        _state = stateTransitions[std::to_underlying(_state)][1];
        onUnhover();
    }

    void press()
    {
        _state = stateTransitions[std::to_underlying(_state)][2];
        onPress();
    }

    void release()
    {
        _state = stateTransitions[std::to_underlying(_state)][3];
        onRelease();
    }

protected:
    virtual void onHover() {}
    virtual void onUnhover() {}
    virtual void onPress() {}
    virtual void onRelease() {}

private:
    static constexpr auto stateTransitions = std::array{
        //         hover           unhover      press           release
        std::array{State::Hovered, State::Idle, State::Pressed, State::Idle   }, // from idle
        std::array{State::Hovered, State::Idle, State::Pressed, State::Hovered}, // from hovered
        std::array{State::Pressed, State::Lost, State::Pressed, State::Hovered}, // from pressed
        std::array{State::Pressed, State::Lost, State::Lost,    State::Idle   }, // from lost
    };

    State _state = State::Idle;
};

class UI {
public:
    template <class W, class... Args>
    requires std::derived_from<W, Widget> && std::constructible_from<W, Args...>
    void add(Args&&... args)
    {
        _widgets.push_back(std::make_unique<W>(std::forward<Args>(args)...));
    }

    void render(sdl::Renderer& renderer) const
    {
        for (const auto& widget : _widgets) {
            widget->render(renderer);
        }
    }

    bool processEvent(const SDL_Event& event)
    {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT) {
            if (_hovered) {
                _pressed = _hovered;
                _pressed->press();
                return true;
            }
        } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
                event.button.button == SDL_BUTTON_LEFT) {
            if (_pressed) {
                _pressed->release();
                if (_hovered) {
                    if (_hovered == _pressed) {
                        _hovered->act();
                    } else {
                        _hovered->hover();
                    }
                }
                _pressed = nullptr;
                return true;
            }
        } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            float x = event.motion.x;
            float y = event.motion.y;

            if (_hovered && !_hovered->contains(x, y)) {
                _hovered->unhover();
                _hovered = nullptr;
            }

            if (!_hovered) {
                _hovered = widgetUnderCursor(x, y);
                if (_hovered && (!_pressed || _hovered == _pressed)) {
                    _hovered->hover();
                }
            }
        }

        return false;
    }

private:
    Widget* widgetUnderCursor(float x, float y)
    {
        for (const auto& widget : _widgets) {
            if (widget->contains(x, y)) {
                return widget.get();
            }
        }
        return nullptr;
    }

    std::vector<std::unique_ptr<Widget>> _widgets;
    Widget* _hovered = nullptr;
    Widget* _pressed = nullptr;
};

class Button : public Widget {
public:
    Button(
        sdl::Renderer& renderer,
        sdl::Font& font,
        const Point& center,
        const char* text,
        std::function<void()> action)
        : _outerRect{
            .x = center.x - 50.f,
            .y = center.y - 10.f,
            .w = 100.f,
            .h = 50.f,
        }
        , _innerRect{
            .x = _outerRect.x + 5.f,
            .y = _outerRect.y + 5.f,
            .w = _outerRect.w - 10.f,
            .h = _outerRect.h - 10.f,
        }
        , _action{std::move(action)}
    {
        // TODO: create texture with text
        (void)renderer;
        (void)font;
        (void)text;
    }

    bool contains(float x, float y) const override
    {
        return x >= _outerRect.x && x <= _outerRect.x + _outerRect.w &&
            y >= _outerRect.y && y <= _outerRect.y + _outerRect.h;
    }

    void act() const override
    {
        _action();
    }

    void render(sdl::Renderer& renderer) const override
    {
        renderer.setDrawColor(outerColor());
        renderer.fillRect(_outerRect);
        renderer.setDrawColor(innerColor());
        renderer.fillRect(_innerRect);
    }

private:
    static constexpr auto outerColors = std::array{
        SDL_Color{100, 50, 50, 255}, // idle
        SDL_Color{120, 70, 70, 255}, // hovered
        SDL_Color{70, 120, 70, 255}, // pressed
        SDL_Color{50, 100, 50, 255}, // lost
    };
    static constexpr auto innerColors = std::array{
        SDL_Color{100, 100, 100, 255}, // idle
        SDL_Color{120, 120, 120, 255}, // hovered
        SDL_Color{120, 150, 120, 255}, // pressed
        SDL_Color{80, 100, 80, 255}, // lost
    };

    const SDL_Color& outerColor() const
    {
        return outerColors[std::to_underlying(state())];
    }

    const SDL_Color& innerColor() const
    {
        return innerColors[std::to_underlying(state())];
    }

    SDL_FRect _outerRect;
    SDL_FRect _innerRect;
    std::function<void()> _action;
    sdl::Texture _textTexture;
};
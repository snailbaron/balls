#pragma once

#include "widgets.hpp"

#include "geometry.hpp"
#include "intersection.hpp"
#include "sdl.hpp"

#include <vector>

class Camera {
public:
    void focus(
        int screenWidth, int screenHeight, const Point& center, float minRange)
    {
        _screenWidth = 1.f * screenWidth;
        _screenHeight = 1.f * screenHeight;
        _screenWorldRect = AxisAlignedRect{center, minRange, minRange};
        if (screenWidth > screenHeight) {
            _screenWorldRect.width *= _screenWidth / _screenHeight;
        } else {
            _screenWorldRect.height *= _screenHeight / _screenWidth;
        }
    }

    void moveScreen(float dx, float dy)
    {
        _screenWorldRect.center.x -= dx * _screenWorldRect.width / _screenWidth;
        _screenWorldRect.center.y += dy * _screenWorldRect.height / _screenHeight;
    }

    void zoomOut(float x)
    {
        // this zoom is unstable, should really have integer zoom levels
        _screenWorldRect.width *= std::pow(1.2f, x);
        _screenWorldRect.height *= std::pow(1.2f, x);
    }

    void zoomIn(float x)
    {
        _screenWorldRect.width /= std::pow(1.2f, x);
        _screenWorldRect.height /= std::pow(1.2f, x);
    }

    Point project(const Point& point)
    {
        return Point{
            .x = (point.x - _screenWorldRect.minX()) * _screenWidth / _screenWorldRect.width,
            .y = (_screenWorldRect.maxY() - point.y) * _screenHeight / _screenWorldRect.height,
        };
    }

    Point toWorld(const Point& screenPoint)
    {
        return Point{
            .x = screenPoint.x * _screenWorldRect.width / _screenWidth + _screenWorldRect.minX(),
            .y = _screenWorldRect.maxY() - screenPoint.y * _screenWorldRect.height / _screenHeight,
        };
    }

    Line project(const Line& line)
    {
        return Line{project(line.point()), line.direction()};
    }

    AxisAlignedRect project(const AxisAlignedRect& rect)
    {
        return AxisAlignedRect{
            .center = project(rect.center),
            .width = rect.width * _screenWidth / _screenWorldRect.width,
            .height = rect.height * _screenHeight / _screenWorldRect.height,
        };
    }

    std::optional<Segment> clip(const Line& line)
    {
        return intersectionSegment(line, _screenWorldRect);
    }

private:
    float _screenWidth = 0.f;
    float _screenHeight = 0.f;
    AxisAlignedRect _screenWorldRect;
};

class EditorWindow {
public:
    EditorWindow()
        : _window("balls editor", 1024, 768, SDL_WINDOW_RESIZABLE)
        , _renderer(_window, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
    {
        auto [w, h] = _renderer.size();
        _camera.focus(w, h, Point{50, 50}, 120);

        _ui.add<Button>(_renderer, _font, Point{100, 50}, "button1", []{ std::cerr << "action1\n"; });
        _ui.add<Button>(_renderer, _font, Point{100, 120}, "button2", [] { std::cerr << "action2\n"; });
    }

    bool processInput()
    {
        for (SDL_Event event; SDL_PollEvent(&event); ) {
            if (event.type == SDL_EVENT_QUIT) {
                return false;
            }

            if (_ui.processEvent(event)) {
                continue;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                    event.button.button == SDL_BUTTON_RIGHT) {
                _draggingField = true;
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
                    event.button.button == SDL_BUTTON_RIGHT) {
                _draggingField = false;
            } else if (event.type == SDL_EVENT_MOUSE_MOTION) {
                if (_draggingField) {
                    _camera.moveScreen(event.motion.xrel, event.motion.yrel);
                }
            } else if (event.type == SDL_EVENT_MOUSE_WHEEL) {
                if (event.wheel.y > 0) {
                    _camera.zoomIn(event.wheel.y);
                } else if (event.wheel.y < 0) {
                    _camera.zoomOut(-event.wheel.y);
                }
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                    event.button.button == SDL_BUTTON_LEFT) {
                _bricks.push_back(AxisAlignedRect{
                    .center = _camera.toWorld(Point{event.button.x, event.button.y}),
                    .width = 10,
                    .height = 4,
                });
            }
        }

        return true;
    }

    void update(float delta)
    {
        (void)delta;
    }

    void present()
    {
        _renderer.setDrawColor(50, 50, 50, 255);
        _renderer.clear();

        _renderer.setDrawColor(200, 100, 100, 255);
        _renderer.renderRect(toSdl(_camera.project(_fieldRect)));

        _renderer.setDrawColor(150, 200, 150, 255);
        for (const auto& brick : _bricks) {
            _renderer.fillRect(toSdl(_camera.project(brick)));
        }

        _ui.render(_renderer);

        _renderer.present();
    }

private:
    SDL_FRect toSdl(const AxisAlignedRect& rect)
    {
        return SDL_FRect{
            .x = rect.minX(),
            .y = rect.minY(),
            .w = rect.width,
            .h = rect.height,
        };
    }

    sdl::Window _window;
    sdl::Renderer _renderer;

    sdl::Font _font;

    AxisAlignedRect _fieldRect {Point{50.f, 50.f}, 100.f, 100.f};
    Camera _camera;
    std::vector<AxisAlignedRect> _bricks;
    UI _ui;

    bool _draggingField = false;
};
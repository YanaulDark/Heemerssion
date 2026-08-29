#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>
#include "layout_engine.h"

// Структура цвета (RGBA)
typedef struct {
    uint8_t r, g, b, a;
} Color;

// Основной буфер пикселей
typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t stride;    // Шаг в байтах (обычно width * 4)
    uint8_t* data;      // Указатель на пиксели (RGBA)
} PixelBuffer;

// Функции рендеринга
void render_page(const LayoutNode* nodes, int32_t count, PixelBuffer* buffer);

// Быстрая заливка прямоугольника (с оптимизациями)
void fill_rect(PixelBuffer* buffer, int32_t x, int32_t y, 
               uint32_t width, uint32_t height, Color color);

// Быстрая растеризация текста (заглушка)
void render_text(PixelBuffer* buffer, int32_t x, int32_t y, 
                 const char* text, Color color);

#endif
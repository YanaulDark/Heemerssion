#include "renderer.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Оптимизированная заливка прямоугольника (ручная развертка циклов)
void fill_rect(PixelBuffer* buffer, int32_t x, int32_t y, 
               uint32_t width, uint32_t height, Color color) {
    if (buffer == NULL || buffer->data == NULL) return;
    if (width == 0 || height == 0) return;
    
    // Клиппинг по границам буфера
    int32_t start_x = x < 0 ? 0 : x;
    int32_t start_y = y < 0 ? 0 : y;
    int32_t end_x = (x + width) > buffer->width ? buffer->width : (x + width);
    int32_t end_y = (y + height) > buffer->height ? buffer->height : (y + height);
    
    uint32_t draw_width = end_x - start_x;
    uint32_t draw_height = end_y - start_y;
    
    // Предварительно собираем 4 байта цвета в один uint32_t для быстрой записи
    uint32_t color32 = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
    
    // Оптимизированный цикл (развертка по строкам)
    for (int32_t row = start_y; row < end_y; row++) {
        uint32_t* pixel_row = (uint32_t*)(buffer->data + row * buffer->stride);
        uint32_t* pixel_ptr = pixel_row + start_x;
        
        // Заполняем строку пакетно (по 4 пикселя за раз, если поддерживается)
        uint32_t count = draw_width;
        while (count >= 4) {
            pixel_ptr[0] = color32;
            pixel_ptr[1] = color32;
            pixel_ptr[2] = color32;
            pixel_ptr[3] = color32;
            pixel_ptr += 4;
            count -= 4;
        }
        
        // Остаток
        while (count > 0) {
            *pixel_ptr++ = color32;
            count--;
        }
    }
}

// Рендеринг всей страницы
void render_page(const LayoutNode* nodes, int32_t count, PixelBuffer* buffer) {
    if (count == 0 || nodes == NULL || buffer == NULL || buffer->data == NULL) {
        return;
    }
    
    // Очищаем буфер белым цветом
    Color white = {255, 255, 255, 255};
    fill_rect(buffer, 0, 0, buffer->width, buffer->height, white);
    
    // Для каждого узла рисуем прямоугольник
    for (int32_t i = 0; i < count; i++) {
        const LayoutNode* node = &nodes[i];
        
        // Пропускаем невидимые узлы
        if (!node->is_visible) continue;
        
        // Конвертируем float в целые координаты для пикселей
        int32_t x = (int32_t)node->x;
        int32_t y = (int32_t)node->y;
        uint32_t w = (uint32_t)(node->width + 0.5f);
        uint32_t h = (uint32_t)(node->height + 0.5f);
        
        // Рисуем фон узла (полупрозрачный синий для наглядности)
        Color bg = {100, 150, 255, 200};
        fill_rect(buffer, x, y, w, h, bg);
        
        // Рисуем рамку (бордер) - для простоты используем черный
        if (node->border_top > 0 || node->border_bottom > 0 || 
            node->border_left > 0 || node->border_right > 0) {
            Color border = {0, 0, 0, 255};
            
            // Верхняя рамка
            if (node->border_top > 0) {
                fill_rect(buffer, x, y, w, (uint32_t)node->border_top, border);
            }
            // Нижняя рамка
            if (node->border_bottom > 0) {
                fill_rect(buffer, x, y + h - (uint32_t)node->border_bottom, 
                         w, (uint32_t)node->border_bottom, border);
            }
            // Левая рамка
            if (node->border_left > 0) {
                fill_rect(buffer, x, y, (uint32_t)node->border_left, h, border);
            }
            // Правая рамка
            if (node->border_right > 0) {
                fill_rect(buffer, x + w - (uint32_t)node->border_right, y,
                         (uint32_t)node->border_right, h, border);
            }
        }
    }
}

// Заглушка для текста (в реальном проекте тут был бы freetype/abglyph)
void render_text(PixelBuffer* buffer, int32_t x, int32_t y, 
                 const char* text, Color color) {
    // В реальном движке тут используется шрифтовой растеризатор
    // Для примера просто рисуем черные пиксели
    if (text == NULL) return;
    
    // Простой "примитивный" рендеринг текста - рисуем точки
    for (int i = 0; text[i] != '\0'; i++) {
        // Вместо реального текста рисуем квадратик-заглушку
        fill_rect(buffer, x + i * 8, y, 6, 10, color);
    }
}
#ifndef LAYOUT_ENGINE_H
#define LAYOUT_ENGINE_H

#include <stdint.h>

// Структура узла DOM для верстки (идеально совпадает с Rust-структурой)
typedef struct {
    float x, y;           // Позиция относительно родителя
    float width, height;  // Размеры
    float margin_top, margin_right, margin_bottom, margin_left;
    float padding_top, padding_right, padding_bottom, padding_left;
    float border_top, border_right, border_bottom, border_left;
    
    int32_t parent_index;  // Индекс родителя (-1 если корень)
    int32_t first_child;   // Индекс первого ребенка
    int32_t next_sibling;  // Индекс следующего брата
    
    uint8_t display_type;  // 0 = block, 1 = inline, 2 = flex
    uint8_t position_type; // 0 = static, 1 = relative, 2 = absolute
    uint8_t is_visible;    // 1 = видим, 0 = скрыт
} LayoutNode;

// Основная функция верстки
void calculate_layout(LayoutNode* nodes, int32_t count, float viewport_width);

// Вспомогательная функция для отладки
void print_layout_tree(const LayoutNode* nodes, int32_t count);

#endif
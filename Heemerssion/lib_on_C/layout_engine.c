#include "layout_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// SIMD-оптимизированное вычисление ширины (используем SSE2 через интринсики)
#ifdef __SSE2__
#include <emmintrin.h>
#endif

// Рекурсивный расчет позиций (основной алгоритм)
static void layout_node(LayoutNode* nodes, int32_t idx, float parent_x, float parent_y, float parent_width) {
    LayoutNode* node = &nodes[idx];
    
    // Если элемент скрыт - пропускаем
    if (!node->is_visible) return;
    
    // Вычисляем "коробочную модель" (Box Model)
    float content_width = node->width;
    float content_height = node->height;
    
    // Полная ширина с учетом margin/padding/border
    float total_width = content_width 
        + node->padding_left + node->padding_right
        + node->border_left + node->border_right;
    
    float total_height = content_height 
        + node->padding_top + node->padding_bottom
        + node->border_top + node->border_bottom;
    
    // Расчет позиции в зависимости от типа позиционирования
    switch (node->position_type) {
        case 0: // static (обычный поток)
            node->x = parent_x + node->margin_left + node->border_left + node->padding_left;
            node->y = parent_y + node->margin_top + node->border_top + node->padding_top;
            break;
        case 1: // relative (относительно себя)
            node->x += parent_x;
            node->y += parent_y;
            break;
        case 2: // absolute (абсолютное позиционирование)
            // В реальном движке тут бы был поиск ближайшего positioned-родителя
            // Для простоты считаем от корня
            break;
    }
    
    // Рекурсивно обрабатываем детей
    if (node->first_child != -1) {
        int32_t child = node->first_child;
        float child_x = node->x + node->padding_left + node->border_left;
        float child_y = node->y + node->padding_top + node->border_top;
        float child_width = node->width - node->padding_left - node->padding_right 
                           - node->border_left - node->border_right;
        
        while (child != -1) {
            layout_node(nodes, child, child_x, child_y, child_width);
            child = nodes[child].next_sibling;
            
            // Для блочных элементов смещаем по Y
            if (nodes[child].display_type == 0) {
                child_y += nodes[child].height + nodes[child].margin_top + nodes[child].margin_bottom;
            }
        }
    }
}

// SIMD-оптимизированная обработка массива узлов (пакетная обработка)
void calculate_layout(LayoutNode* nodes, int32_t count, float viewport_width) {
    if (count == 0 || nodes == NULL) return;
    
    // Сбрасываем позиции для всех узлов
    #ifdef __SSE2__
    // Используем SIMD для быстрого обнуления (если структура выровнена)
    __m128 zero = _mm_setzero_ps();
    for (int i = 0; i < count; i++) {
        // Обнуляем x и y (первые 8 байт структуры)
        // В реальном коде нужно быть осторожным с выравниванием
        float* pos = (float*)&nodes[i];
        _mm_store_ps(pos, zero);
    }
    #else
    for (int i = 0; i < count; i++) {
        nodes[i].x = 0.0f;
        nodes[i].y = 0.0f;
    }
    #endif
    
    // Запускаем рекурсивный расчет с корневого элемента (индекс 0)
    if (count > 0) {
        layout_node(nodes, 0, 0.0f, 0.0f, viewport_width);
    }
}

// Отладочная печать дерева (только для проверки)
void print_layout_tree(const LayoutNode* nodes, int32_t count) {
    for (int32_t i = 0; i < count; i++) {
        const LayoutNode* node = &nodes[i];
        printf("Node %d: pos=(%.2f, %.2f) size=(%.2f x %.2f) parent=%d\n",
               i, node->x, node->y, node->width, node->height, node->parent_index);
    }
}
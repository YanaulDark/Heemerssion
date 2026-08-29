#ifndef LAYOUT_ENGINE_H
#define LAYOUT_ENGINE_H

#include <stdint.h>

// ==============================================
// СТРУКТУРЫ ДАННЫХ
// ==============================================

// Структура узла DOM для верстки (идеально совпадает с Rust-структурой)
typedef struct {
    float x, y;                              // Позиция относительно родителя
    float width, height;                     // Размеры содержимого
    
    // Отступы (margin)
    float margin_top;
    float margin_right;
    float margin_bottom;
    float margin_left;
    
    // Внутренние отступы (padding)
    float padding_top;
    float padding_right;
    float padding_bottom;
    float padding_left;
    
    // Рамки (border)
    float border_top;
    float border_right;
    float border_bottom;
    float border_left;
    
    // Связи в DOM-дереве
    int32_t parent_index;                    // Индекс родителя (-1 если корень)
    int32_t first_child;                     // Индекс первого ребенка (-1 если нет)
    int32_t next_sibling;                    // Индекс следующего брата (-1 если нет)
    
    // Типы отображения
    uint8_t display_type;                    // 0 = block, 1 = inline, 2 = flex, 3 = grid
    uint8_t position_type;                   // 0 = static, 1 = relative, 2 = absolute, 3 = fixed
    
    // Состояние
    uint8_t is_visible;                      // 1 = видим, 0 = скрыт (display: none)
    uint8_t has_overflow;                    // 1 = overflow: hidden/scroll
    
    // Для Flexbox (если display_type == 2)
    float flex_grow;                         // Коэффициент растяжения
    float flex_shrink;                       // Коэффициент сжатия
    float flex_basis;                        // Базовая ширина/высота
    
    // Для Grid (если display_type == 3)
    uint8_t grid_row_start;
    uint8_t grid_row_end;
    uint8_t grid_column_start;
    uint8_t grid_column_end;
    
    // Зарезервировано для будущих расширений
    float reserved[4];
} LayoutNode;

// ==============================================
// ОСНОВНЫЕ ФУНКЦИИ ВЕРСТКИ
// ==============================================

/**
 * Главная функция расчета макета страницы
 * 
 * @param nodes          Массив узлов DOM
 * @param count          Количество узлов в массиве
 * @param viewport_width Ширина вьюпорта в пикселях
 */
void calculate_layout(LayoutNode* nodes, int32_t count, float viewport_width);

/**
 * Расчет макета с поддержкой Flexbox
 * 
 * @param nodes          Массив узлов DOM
 * @param count          Количество узлов
 * @param viewport_width Ширина вьюпорта
 * @param viewport_height Высота вьюпорта
 */
void calculate_layout_flex(LayoutNode* nodes, int32_t count, 
                           float viewport_width, float viewport_height);

/**
 * Пересчет позиций после скролла или изменения размеров
 * 
 * @param nodes          Массив узлов
 * @param count          Количество узлов
 * @param scroll_x       Смещение по X
 * @param scroll_y       Смещение по Y
 */
void apply_scroll(LayoutNode* nodes, int32_t count, 
                  float scroll_x, float scroll_y);

/**
 * Проверка пересечения двух прямоугольников
 * (используется для оптимизации отрисовки - clipping)
 * 
 * @param ax, ay, aw, ah  Прямоугольник A
 * @param bx, by, bw, bh  Прямоугольник B
 * @return 1 если пересекаются, 0 если нет
 */
int rect_intersects(float ax, float ay, float aw, float ah,
                    float bx, float by, float bw, float bh);

// ==============================================
// ОТЛАДОЧНЫЕ ФУНКЦИИ
// ==============================================

/**
 * Печать всего дерева в консоль (для отладки)
 * 
 * @param nodes Массив узлов
 * @param count Количество узлов
 */
void print_layout_tree(const LayoutNode* nodes, int32_t count);

/**
 * Печать конкретного узла с его детьми (рекурсивно)
 * 
 * @param nodes Массив узлов
 * @param index Индекс узла
 * @param depth Глубина отступа (для форматирования)
 */
void print_node_recursive(const LayoutNode* nodes, int32_t index, int32_t depth);

/**
 * Проверка целостности дерева (поиск циклов, некорректных ссылок)
 * 
 * @param nodes Массив узлов
 * @param count Количество узлов
 * @return 1 если дерево корректно, 0 если есть ошибки
 */
int validate_tree(const LayoutNode* nodes, int32_t count);

// ==============================================
// ФУНКЦИИ ДЛЯ РАБОТЫ С ПОТОКАМИ
// ==============================================

/**
 * Параллельная версия расчета макета (использует OpenMP если доступен)
 * 
 * @param nodes          Массив узлов
 * @param count          Количество узлов
 * @param viewport_width Ширина вьюпорта
 * @param num_threads    Количество потоков (0 = авто)
 */
void calculate_layout_parallel(LayoutNode* nodes, int32_t count, 
                               float viewport_width, int32_t num_threads);

#endif // LAYOUT_ENGINE_H
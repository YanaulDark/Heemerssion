#include <string.h>
#include <stdlib.h>
#include <math.h>

// Быстрая функция смешивания цветов (альфа-блендинг)
void blend_colors(uint8_t* dest, uint8_t* src, float alpha) {
    if (alpha <= 0.0f) return;
    if (alpha >= 1.0f) {
        memcpy(dest, src, 4);
        return;
    }
    
    // Оптимизированное смешивание
    float inv_alpha = 1.0f - alpha;
    dest[0] = (uint8_t)(dest[0] * inv_alpha + src[0] * alpha);
    dest[1] = (uint8_t)(dest[1] * inv_alpha + src[1] * alpha);
    dest[2] = (uint8_t)(dest[2] * inv_alpha + src[2] * alpha);
    dest[3] = (uint8_t)(dest[3] * inv_alpha + src[3] * alpha);
}

// Вычисление матрицы трансформации для вьюпорта
void calculate_viewport_matrix(float width, float height, float matrix[16]) {
    // Инициализация единичной матрицы
    memset(matrix, 0, sizeof(float) * 16);
    matrix[0] = 2.0f / width;
    matrix[5] = -2.0f / height;
    matrix[10] = 1.0f;
    matrix[12] = -1.0f;
    matrix[13] = 1.0f;
    matrix[15] = 1.0f;
}

// Быстрое преобразование цветового пространства (RGB -> Grayscale)
void rgb_to_grayscale(uint8_t* pixels, uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        uint8_t* p = pixels + i * 4;
        uint8_t gray = (uint8_t)(0.299f * p[0] + 0.587f * p[1] + 0.114f * p[2]);
        p[0] = gray;
        p[1] = gray;
        p[2] = gray;
        // Альфа-канал не трогаем
    }
}
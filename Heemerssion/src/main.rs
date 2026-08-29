// Включаем сгенерированные биндинги 
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

fn main() {
    // Вызываем C-функцию для расчета макета страницы
    // Допустим, в C мы передаем указатель на массив DOM-нод
    let mut result: f32 = 0.0;
    unsafe {
        // Предположим, что в C есть функция:
        // void calculate_layout(float* dom_data, int len, float* out_result);
        calculate_layout(std::ptr::null(), 0, &mut result);
    }
    println!("Layout calculated by C: {}", result);
}

// Теперь можно использовать новые функции
unsafe {
    // Проверка пересечения двух прямоугольников
    let intersects = rect_intersects(
        10.0, 20.0, 100.0, 50.0,  // rect A
        50.0, 30.0, 80.0, 60.0    // rect B
    );
    println!("Прямоугольники пересекаются: {}", intersects != 0);
    
    // Параллельный расчет (если OpenMP доступен)
    calculate_layout_parallel(nodes.as_mut_ptr(), nodes.len() as i32, 800.0, 4);
    
    // Применяем скролл
    apply_scroll(nodes.as_mut_ptr(), nodes.len() as i32, 100.0, 200.0);
    
    // Валидация дерева
    let valid = validate_tree(nodes.as_ptr(), nodes.len() as i32);
    if valid == 0 {
        eprintln!("❌ Дерево повреждено!");
    }
}
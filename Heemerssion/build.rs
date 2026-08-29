use std::env;
use std::path::PathBuf;

fn main() {
    // Указываем компилятору C где искать файлы
    println!("cargo:rustc-link-search=native={}", 
        PathBuf::from("c_lib").display()
    );

    //  Собираем файлы в статическую библиотеку
    cc::Build::new()
        .file("c_lib/layout_engine.c")
        .file("c_lib/renderer.c")
        .include("c_lib")           // Путь для .h файлов
        .flag("-O3")                // Максимальная оптимизация для C
        .flag("-march=native")      // Векторизация под ваш процессор
        .compile("clayout");        // Имя библиотеки (libclayout.a)

    let bindings = bindgen::Builder::default()
        .header("c_lib/layout_engine.h")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Не удалось сгенерировать обертки");
    
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings.write_to_file(out_path.join("bindings.rs"))
        .expect("Не удалось записать биндинги");
}
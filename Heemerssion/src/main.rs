// Подключение модулей
mod dom_hub { pub mod dom; }
mod css_hub { pub mod css; }
mod graphics_hub { pub mod graphics; }
mod html_hub { pub mod html; }
mod layout_hub { pub mod layout; }
mod net_hub { pub mod net; }
mod paint_hub { pub mod paint; }
mod scripting_hub { pub mod scripting; }
mod shell_hub { pub mod shell; }
mod style_hub { pub mod style; }

use std::io;

fn main() {
    input();
}

fn input() -> u8 {
    let mut user_input = String::new();
    println!("Please write command:"); // ← ВАЖНО: ;
                                       //
    if io::stdin().read_line(&mut user_input).is_ok() {
        match user_input.trim() {
            "css" => println!("{}", css_hub::css::css_go()),
            "dom" => println!("{}", dom_hub::dom::dom_go()),
            "graphics" => println!("{}", graphics_hub::graphics::graphics_go()),
            "html" => println!("{}", html_hub::html::html_go()),
            "layout" => println!("{}", layout_hub::layout::layout_go()),
            "net" => println!("{}", net_hub::net::net_go()),
            "paint" => println!("{}", paint_hub::paint::paint_go()),
            "scripting" => println!("{}", scripting_hub::scripting::scripting_go()),
            "shell" => println!("{}", shell_hub::shell::shell_go()),
            "style" => println!("{}", style_hub::style::style_go()),
            _ => println!("This not module "), 
        }
    }
    return 0
}

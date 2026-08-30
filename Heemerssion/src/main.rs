//Подключение модулей
mod dom_hub {
    pub mod dom;
}

mod css_hub{
    pub mod css;
}

mod graphics_hub {
    pub mod graphics;
}

mod html_hub {
    pub mod html;
}

mod layout_hub{
    pub mod layout;
}

mod net_hub {
    pub mod net;
}

mod paint_hub{
    pub mod paint;
}

mod scripting_hub{
    pub mod scripting;
}

mod shell_hub {
    pub mod shell
}

mod style_hub {
    pub mod style
}

//  Подключаем input-output из std
use std::io;

fn main() {
    let mut user_input = String::new();
    println!("Please write command:")

    if io::stdin().read_line(&mut user_input).is_ok() {
        match user_input.trim() {
            //CSS  модуль
            "css" => println!("{}", css::css_go()),

            //DOM модуль
            "dom" => println!("{}", dom::dom_go()),

            //GRAPHICS модуль
            "graphics" => println!("{}", garphics::graphics_go()),
            
            //HTML модуль
            "html" => println!("{}", html::html_go()),
            
            //LAYOUT модуль
            "layout" => println!("{}", layout::layout_go()),
            
            //NET модуль
            "net" => println!("{}", net::net_go()),
            
            //Paint модуль
            "paint" => println!("{}", paint::paint_go()),
            
            //SCRIPTING модуль
            "scripting" => println!("{}", scripting::scripting_go()),
            
            //SHELL модуль
            "shell" => println!("{}", shell::shell_go()),
            
            //STYLE модуль
            "style" => println!("{}", style::style_go()),
            _ => println!("Is not correct command"),

        }
    }
    
}

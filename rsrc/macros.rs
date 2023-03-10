use std::alloc::{alloc, Layout};
// create new memory block
pub unsafe fn new<S> () -> (Layout, *mut u8) {
       let layout = Layout::new::<S>(); 
       let ptr = alloc(layout);
       return (layout,ptr);
}
// or operator
pub const fn or(a:bool, b:bool) -> bool {
        let c = if a as u8 + b as u8 == 1 {true} else {false};
        c
}
// MinMax
pub const fn min_max(a:i32,b:i32) -> i32 {
    let c = if a < b  {a} else {b};
    c
}
// Absolute value
pub const fn abs(a:i32) -> i32 {
    let b = if a < 0 {a * -1} else {a};  
    b
}
///CLI colors
pub const RED:&str = "\x1B[31m";
pub const GREEN:&str = "\x1B[32m";
pub const YELLOW:&str = "\x1B[33m";
pub const BLUE:&str = "\x1B[34m";
pub const MAGENTA:&str = "\x1B[35m";
pub const CYAN:&str = "\x1B[36m";
pub const WHITE:&str = "\x1B[37m";
pub const RESET:&str = "\x1B[0m";
pub const BOLD:&str = "\x1B[1m";


pub fn glms_warning(ap:&[&str]) {
    for p in ap {
    println!("{}(GLMS)(WARNING): {} \n {}",YELLOW,p,RESET);
    }
}
pub fn glms_warning_return(ret:i32, ap:&[&str]) -> i32 {
    println!("\n****\n");
    for p in ap {
    println!("{}(GLMS)(WARNING): {} \n {}",YELLOW,p,RESET);
    }
    println!("\n****\n");
    return ret;
}
pub fn glms_error(e:&[&str]) {
    for er in e {
        println!("{}{}(GLMS)(ERROR): {} {}",BOLD,RED,er,RESET)
    }
    std::process::exit(0);
}

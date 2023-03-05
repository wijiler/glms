#![feature(c_variadic)]
pub mod macros;
pub mod io;
pub mod token;
pub mod lexer;
pub use std::alloc::dealloc;
/// Rust "rewrite" of glms so I can compile it on my mac/windows machines


pub fn main () {
    println!("glms");
    macros::glms_warning(true,&["stinky error"]);
}

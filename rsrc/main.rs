pub mod macros;
pub mod io;
pub mod token;
pub mod lexer;
/// Rust "rewrite" of glms so I can compile it on my mac/windows machines


pub fn main () {
    println!("glms");
    macros::glms_error(&["Oops"]);
}

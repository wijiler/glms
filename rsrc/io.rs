use std::fs;
use std::path::Path;
use std::process;
pub fn glms_file_exists(path: &str) -> bool { let exists = Path::new(path).exists(); exists }

pub fn glms_get_file_contents(path: &str) -> String {
    if !glms_file_exists(path) {
        println!("Error: No such file as {}",path);
        process::exit(-1);
    }
    let file = fs::read_to_string(path).unwrap();
    if file.len() <= 0 {
        println!("Error: File {} is empty",path);
        process::exit(-1);
    } 
    return file;
}

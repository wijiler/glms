use crate::io::*;
use crate::token::*;
use crate::macros::{glms_warning,glms_error};
use std::process::exit;
pub struct GLMSTokenMap {
    pattern:&'static str,
    ttype:TOK 
}
impl GLMSTokenMap {
    pub const fn new (p:&'static str,tt:TOK) -> Self {
        Self {
            pattern:p,
            ttype:tt
        }
    }
}
pub const TOKEN_MAP:[GLMSTokenMap;31] =
    [
    GLMSTokenMap::new("import", TOK::GLMS_TOKEN_TYPE_SPECIAL_IMPORT),
    GLMSTokenMap::new("as", TOK::GLMS_TOKEN_TYPE_SPECIAL_AS),
    GLMSTokenMap::new("function", TOK::GLMS_TOKEN_TYPE_SPECIAL_FUNCTION),
    GLMSTokenMap::new("return", TOK::GLMS_TOKEN_TYPE_SPECIAL_RETURN),
    GLMSTokenMap::new("if", TOK::GLMS_TOKEN_TYPE_SPECIAL_IF),
    GLMSTokenMap::new("else", TOK::GLMS_TOKEN_TYPE_SPECIAL_ELSE),
    GLMSTokenMap::new("false", TOK::GLMS_TOKEN_TYPE_SPECIAL_FALSE),
    GLMSTokenMap::new("true", TOK::GLMS_TOKEN_TYPE_SPECIAL_TRUE),
    GLMSTokenMap::new("for", TOK::GLMS_TOKEN_TYPE_SPECIAL_FOR),
    GLMSTokenMap::new("switch", TOK::GLMS_TOKEN_TYPE_SPECIAL_SWITCH),
    GLMSTokenMap::new("case", TOK::GLMS_TOKEN_TYPE_SPECIAL_CASE),
    GLMSTokenMap::new("break", TOK::GLMS_TOKEN_TYPE_SPECIAL_BREAK),
    GLMSTokenMap::new("while", TOK::GLMS_TOKEN_TYPE_SPECIAL_WHILE),
    GLMSTokenMap::new("string", TOK::GLMS_TOKEN_TYPE_SPECIAL_STRING),
    GLMSTokenMap::new("number", TOK::GLMS_TOKEN_TYPE_SPECIAL_NUMBER),
    GLMSTokenMap::new("int", TOK::GLMS_TOKEN_TYPE_SPECIAL_INT),
    GLMSTokenMap::new("float", TOK::GLMS_TOKEN_TYPE_SPECIAL_FLOAT),
    GLMSTokenMap::new("null", TOK::GLMS_TOKEN_TYPE_SPECIAL_NULL),
    GLMSTokenMap::new("array", TOK::GLMS_TOKEN_TYPE_SPECIAL_ARRAY),
    GLMSTokenMap::new("object", TOK::GLMS_TOKEN_TYPE_SPECIAL_OBJECT),
    GLMSTokenMap::new("struct", TOK::GLMS_TOKEN_TYPE_SPECIAL_STRUCT),
    GLMSTokenMap::new("let", TOK::GLMS_TOKEN_TYPE_SPECIAL_LET),
    GLMSTokenMap::new("const", TOK::GLMS_TOKEN_TYPE_SPECIAL_CONST),
    GLMSTokenMap::new("typedef", TOK::GLMS_TOKEN_TYPE_SPECIAL_TYPEDEF),
    GLMSTokenMap::new("vec2", TOK::GLMS_TOKEN_TYPE_SPECIAL_VEC2),
    GLMSTokenMap::new("vec3", TOK::GLMS_TOKEN_TYPE_SPECIAL_VEC3),
    GLMSTokenMap::new("vec3", TOK::GLMS_TOKEN_TYPE_SPECIAL_VEC3),
    GLMSTokenMap::new("mat3", TOK::GLMS_TOKEN_TYPE_SPECIAL_MAT3),
    GLMSTokenMap::new("mat4", TOK::GLMS_TOKEN_TYPE_SPECIAL_MAT4),
    GLMSTokenMap::new("bool", TOK::GLMS_TOKEN_TYPE_SPECIAL_BOOL),
    GLMSTokenMap::new("enum", TOK::GLMS_TOKEN_TYPE_SPECIAL_ENUM)
    ];

pub struct Lexer {
   it:usize,
   length:usize,
   cc:char,
   source:String,
   initialized:bool
}
impl Lexer {
pub fn init(&mut self,s:String) -> Self {
     if &s.len() <= &0 {glms_error(&["Lexer is not able to read file (EMPTY_FILE_ERROR)"]);}
     let i: usize = 0;
     let sou= &s;
     let le= s.len();
     let c:char = s.as_bytes()[i] as char;
     Lexer { it: i, length: le, cc: c, source: sou.to_owned(), initialized: true } 
    }
/// peek x ahead in the lexer
pub fn peek(&mut self,p:usize) -> char {
    if  self.it + p >= self.length {glms_error(&["Lexer peek not within file bounds (LEXER_OUTOFBOUNDS_ERROR)"]);};
    return self.source.as_bytes()[self.it + p] as char
   }
/// detect if whitespace
pub fn lexer_is_whitespace(&self) -> bool { if self.cc == ' ' || self.cc == '\t' || self.cc == '\n' {true} else {false}}
/// detect if comment
pub fn lexer_is_comment(&mut self) -> bool {if self.cc == '/' && self.peek(1) == '/' {true} else {false}}
pub fn lexer_advance(mut self) {
    if self.it >= self.length {glms_error(&["Lexer cannot adance (LEXER_OUTOFBOUNDS_ERROR)"])}
    self.it += 1;
    self.cc = self.source.as_bytes()[self.it] as  char;
    }
}

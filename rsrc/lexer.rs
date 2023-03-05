use crate::io::*;
use crate::token::*;
struct GLMSTokenMap {
    pattern:String,
    ttype:TOK 
}
impl GLMSTokenMap {
    pub fn new (p:&str,tt:TOK) -> Self {
        Self {
            pattern:p.to_string(),
            ttype:tt
        }
    }
}
const TOKEN_MAP:Vec<GLMSTokenMap> = [
    GLMSTokenMap::new("import", GLMS_TOKEN_TYPE_SPECIAL_IMPORT),
    GLMSTokenMap::new("as", GLMS_TOKEN_TYPE_SPECIAL_AS),
    GLMSTokenMap::new("function", GLMS_TOKEN_TYPE_SPECIAL_FUNCTION),
    GLMSTokenMap::new("return", GLMS_TOKEN_TYPE_SPECIAL_RETURN),
    GLMSTokenMap::new("if", GLMS_TOKEN_TYPE_SPECIAL_IF),
    GLMSTokenMap::new("else", GLMS_TOKEN_TYPE_SPECIAL_ELSE),
    GLMSTokenMap::new("false", GLMS_TOKEN_TYPE_SPECIAL_FALSE),
    GLMSTokenMap::new("true", GLMS_TOKEN_TYPE_SPECIAL_TRUE),
    GLMSTokenMap::new("for", GLMS_TOKEN_TYPE_SPECIAL_FOR),
    GLMSTokenMap::new("switch", GLMS_TOKEN_TYPE_SPECIAL_SWITCH),
    GLMSTokenMap::new("case", GLMS_TOKEN_TYPE_SPECIAL_CASE),
    GLMSTokenMap::new("break", GLMS_TOKEN_TYPE_SPECIAL_BREAK),
    GLMSTokenMap::new("while", GLMS_TOKEN_TYPE_SPECIAL_WHILE),
    GLMSTokenMap::new("string", GLMS_TOKEN_TYPE_SPECIAL_STRING),
    GLMSTokenMap::new("number", GLMS_TOKEN_TYPE_SPECIAL_NUMBER),
    GLMSTokenMap::new("int", GLMS_TOKEN_TYPE_SPECIAL_INT),
    GLMSTokenMap::new("float", GLMS_TOKEN_TYPE_SPECIAL_FLOAT),
    GLMSTokenMap::new("null", GLMS_TOKEN_TYPE_SPECIAL_NULL),
    GLMSTokenMap::new("array", GLMS_TOKEN_TYPE_SPECIAL_ARRAY),
    GLMSTokenMap::new("object", GLMS_TOKEN_TYPE_SPECIAL_OBJECT),
    GLMSTokenMap::new("struct", GLMS_TOKEN_TYPE_SPECIAL_STRUCT),
    GLMSTokenMap::new("let", GLMS_TOKEN_TYPE_SPECIAL_LET),
    GLMSTokenMap::new("const", GLMS_TOKEN_TYPE_SPECIAL_CONST),
    GLMSTokenMap::new("typedef", GLMS_TOKEN_TYPE_SPECIAL_TYPEDEF),
    GLMSTokenMap::new("vec2", GLMS_TOKEN_TYPE_SPECIAL_VEC2),
    GLMSTokenMap::new("vec3", GLMS_TOKEN_TYPE_SPECIAL_VEC3),
    GLMSTokenMap::new("vec3", GLMS_TOKEN_TYPE_SPECIAL_VEC3),
    GLMSTokenMap::new("mat3", GLMS_TOKEN_TYPE_SPECIAL_MAT3),
    GLMSTokenMap::new("mat4", GLMS_TOKEN_TYPE_SPECIAL_MAT4),
    GLMSTokenMap::new("bool", GLMS_TOKEN_TYPE_SPECIAL_BOOL),
    GLMSTokenMap::new("enum", GLMS_TOKEN_TYPE_SPECIAL_ENUM)
];



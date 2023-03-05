 #[derive(PartialEq)]
pub enum TOK {
  GLMS_TOKEN_TYPE_NONE                                                    ,
  GLMS_TOKEN_TYPE_EOF                                                     ,
  GLMS_TOKEN_TYPE_LBRACE                                                  ,
  GLMS_TOKEN_TYPE_RBRACE                                                  ,
  GLMS_TOKEN_TYPE_LBRACKET                                                ,
  GLMS_TOKEN_TYPE_RBRACKET                                                ,
  GLMS_TOKEN_TYPE_LPAREN                                                  ,
  GLMS_TOKEN_TYPE_RPAREN                                                  ,
  GLMS_TOKEN_TYPE_SEMI                                                    ,
  GLMS_TOKEN_TYPE_COMMA                                                   ,
  GLMS_TOKEN_TYPE_COLON                                                   ,
  GLMS_TOKEN_TYPE_DOT                                                     ,
  GLMS_TOKEN_TYPE_ID                                                      ,
  GLMS_TOKEN_TYPE_STRING                                                  ,
  GLMS_TOKEN_TYPE_TEMPLATE_STRING                                         ,
  GLMS_TOKEN_TYPE_NUMBER                                                  ,
  GLMS_TOKEN_TYPE_INT                                                     ,
  GLMS_TOKEN_TYPE_FLOAT                                                   ,
  GLMS_TOKEN_TYPE_ADD                                                     ,
  GLMS_TOKEN_TYPE_ADD_ADD                                                 ,
  GLMS_TOKEN_TYPE_ADD_EQUALS                                              ,
  GLMS_TOKEN_TYPE_AND_AND                                                 ,
  GLMS_TOKEN_TYPE_PIPE_PIPE                                               ,
  GLMS_TOKEN_TYPE_MUL                                                     ,
  GLMS_TOKEN_TYPE_MUL_EQUALS                                              ,
  GLMS_TOKEN_TYPE_SUB                                                     ,
  GLMS_TOKEN_TYPE_SUB_SUB                                                 ,
  GLMS_TOKEN_TYPE_SUB_EQUALS                                              ,
  GLMS_TOKEN_TYPE_DIV                                                     ,
  GLMS_TOKEN_TYPE_DIV_EQUALS                                              ,
  GLMS_TOKEN_TYPE_PERCENT                                                 ,
  GLMS_TOKEN_TYPE_EQUALS                                                  ,
  GLMS_TOKEN_TYPE_EQUALS_EQUALS                                           ,
  GLMS_TOKEN_TYPE_GT                                                      ,
  GLMS_TOKEN_TYPE_LT                                                      ,
  GLMS_TOKEN_TYPE_GTE                                                     ,
  GLMS_TOKEN_TYPE_LTE                                                     ,
  GLMS_TOKEN_TYPE_EXCLAM                                                  ,
  GLMS_TOKEN_TYPE_SPECIAL_NULL                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_IMPORT                                          ,
  GLMS_TOKEN_TYPE_SPECIAL_AS                                              ,
  GLMS_TOKEN_TYPE_SPECIAL_TYPEDEF                                         ,
  GLMS_TOKEN_TYPE_SPECIAL_USER_TYPE                                       ,
  GLMS_TOKEN_TYPE_SPECIAL_CONST                                           ,
  GLMS_TOKEN_TYPE_SPECIAL_LET                                             ,
  GLMS_TOKEN_TYPE_SPECIAL_STRUCT                                          ,
  GLMS_TOKEN_TYPE_SPECIAL_VEC2                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_VEC3                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_VEC4                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_MAT3                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_MAT4                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_ENUM                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_STRING                                          ,
  GLMS_TOKEN_TYPE_SPECIAL_NUMBER                                          ,
  GLMS_TOKEN_TYPE_SPECIAL_INT                                             ,
  GLMS_TOKEN_TYPE_SPECIAL_FLOAT                                           ,
  GLMS_TOKEN_TYPE_SPECIAL_ARRAY                                           ,
  GLMS_TOKEN_TYPE_SPECIAL_OBJECT                                          ,
  GLMS_TOKEN_TYPE_SPECIAL_FOR                                             ,
  GLMS_TOKEN_TYPE_SPECIAL_SWITCH                                          ,
  GLMS_TOKEN_TYPE_SPECIAL_CASE                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_BREAK                                           ,
  GLMS_TOKEN_TYPE_SPECIAL_WHILE                                           ,
  GLMS_TOKEN_TYPE_SPECIAL_FALSE                                           ,
  GLMS_TOKEN_TYPE_SPECIAL_TRUE                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_BOOL                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_IF                                              ,
  GLMS_TOKEN_TYPE_SPECIAL_ELSE                                            ,
  GLMS_TOKEN_TYPE_SPECIAL_FUNCTION                                        ,
  GLMS_TOKEN_TYPE_SPECIAL_RETURN
}

pub fn glms_token_type_is_flag(tt:TOK) -> bool {
    if   tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_USER_TYPE ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_CONST ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_NUMBER ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_INT ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_FLOAT ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_ARRAY ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_OBJECT ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_STRING ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_LET ||
         tt == TOK::GLMS_TOKEN_TYPE_SPECIAL_BOOL {
            return true;
         }
    else { return false; }
}

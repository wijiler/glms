#ifndef GLMS_AST_TYPE_H
#define GLMS_AST_TYPE_H
#include <glms/macros.h>

#define GLMS_FOREACH_AST_TYPE(TOK)           \
  TOK(GLMS_AST_TYPE_EOF)                     \
  TOK(GLMS_AST_TYPE_NOOP)\
  TOK(GLMS_AST_TYPE_UNDEFINED)\
  TOK(GLMS_AST_TYPE_COMPOUND)\
  TOK(GLMS_AST_TYPE_ID)\
  TOK(GLMS_AST_TYPE_STACK_PTR)\
  TOK(GLMS_AST_TYPE_STRING)\
  TOK(GLMS_AST_TYPE_NUMBER)\
  TOK(GLMS_AST_TYPE_BOOL)\
  TOK(GLMS_AST_TYPE_ARRAY)\
  TOK(GLMS_AST_TYPE_VEC2)\
  TOK(GLMS_AST_TYPE_VEC3)\
  TOK(GLMS_AST_TYPE_VEC4)\
  TOK(GLMS_AST_TYPE_TYPEDEF)\
  TOK(GLMS_AST_TYPE_OBJECT)\
  TOK(GLMS_AST_TYPE_STRUCT)\
  TOK(GLMS_AST_TYPE_ENUM)\
  TOK(GLMS_AST_TYPE_BINOP)\
  TOK(GLMS_AST_TYPE_UNOP)\
  TOK(GLMS_AST_TYPE_ACCESS)\
  TOK(GLMS_AST_TYPE_BLOCK)\
  TOK(GLMS_AST_TYPE_FOR)\
  TOK(GLMS_AST_TYPE_CALL)\
  TOK(GLMS_AST_TYPE_FUNC)\
  TOK(GLMS_AST_TYPE_RETURN)

typedef enum {
  GLMS_FOREACH_AST_TYPE(GLMS_GENERATE_ENUM)
} GLMSASTType;

static const char *GLMS_AST_TYPE_STR[] = {
    GLMS_FOREACH_AST_TYPE(GLMS_GENERATE_STRING)
};


#endif

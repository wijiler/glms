#include <jscript/env.h>
#include <jscript/macros.h>
#include <jscript/parser.h>

int jscript_parser_init(JSCRIPTParser *parser, JSCRIPTEnv *env) {
  if (!parser || !env)
    return 0;
  if (parser->initialized)
    return 1;

  parser->initialized = true;
  parser->env = env;
  jscript_lexer_next(&parser->env->lexer, &parser->token);

  return 1;
}

int jscript_parser_eat(JSCRIPTParser *parser, JSCRIPTTokenType token_type) {
  if (!parser)
    return 0;
  if (!parser->initialized)
    JSCRIPT_WARNING_RETURN(0, stderr, "parser not initialized.\n");

  if (parser->token.type != token_type) {
    JSCRIPT_WARNING_RETURN(0, stderr, "Unexpected token `%s`\n",
                           JSCRIPT_TOKEN_TYPE_STR[parser->token.type]);
  }

  if (!jscript_lexer_next(&parser->env->lexer, &parser->token))
    return 0;

  return 1;
}

static JSCRIPTAST* jscript_parser_error(JSCRIPTParser* parser) {
  JSCRIPT_WARNING(stderr, "Unexpected token `%s`\n", JSCRIPT_TOKEN_TYPE_STR[parser->token.type]);
  parser->error = true;

  return jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_EOF);
}

static JSCRIPTAST *jscript_parser_parse_noop(JSCRIPTParser *parser) {
  JSCRIPTAST *ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_NOOP);
  return ast;
}

static JSCRIPTAST *jscript_parser_parse_eof(JSCRIPTParser *parser) {
  JSCRIPTAST *ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_EOF);
  return ast;
}

JSCRIPTAST *jscript_parser_parse_id(JSCRIPTParser *parser, bool skip_next) {
  JSCRIPTAST *ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_ID);
  ast->as.id.value = parser->token.value;
  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_ID);

  if (skip_next) return ast;

  JSCRIPTTokenType next_type = parser->token.type;

  switch (next_type) {
    case JSCRIPT_TOKEN_TYPE_LPAREN: {
      return jscript_parser_parse_call(parser, ast);
    }; break;
    case JSCRIPT_TOKEN_TYPE_ID: {
      JSCRIPTAST *next_ast =
          jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_ID);
      next_ast->as.id.flag = ast;
      next_ast->as.id.value = parser->token.value;
      jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_ID);

      ast = next_ast;
    }; break;
    default: {}; break;
  }

  return ast;
}

JSCRIPTAST *jscript_parser_parse_number(JSCRIPTParser *parser) {
  JSCRIPTAST *ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_NUMBER);
  ast->as.number.value =
      atof(jscript_string_view_get_value(&parser->token.value));
  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_NUMBER);
  return ast;
}

JSCRIPTAST* jscript_parser_parse_string(JSCRIPTParser* parser) {
  JSCRIPTAST *ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_STRING);
  ast->as.string.value = parser->token.value;
  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_STRING);
  return ast;
}

JSCRIPTAST *jscript_parser_parse_binop(JSCRIPTParser *parser,
                                       JSCRIPTAST *left) {
  JSCRIPTAST *ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_BINOP);
  ast->as.binop.left = left;
  ast->as.binop.op = parser->token.type;
  jscript_parser_eat(parser, parser->token.type);
  ast->as.binop.right = jscript_parser_parse_term(parser);
  return ast;
}

JSCRIPTAST* jscript_parser_parse_unop(JSCRIPTParser* parser) {
  JSCRIPTAST *ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_UNOP);
  ast->as.unop.op = parser->token.type;
  jscript_parser_eat(parser, parser->token.type);
  ast->as.unop.right = jscript_parser_parse_term(parser);
  return ast;
}

JSCRIPTAST *jscript_parser_parse_factor(JSCRIPTParser *parser) {
  if (parser->token.type == JSCRIPT_TOKEN_TYPE_LPAREN) {

    if (jscript_parser_peek_check_arrow_function(parser)) {
      return jscript_parser_parse_arrow_function(parser);
    }
    JSCRIPTAST* next = 0;
    jscript_parser_eat(parser, parser->token.type);

    if (parser->token.type != JSCRIPT_TOKEN_TYPE_RPAREN) {
      next = jscript_parser_parse_expr(parser);
    }

    jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_RPAREN);

    return next ? next : jscript_parser_error(parser);
  }
  switch (parser->token.type) {
  case JSCRIPT_TOKEN_TYPE_ID: {
    return jscript_parser_parse_id(parser, false);
  }; break;
  case JSCRIPT_TOKEN_TYPE_STRING: {
    return jscript_parser_parse_string(parser);
  }; break;
  case JSCRIPT_TOKEN_TYPE_NUMBER: {
    return jscript_parser_parse_number(parser);
  }; break;
  case JSCRIPT_TOKEN_TYPE_SPECIAL_FUNCTION: {
    return jscript_parser_parse_function(parser);
  }; break;
  case JSCRIPT_TOKEN_TYPE_SPECIAL_RETURN: {
    return jscript_parser_parse_unop(parser);
  }; break;
  case JSCRIPT_TOKEN_TYPE_EOF: {
    return jscript_parser_parse_eof(parser);
  }; break;
    default: {
      return jscript_parser_error(parser);
  }; break;
  }
  return jscript_parser_error(parser);
}

JSCRIPTAST *jscript_parser_parse_term(JSCRIPTParser *parser) {
  JSCRIPTAST *left = jscript_parser_parse_factor(parser);

  while (
    parser->token.type == JSCRIPT_TOKEN_TYPE_MUL ||
    parser->token.type == JSCRIPT_TOKEN_TYPE_DIV
  ) {
    JSCRIPTAST* binop = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_BINOP);
    binop->as.binop.left = left;
    binop->as.binop.op = parser->token.type;
    jscript_parser_eat(parser, parser->token.type);
    binop->as.binop.right = jscript_parser_parse_expr(parser);
    left = binop;
  }


  return left;
}

JSCRIPTAST *jscript_parser_parse_expr(JSCRIPTParser *parser) {
  JSCRIPTAST *left = jscript_parser_parse_term(parser);

  while (
    parser->token.type == JSCRIPT_TOKEN_TYPE_ADD ||
    parser->token.type == JSCRIPT_TOKEN_TYPE_SUB
  ) {
    JSCRIPTAST* binop = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_BINOP);
    binop->as.binop.left = left;
    binop->as.binop.op = parser->token.type;
    jscript_parser_eat(parser, parser->token.type);
    binop->as.binop.right = jscript_parser_parse_term(parser);
    left = binop;
  }

  while (parser->token.type == JSCRIPT_TOKEN_TYPE_EQUALS) {
    JSCRIPTAST* binop = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_BINOP);
    binop->as.binop.left = left;
    binop->as.binop.op = parser->token.type;
    jscript_parser_eat(parser, parser->token.type);
    binop->as.binop.right = jscript_parser_parse_expr(parser);
    left = binop;
  }

  return left;
}

JSCRIPTAST* jscript_parser_parse_call(JSCRIPTParser* parser, JSCRIPTAST* left) {
  JSCRIPTAST* ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_CALL);
  ast->as.call.left = left;

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_LPAREN);

  if (parser->token.type != JSCRIPT_TOKEN_TYPE_RPAREN) {
    JSCRIPTAST* arg = jscript_parser_parse_expr(parser);
    jscript_ast_push(ast, arg);
  }

  while (parser->token.type == JSCRIPT_TOKEN_TYPE_COMMA) {
    jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_COMMA);
    JSCRIPTAST* arg = jscript_parser_parse_expr(parser);
    jscript_ast_push(ast, arg);
  }

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_RPAREN);

  return ast;
}

JSCRIPTAST* jscript_parser_parse_arrow_function(JSCRIPTParser* parser) {
  JSCRIPTAST* ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_FUNC);

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_LPAREN);

  if (parser->token.type != JSCRIPT_TOKEN_TYPE_RPAREN) {
    JSCRIPTAST* arg = jscript_parser_parse_expr(parser);
    jscript_ast_push(ast, arg);
  }

  while (parser->token.type == JSCRIPT_TOKEN_TYPE_COMMA) {
    jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_COMMA);
    JSCRIPTAST* arg = jscript_parser_parse_expr(parser);
    jscript_ast_push(ast, arg);
  }

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_RPAREN);

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_EQUALS);
  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_GT);

  if (parser->token.type == JSCRIPT_TOKEN_TYPE_LBRACE) {
    jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_LBRACE);

    if (parser->token.type != JSCRIPT_TOKEN_TYPE_RBRACE) {
      ast->as.func.body = jscript_parser_parse_compound(parser, true);
    }

    jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_RBRACE);
  } else {
    ast->as.func.body = jscript_parser_parse_expr(parser);
  }

  return ast;
}


JSCRIPTAST* jscript_parser_parse_function(JSCRIPTParser* parser) {
  JSCRIPTAST* ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_FUNC);
  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_SPECIAL_FUNCTION);
  ast->as.func.id = jscript_parser_parse_id(parser, true);

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_LPAREN);

  if (parser->token.type != JSCRIPT_TOKEN_TYPE_RPAREN) {
    JSCRIPTAST* arg = jscript_parser_parse_expr(parser);
    jscript_ast_push(ast, arg);
  }

  while (parser->token.type == JSCRIPT_TOKEN_TYPE_COMMA) {
    jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_COMMA);
    JSCRIPTAST* arg = jscript_parser_parse_expr(parser);
    jscript_ast_push(ast, arg);
  }

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_RPAREN);

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_LBRACE);

  if (parser->token.type != JSCRIPT_TOKEN_TYPE_RBRACE) {
    ast->as.func.body = jscript_parser_parse_compound(parser, true);
  }

  jscript_parser_eat(parser, JSCRIPT_TOKEN_TYPE_RBRACE);

  return ast;
}

JSCRIPTAST* jscript_parser_parse_compound(JSCRIPTParser* parser, bool skip_brace) {

  JSCRIPTAST *ast = jscript_env_new_ast(parser->env, JSCRIPT_AST_TYPE_COMPOUND);

  while (parser->token.type != JSCRIPT_TOKEN_TYPE_EOF && parser->error == false) {

    while (parser->token.type != JSCRIPT_TOKEN_TYPE_SEMI) {
      JSCRIPTAST *child = jscript_parser_parse_expr(parser);

      if (!child)
        break;

      jscript_ast_push(ast, child);

      if (child->type == JSCRIPT_AST_TYPE_EOF)
        break;
    }

    while (parser->token.type == JSCRIPT_TOKEN_TYPE_SEMI) {
      jscript_parser_eat(parser, parser->token.type);
    }

    if (skip_brace && parser->token.type == JSCRIPT_TOKEN_TYPE_RBRACE) break;
  }

  return ast;
}

JSCRIPTAST *jscript_parser_parse(JSCRIPTParser *parser) {
  if (!parser)
    return 0;
  if (!parser->initialized)
    JSCRIPT_WARNING_RETURN(0, stderr, "parser not initialized.\n");

  return jscript_parser_parse_compound(parser, false);
}


bool jscript_parser_peek_check_arrow_function(JSCRIPTParser* parser) {
  JSCRIPTLexer* lexer = &parser->env->lexer;
  int64_t start_i = lexer->i;
  char start_c = lexer->c;

  char c = lexer->source[lexer->i];
  int64_t i = lexer->i;
  while (c != 0 && c != ';') {

    if (c == '=' && lexer->source[MIN(i, lexer->length-1)] == '>') {
      return true;
    }
    c = lexer->source[i++];
  }

  return false;
}

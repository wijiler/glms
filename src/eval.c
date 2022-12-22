#include <jscript/eval.h>
#include <jscript/macros.h>
#include <jscript/env.h>

JSCRIPTAST* jscript_fptr_print(JSCRIPTEval* eval, JSCRIPTAST* ast, JSCRIPTASTList* args, JSCRIPTStack* stack) {
  if (!args) return ast;

  for (int64_t i = 0; i < args->length; i++) {
    JSCRIPTAST* arg = jscript_eval(eval, args->items[i], stack);
    switch (arg->type) {
      case JSCRIPT_AST_TYPE_NUMBER: {printf("%1.6f\n", arg->as.number.value);}; break;
      case JSCRIPT_AST_TYPE_STRING: {
        const char* value = jscript_string_view_get_value(&arg->as.string.value);
        printf("%s\n", value);
      }; break;
      default: {
        printf("%p\n", arg);
      }; break;
    }

  }

  return ast;
}

int jscript_eval_init(JSCRIPTEval* eval, struct JSCRIPT_ENV_STRUCT* env) {
  if (!eval || !env) return 0;
  if (eval->initialized) return 1;
  eval->initialized = true;
  eval->env = env;
  jscript_env_register_function(env, "print", jscript_fptr_print);
  return 1;
}


JSCRIPTAST* jscript_eval_lookup(JSCRIPTEval* eval, const char* name, JSCRIPTStack* stack) {

  JSCRIPTAST* ast = jscript_env_lookup_function(eval->env, name);

  if (ast) return ast;

  return jscript_stack_get(stack, name);
}

JSCRIPTAST* jscript_eval(JSCRIPTEval* eval, JSCRIPTAST* ast, JSCRIPTStack* stack) {
  if (!eval || !ast) return 0;
  if (!eval->initialized) JSCRIPT_WARNING_RETURN(0, stderr, "evaluator not initialized.\n");

  switch (ast->type) {
    case JSCRIPT_AST_TYPE_COMPOUND: { return jscript_eval_compound(eval, ast, stack); }; break;
    case JSCRIPT_AST_TYPE_BINOP: { return jscript_eval_binop(eval, ast, stack); }; break;
    case JSCRIPT_AST_TYPE_UNOP: { return jscript_eval_unop(eval, ast, stack); }; break;
    case JSCRIPT_AST_TYPE_CALL: { return jscript_eval_call(eval, ast, stack); }; break;
    case JSCRIPT_AST_TYPE_FUNC: { return jscript_eval_function(eval, ast, stack); }; break;
    case JSCRIPT_AST_TYPE_ID: { return jscript_eval_id(eval, ast, stack); }; break;
    default: { return ast; }; break;
  }

  return ast;
}

JSCRIPTAST* jscript_eval_id(JSCRIPTEval* eval, JSCRIPTAST* ast, JSCRIPTStack* stack) {
  const char* name = jscript_ast_get_name(ast);

  if (!name) return ast;

  JSCRIPTAST* value = jscript_eval_lookup(eval, name, stack);

  if (!value) {
    JSCRIPT_WARNING_RETURN(ast, stderr, "`%s` is undefined.\n", name);
  }

  return value;
}



JSCRIPTAST* jscript_eval_call(JSCRIPTEval* eval, JSCRIPTAST* ast, JSCRIPTStack* stack) {
  // ast->as.call.left = jscript_eval(eval, ast->as.call.left, stack);

  const char*  fname = jscript_string_view_get_value(&ast->as.call.left->as.id.value);

  if (!fname) {
    JSCRIPT_WARNING_RETURN(ast, stderr, "function missing name.\n");
  }

  JSCRIPTAST* func = jscript_eval_lookup(eval, fname, stack);

  if (func->type != JSCRIPT_AST_TYPE_FUNC) func = 0;

  if (!func) JSCRIPT_WARNING_RETURN(ast, stderr, "No such function `%s`\n", fname);

  if (func->fptr != 0) {
    return func->fptr(eval, ast, ast->children, stack);
  } else if (func->as.func.body != 0) {
    return jscript_eval(eval, func->as.func.body, stack);
  } else {
    return ast;
  }
}

JSCRIPTAST* jscript_eval_unop(JSCRIPTEval* eval, JSCRIPTAST* ast, JSCRIPTStack* stack) {
  switch (ast->as.unop.op) {
    case JSCRIPT_TOKEN_TYPE_SPECIAL_RETURN: {
      JSCRIPTAST* retval = jscript_eval(eval, ast->as.unop.right, stack);
      jscript_stack_push(stack, "return", retval);
      stack->return_flag = true;
      return retval;
    }; break;
    default: { return ast; }; break;
  }
  return ast;
}

JSCRIPTAST* jscript_eval_binop(JSCRIPTEval* eval, JSCRIPTAST* ast, JSCRIPTStack* stack) {
  switch(ast->as.binop.op) {
    case JSCRIPT_TOKEN_TYPE_MUL:{
      ast->as.binop.left = jscript_eval(eval, ast->as.binop.left, stack);
      ast->as.binop.right = jscript_eval(eval, ast->as.binop.right, stack);
      return jscript_env_new_ast_number(eval->env, JSCRIPTAST_VALUE(ast->as.binop.left) * JSCRIPTAST_VALUE(ast->as.binop.right));
    };break;
    case JSCRIPT_TOKEN_TYPE_DIV:{
      ast->as.binop.left = jscript_eval(eval, ast->as.binop.left, stack);
      ast->as.binop.right = jscript_eval(eval, ast->as.binop.right, stack);
      return jscript_env_new_ast_number(eval->env, JSCRIPTAST_VALUE(ast->as.binop.left) / JSCRIPTAST_VALUE(ast->as.binop.right));
    };break;
    case JSCRIPT_TOKEN_TYPE_ADD:{
      ast->as.binop.left = jscript_eval(eval, ast->as.binop.left, stack);
      ast->as.binop.right = jscript_eval(eval, ast->as.binop.right, stack);
      return jscript_env_new_ast_number(eval->env, JSCRIPTAST_VALUE(ast->as.binop.left) + JSCRIPTAST_VALUE(ast->as.binop.right));
    };break;
    case JSCRIPT_TOKEN_TYPE_SUB:{
      ast->as.binop.left = jscript_eval(eval, ast->as.binop.left, stack);
      ast->as.binop.right = jscript_eval(eval, ast->as.binop.right, stack);
      return jscript_env_new_ast_number(eval->env, JSCRIPTAST_VALUE(ast->as.binop.left) - JSCRIPTAST_VALUE(ast->as.binop.right));
    };break;
    case JSCRIPT_TOKEN_TYPE_PERCENT:{
      ast->as.binop.left = jscript_eval(eval, ast->as.binop.left, stack);
      ast->as.binop.right = jscript_eval(eval, ast->as.binop.right, stack);
      return jscript_env_new_ast_number(eval->env, (int)JSCRIPTAST_VALUE(ast->as.binop.left) % (int)JSCRIPTAST_VALUE(ast->as.binop.right));
    };break;
    case JSCRIPT_TOKEN_TYPE_EQUALS: {
      if (ast->as.binop.right->type == JSCRIPT_AST_TYPE_FUNC && !jscript_ast_get_name(ast->as.binop.right)) {
        if (ast->as.binop.left->type == JSCRIPT_AST_TYPE_ID) {
          ast->as.binop.right->as.func.id = ast->as.binop.left;
        }
      }
      ast->as.binop.right = jscript_eval(eval, ast->as.binop.right, stack);
      jscript_stack_push(stack, jscript_ast_get_name(ast), ast->as.binop.right);

      if (stack->return_flag) {
        printf("yes\n");
        JSCRIPTAST* retval = jscript_eval_lookup(eval, "return", stack);

        if (retval) return retval;
      }
    }; break;
    default: { return ast; }; break;
  }

  return ast;
}

JSCRIPTAST* jscript_eval_function(JSCRIPTEval* eval, JSCRIPTAST* ast, JSCRIPTStack* stack) {
  // ast->as.func.id = jscript_eval(eval, ast->as.func.id, stack);

  const char* fname = jscript_ast_get_name(ast);

  if (!fname) JSCRIPT_WARNING_RETURN(ast, stderr, "Expected a name to exist.\n");

  jscript_stack_push(stack, fname, ast);

  return ast;
}

JSCRIPTAST* jscript_eval_compound(JSCRIPTEval* eval, JSCRIPTAST* ast, JSCRIPTStack* stack) {
  if (!ast->children || ast->children->length <= 0) return ast;


  JSCRIPTStack local_stack = {0};
  jscript_stack_init(&local_stack);
  jscript_stack_copy(*stack, &local_stack);

  for (int64_t i = 0; i < ast->children->length; i++) {
    JSCRIPTAST* child = ast->children->items[i];

    child = jscript_eval(eval, child, &local_stack);

    if (local_stack.return_flag) {
      JSCRIPTAST* retval = jscript_eval_lookup(eval, "return", &local_stack);
      if (retval != 0) {
        local_stack.return_flag = false;
        jscript_stack_clear(&local_stack);
        return retval;
      }
    }

    if (ast->children->length == 1) {
      jscript_stack_clear(&local_stack);
      return child;
    }
  }

  jscript_stack_clear(&local_stack);



  return ast;
}

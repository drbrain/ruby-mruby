#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <ruby.h>

#include "extconf.h"

VALUE
mruby_alloc(VALUE klass) {
    VALUE obj;
    mrb_state *mrb;

    mrb = mrb_open();

    obj = Data_Wrap_Struct(klass, NULL, mrb_close, mrb);

    return obj;
}

VALUE mruby_eval(VALUE mruby, VALUE source) {
    mrb_state *mrb;
    mrb_sym zero_sym;
    mrb_value v, inspect_v;
    mrbc_context *c;

    char *src;

    VALUE result;

    src = StringValuePtr(source);

    Data_Get_Struct(mruby, mrb_state, mrb);

    c = mrbc_context_new(mrb);

    zero_sym = mrb_intern_lit(mrb, "$0");
    mrbc_filename(mrb, c, "<ruby>");
    mrb_gv_set(mrb, zero_sym, mrb_str_new_lit(mrb, "<ruby>"));

    v = mrb_load_string_cxt(mrb, src, c);

    mrbc_context_free(mrb, c);

    inspect_v = mrb_funcall(mrb, v, "inspect", 0);

    result = rb_str_new(MRSTRING_PTR(inspect_v), MRSTRING_LEN(inspect_v));

    return result;
}

void
Init_mruby(void) {
    VALUE cMRuby = rb_define_class("MRuby", rb_cObject);

    rb_define_alloc_func(cMRuby, mruby_alloc);

    rb_define_method(cMRuby, "eval", mruby_eval, 1);
}


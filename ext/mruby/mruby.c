#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/variable.h>

#include <ruby.h>

#include "extconf.h"

static VALUE sym_verbose;

VALUE
mruby_alloc(VALUE klass) {
    VALUE obj;
    mrb_state *mrb;

    mrb = mrb_open();

    obj = Data_Wrap_Struct(klass, NULL, mrb_close, mrb);

    return obj;
}

static void
mruby_context_options(VALUE options, mrbc_context *c) {
    if (RTEST(rb_hash_fetch(options, sym_verbose)))
	c->dump_result = TRUE;
}

VALUE
mruby_eval(int argc, VALUE *argv, VALUE mruby) {
    mrb_state *mrb;
    mrb_sym zero_sym;
    mrb_value v, inspect_v;
    mrbc_context *c;

    VALUE source, source_bytesize, options, result;

    rb_scan_args(argc, argv, "1:", &source, &options);

    Data_Get_Struct(mruby, mrb_state, mrb);

    c = mrbc_context_new(mrb);

    mruby_context_options(options, c);

    zero_sym = mrb_intern_lit(mrb, "$0");
    mrbc_filename(mrb, c, "<ruby>");
    mrb_gv_set(mrb, zero_sym, mrb_str_new_lit(mrb, "<ruby>"));

    source_bytesize = LONG2NUM(RSTRING_LEN(source));

    v = mrb_load_nstring_cxt(mrb,
	    StringValuePtr(source), NUM2INT(source_bytesize), c);

    mrbc_context_free(mrb, c);

    inspect_v = mrb_funcall(mrb, v, "inspect", 0);

    result = rb_str_new(MRSTRING_PTR(inspect_v), MRSTRING_LEN(inspect_v));

    return result;
}

void
Init_mruby(void) {
    VALUE cMRuby = rb_define_class("MRuby", rb_cObject);

    rb_define_alloc_func(cMRuby, mruby_alloc);

    rb_define_method(cMRuby, "eval", mruby_eval, -1);

    sym_verbose = ID2SYM(rb_intern("verbose"));
}


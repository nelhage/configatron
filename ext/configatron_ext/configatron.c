#include "ruby.h"
#include <stdio.h>

struct {
    struct {
        VALUE mod;
        struct {
            VALUE mod;
        } Store;

        struct {
            VALUE mod;
        } Proc;
    } Configatron;

    struct {
        ID Configatron;
        ID Store;
        ID Proc;

        ID method_missing;
        ID call;
        ID do_lookup;

        ID aget;
    } sym;
}c;

static VALUE
store_method_missing(int argc, VALUE *argv, VALUE self) {
    VALUE id, block;
    rb_scan_args(argc, argv, "1*&", &id, NULL, &block);
    if(TYPE(id) != T_SYMBOL) {
        return Qnil;
    }
    if(block != Qnil) {
        VALUE val = rb_funcall(self, c.sym.aget, 1, id);
        return rb_funcall(block, c.sym.call, 1, val);
    }
    const char *meth = rb_id2name(SYM2ID(id));
    int len = strlen(meth);
    if(len >=1 && (meth[len-1] == '=' || meth[len-1] == '!')) {
        return rb_funcallv(self, c.sym.do_lookup, argc, argv);
    }
    return rb_funcall(self, c.sym.aget, 1, id);
}

void
Init_configatron_ext() {
#define S(name) c.sym.name = rb_intern(#name)
    S(Configatron);
    S(Store);
    S(Proc);
    S(method_missing);
    S(call);
    S(do_lookup);
#undef S
    c.sym.aget = rb_intern("[]");

    c.Configatron.mod = rb_const_get(rb_cObject, c.sym.Configatron);
    c.Configatron.Store.mod = rb_const_get(c.Configatron.mod, c.sym.Store);
    c.Configatron.Proc.mod = rb_const_get(c.Configatron.mod, c.sym.Proc);

    rb_define_method(c.Configatron.Store.mod,
                     "method_missing",
                     store_method_missing, -1);
}

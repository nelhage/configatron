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
        ID new;
        ID to_sym;
        ID __rb_aget;

        ID at__attributes;

        ID aget;
    } sym;
} c;

static VALUE
store_aget(VALUE self, VALUE key) {
    if(TYPE(key) != T_SYMBOL) {
        key = rb_funcall(key, c.sym.to_sym, 0);
        if(TYPE(key) != T_SYMBOL) {
            return Qnil;
        }
    }

    VALUE attributes = rb_attr_get(self, c.sym.at__attributes);
    if (TYPE(attributes) != T_HASH)
        return Qnil;
    VALUE val = rb_hash_lookup2(attributes, key, Qundef);
    if (val != Qundef) {
        if (rb_obj_is_kind_of(val, c.Configatron.Proc.mod)) {
            val = rb_funcall(val, c.sym.call, 0);
        }
        return val;
    }

    return rb_funcall(self, c.sym.__rb_aget, 1, key);
}

static VALUE
store_method_missing(int argc, VALUE *argv, VALUE self) {
    VALUE id, block;
    rb_scan_args(argc, argv, "1*&", &id, NULL, &block);
    if(TYPE(id) != T_SYMBOL) {
        return Qnil;
    }
    if(block != Qnil) {
        VALUE val = store_aget(self, id);
        return rb_funcall(block, c.sym.call, 1, val);
    }
    const char *meth = rb_id2name(SYM2ID(id));
    int len = strlen(meth);
    if(len >=1 && (meth[len-1] == '=' || meth[len-1] == '!')) {
        return rb_funcallv(self, c.sym.do_lookup, argc, argv);
    }
    return store_aget(self, id);
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
    S(to_sym);
    S(__rb_aget);
#undef S

    c.sym.at__attributes = rb_intern("@attributes");
    c.sym.aget = rb_intern("[]");

    c.Configatron.mod = rb_const_get(rb_cObject, c.sym.Configatron);
    c.Configatron.Store.mod = rb_const_get(c.Configatron.mod, c.sym.Store);
    c.Configatron.Proc.mod = rb_const_get(c.Configatron.mod, c.sym.Proc);

    rb_define_method(c.Configatron.Store.mod,
                     "method_missing",
                     store_method_missing, -1);
    rb_define_alias(c.Configatron.Store.mod, "__rb_aget", "[]");
    rb_define_method(c.Configatron.Store.mod,
                     "[]",
                     store_aget, 1);
}

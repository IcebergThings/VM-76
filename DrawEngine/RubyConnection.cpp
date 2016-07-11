#include "engine.hpp"

VALUE wrap_init_engine(VALUE self, VALUE w, VALUE h) {
  int a, b, r;

  a = FIX2INT(w);
  b = FIX2INT(h);
  r = init_engine(a,b);
  return INT2FIX(r);
}

VALUE warp_main_draw_loop() {
  main_draw_loop();
  return Qnil;
}

extern "C" {

  void Init_VMDE() {
    printf("%s[Init_VMDrawEngine]: MODULE INIT\n", DBG_HEAD);

    Global_module = rb_define_module("VMDE");
    rb_define_module_function(Global_module, "init_engine", (RB_F_R) wrap_init_engine, 2);
    rb_define_module_function(Global_module, "draw_loop", (RB_F_R) warp_main_draw_loop, 0);
  }

}

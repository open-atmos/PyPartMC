#pragma once
#include <map>

extern "C" void f_ctor(void *ptr, const int &n);
extern "C" void f_dtor(void *ptr);
extern "C" void f_set_item(void *ptr, void *map);

struct gas_state_t {
  void *ptr;

  gas_state_t(const int &n) {
    f_ctor(&this->ptr, n);
  }

  ~gas_state_t() {
    f_dtor(&this->ptr);
  }

  template<typename idx_t, typename arr_t>
  static void set_item(const gas_state_t &self, const idx_t &idx, const arr_t& arr) {
    std::unordered_map<const char*, const arr_t> tmp{{"gas_mixing_ratio", arr}};   
    //f_set_item(self.ptr, &tmp);
  }

  template<typename idx_t, typename arr_t>
  static auto get_item(const gas_state_t &self, const idx_t &idx) {
    //f_get_item(self.ptr, &arr);
    return arr_t();
  }
};

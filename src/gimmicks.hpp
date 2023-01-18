/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Authors: https://github.com/open-atmos/PyPartMC/graphs/contributors                              #
##################################################################################################*/

#pragma once

#include <iostream>
#include <sstream>
#include <set>
#include <stack>
#include "nlohmann/json.hpp"
#include <tcb/span.hpp>
#include <bpstd/string_view.hpp>

struct Gimmick {
  private:
    std::set<std::string> vars;
    const nlohmann::json *json;
    std::stack<const nlohmann::json*> json_parent;

    void warn(const std::exception &exception) {
        std::cerr << "WARN: " << exception.what() << std::endl;
//        assert(false);
    }

  protected:
    Gimmick() {}

    Gimmick(const nlohmann::json &json) {
        this->set_current_json_ptr(&json);
        for (auto &entry : this->json->items()) {
            this->vars.insert(entry.key());
        }
    };

    void set_current_json_ptr(const nlohmann::json *ptr) {
        this->json = ptr;
    }

    auto dump() const {
        return this->json->dump();
    }

    std::string last_dict_key() const noexcept {
        std::string key = "<NOT FOUND>";
        for (const auto& item : this->json->items())
        {
            if (this->json->is_array()) {
                for (auto &entry : item.value().items()) {
                    key = entry.key();
                }
            } else {
                key = item.key();
            }
        }
        return key;
    }
 
  public:
    virtual ~Gimmick() {}

    void zoom_in(const bpstd::string_view &sub) noexcept {
        auto it = this->json->is_array() 
            ? this->json->at(this->json->size()-1).begin()
            : this->json->find(sub);
        // TODO #112: handle errors
        this->json_parent.push(this->json);
        this->set_current_json_ptr(&(*it));
    }

    void zoom_out() noexcept {
        assert(this->json_parent.size() != 0);
        this->set_current_json_ptr(this->json_parent.top());
        this->json_parent.pop();
    }

    auto zoom_level() noexcept {
        auto level = this->json_parent.size();
        return level;
    }

    auto begin() noexcept {
        return this->json->begin();
    }

    // TODO #112: to be removed after initialising GasData with a list, and not JSON?
    std::string first_field_name() const noexcept {
        // TODO #112: handle errors
        assert(this->json->size() > 0);
        assert(this->json->begin()->size() > 0);
        for (auto &entry : this->json->at(0).items())
        {
            return entry.key();
        }
        assert(false);
        return "";
    }

    auto is_empty() noexcept {
        return this->json->empty();
    }

    auto size() noexcept {
        return this->json->size();
    }

    std::size_t n_elements(const bpstd::string_view &name) noexcept {
        for (auto i=0u; i<this->json->size(); ++i) {
            for (auto &entry : this->json->at(i).items()) {
                if (entry.key() == name)
                    return entry.value().size();
            }
        }
        assert(false);
        return 0;
    }

    auto n_numeric_array_entries() noexcept {
        auto count = 0u;
        for (auto i=0u; i<this->json->size(); ++i) {
            assert(this->json->at(i).is_object());
            assert(this->json->at(i).size() == 1);
            for (auto &entry : this->json->at(i).items()) 
                if (entry.value().is_array() && (entry.value().size() == 0 || entry.value().at(0).is_number()))
                    ++count;
        }
        return count;
    }

    template <typename T>
    void read_value(
        const bpstd::string_view name,
        T *var
    ) {
        *var = this->find(name)->get<T>();
    }

    void read_str(
        const bpstd::string_view &name,
        char* var_data,
        int* var_size
    ) noexcept {
        auto it = this->find(name);
        if (it == this->json->end())
        {
            assert(false);
            // TODO #112
            return;
        }
        auto value = it->is_array()
            ? name
            : it->begin()->get<bpstd::string_view>();  // TODO #112: is this path used anywhere?
        if ((int)value.size() > *var_size) {
            std::ostringstream oss;
            oss << "provided entry \"" << name << "\" has too many characters";
            this->warn(std::invalid_argument(oss.str()));
        }
        for (auto i = 0u; i < value.size(); ++i)
            var_data[i] = value[i];
        var_size[0] = value.size();
    }

    template <typename T1, typename T2>
    void read_arr(
        const T1 &name,
        const tcb::span<T2> &values
    ) noexcept {
        for (auto i=0u; i<this->json->size(); ++i) {
            for (auto &entry : this->json->at(i).items()) {
                if (entry.key() == name) {
                    for (auto j = 0u; j < values.size(); ++j)
                        values[j] = entry.value().at(j);
                    return;
                }
            }
        }
        // TODO #112: check size
    }

    auto varid(const std::string& name) noexcept {
        auto it = this->vars.find(name);
        if (it == this->vars.end()) {
            this->warn(std::logic_error("var not found!"));
        }
        return std::distance(this->vars.begin(), it);
    }

    template <typename T>
    auto find(const T& entry) noexcept -> decltype(this->json->end()) {
        auto it = this->json->find(entry);
        if (it == this->json->end()) {
            std::ostringstream oss;
            oss << "provided data is missing the \"" << entry << "\" entry";
            this->warn(std::invalid_argument(oss.str()));
        }
        return it;
    }

    virtual std::string str() const = 0;

    virtual bool read_line(std::string &name, std::string &data) = 0;
};

struct InputGimmick: Gimmick {
  private:
    std::string key_cond, key_name;
    std::string last_read_line_key = "";
    std::size_t max_zoom_level;

  public:
    InputGimmick(
        const nlohmann::json &json,
        const std::string key_cond = "",
        const std::string key_name = "",
        const std::size_t max_zoom_level = 3
    ) : Gimmick(json), key_cond(key_cond), key_name(key_name), max_zoom_level(max_zoom_level)
    {}

    std::string str() const {
        throw std::logic_error("str() called on InputGimmick!");
    }

    bool read_line(std::string &name, std::string &data) {
        bool eof = this->is_empty();

        if (this->zoom_level() == this->max_zoom_level) { // TODO #112
            eof = true;
            this->zoom_out();
        }

        if (!eof) {
            assert(this->size() == 1);
            auto key = this->last_dict_key();
            if (this->key_name != "" && (this->key_cond == this->last_read_line_key)) {
                name = this->key_name;
                this->zoom_in(key);
            } else {
                name = key;
            }
            data = key;
            this->last_read_line_key = key;
        }
        return eof;
    }
};

struct OutputGimmick: Gimmick {
    std::unique_ptr<nlohmann::json> guard;

    OutputGimmick() : guard(std::make_unique<nlohmann::json>()) {
        this->set_current_json_ptr(this->guard.get());
    }

    std::string str() const {
        return this->dump();
    }

    bool read_line(std::string &, std::string &) {
        throw std::logic_error("read_line() called on OutputGimmick!");
    }
};

std::unique_ptr<Gimmick> &gimmick_ptr();


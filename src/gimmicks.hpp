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

  public:
    virtual ~Gimmick() {}

    void zoom_in(const bpstd::string_view &sub) noexcept {
        auto it = this->json->is_array() 
            ? this->json->at(this->json->size()-1).begin()
            : this->json->find(sub);
        // TODO: handle errors
        this->json_parent.push(this->json);
        this->set_current_json_ptr(&(*it));
    }

    void zoom_out() noexcept {
        assert(this->json_parent.size() != 0);
        this->set_current_json_ptr(this->json_parent.top());
        this->json_parent.pop();
    }

    auto begin() noexcept {
        return this->json->begin();
    }

    // TODO: to be removed after initialising GasData with a list, and not JSON?
    std::string first_field_name() noexcept {
        // TODO: handle errors
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
            // TODO
            return;
        }
        auto value = it->is_array()
            ? name
            : it->begin()->get<bpstd::string_view>();  // TODO: is this path used anywhere?
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
        // TODO: check size
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
};

struct InputGimmick: Gimmick {
    InputGimmick(const nlohmann::json &json) : Gimmick(json) {}
    std::string str() const {
        throw std::logic_error("str() called on InputGimmick!");
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
};

std::unique_ptr<Gimmick> &gimmick_ptr();


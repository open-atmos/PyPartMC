/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include <iostream>
#include <sstream>
#include <span>
#include <set>
#include "nlohmann/json.hpp"

struct Gimmick {
  private:
    std::set<std::string> vars;
    const nlohmann::json *json, *json_parent = NULL;

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

    void zoom_in(const std::string_view &sub) noexcept {
        assert(this->json_parent == NULL);
        auto it = this->find(sub);
        this->json_parent = this->json;
        this->set_current_json_ptr(&(*it));
    }

    void zoom_out() noexcept {
        assert(this->json_parent != NULL);
        this->set_current_json_ptr(this->json_parent);
        this->json_parent = NULL;
    }

    auto is_empty() noexcept {
        return this->json->empty();
    }

    auto n_elements(const std::string_view &name) noexcept {
        return this->json->find(name)->size();
    }

    void read_str(
        const std::string_view &name,
        char* var_data,
        int* var_size
    ) noexcept {
        auto it = this->find(name);
        if (it == this->json->end())
        {
            // TODO
            return;
        }
        auto value = it->is_object()
            ? name
            : it->get<std::string_view>();
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
        const std::span<T2> &values
    ) noexcept {
        auto it = this->find(name);
        if (it == this->json->end())
        {
            // TODO
            return;
        }
        // TODO: check size
        for (auto i = 0u; i < values.size(); ++i)
            values[i] = (*it)[i];
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


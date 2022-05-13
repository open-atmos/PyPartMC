/*##################################################################################################
# This file is a part of PyPartMC licensed under the GNU General Public License v3 (LICENSE file)  #
# Copyright (C) 2022 University of Illinois Urbana-Champaign                                       #
# Author: Sylwester Arabas                                                                         #
##################################################################################################*/

#pragma once

#include <sstream>
#include <set>
#include "nlohmann/json.hpp"

struct Gimmick {
  private:
    std::set<std::string> vars;
    nlohmann::json json;

  protected:
    Gimmick(const nlohmann::json &json) : json(json) {
        for (auto &entry : this->json.items()) {
            this->vars.insert(entry.key());
        }
    };

    auto dump() const {
        return this->json.dump();
    }

  public:
    auto is_empty() {
        return this->json.empty();
    }

    auto varid(const std::string& name) const {
        auto it = this->vars.find(name);
        if (it == this->vars.end()) {
            throw std::logic_error("var not found!");
        }
        return std::distance(this->vars.begin(), it);
    }

    auto find(const std::string& entry) const {
        auto it = this->json.find(entry);
        if (it == this->json.end()) {
            std::ostringstream oss;
            oss << "provided data is missing the \"" << entry << "\" entry";
            throw std::invalid_argument(oss.str());
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
    OutputGimmick() : Gimmick({}) {}

    std::string str() const {
        return this->dump();
    }
};

std::unique_ptr<Gimmick> &gimmick_ptr();


#include <set>
#include <map>
#include <string>
#include <sstream>
#include "nlohmann/json.hpp"

struct InputGuard {
  public:
    InputGuard(const nlohmann::ordered_json &j) {
        process_json(j);

        this->dict_key_present = false;
    }

    ~InputGuard() {}

    void check_used_inputs() {
        for (auto item : this->used_inputs) {
            if (!item.second) {
                std::string err = std::string("WARNING: \"") + item.first + std::string("\" parameter remains unused.");
                throw std::runtime_error(err);
            }
        }
    }

    void mark_used_input(const std::string &input_name) {
        if (this->prefixes.find(input_name) == this->prefixes.end()) {
            std::string prefix = combine_str_vec(this->curr_prefix);

            if (!prefix.empty()) {
                this->used_inputs[prefix + "/" + input_name] = true;
            }
            else {
                this->used_inputs[input_name] = true;
            }
        }
    }

    void update_dict_key(std::string dict_key) {
        this->curr_dict_key = dict_key;
    }

    void check_read_line(std::string line) {
        if (line == "mode_name") {
            if (this->dict_key_present) {
                this->curr_prefix.pop_back();
            }
            this->curr_prefix.push_back(this->curr_dict_key);
            this->dict_key_present = true;
        }
        else if (line.empty()) {
            this->curr_prefix.pop_back();
            this->dict_key_present = false;
        }
    }

    void open_spec_file(std::string spec_file_name) {
        this->curr_prefix.push_back(spec_file_name);
    }

    void close_spec_file() {
        this->curr_prefix.pop_back();
    }
  
  private:
    std::map<std::string, bool> used_inputs;

    std::set<std::string> prefixes;
    std::string curr_dict_key;
    std::vector<std::string> curr_prefix;

    bool dict_key_present;

    void process_json(const nlohmann::ordered_json &j) {
        nlohmann::ordered_json flat = j.flatten();

        // JSON Pointer, as in a string syntax for identifying a specific value in JSON
        std::vector<std::string> json_pointers;

        for (auto f : flat.items()) {
            json_pointers.push_back(clean_string(f.key()));
        }

        std::set<std::string> json_pointers_set(json_pointers.begin(), json_pointers.end());

        for (auto s : json_pointers_set) {
            this->used_inputs[s] = false;
        }

        get_prefixes(json_pointers_set);
    }

    std::string clean_string(std::string str) {
        bool after_slash = false;

        for (size_t i = 0; i < str.size(); i++) {
            if (str.at(i) == '/' && i+1 < str.size()) {
                if (isdigit(str.at(i+1))) {
                    after_slash = true;
                    str.erase(i, 1);
                    i -= 1;
                }
            }
            else if (isdigit(str.at(i)) && after_slash) {
                str.erase(i, 1);
                i -= 1;
            }
            else {
                after_slash = false;
            }
        }

        str.erase(0, 1);

        return str;
    }

    std::string combine_str_vec(std::vector<std::string> vec) {
        if (vec.size() == 0) return "";
        
        std::string temp = vec[0];

        for (size_t i = 1; i < vec.size(); i++) {
            temp += "/";
            temp += vec[i];
        }

        return temp;
    }

    void get_prefixes(std::set<std::string> json_pointers_set) {
        std::string temp;
        std::vector<std::string> temp_vec;

        for (auto s : json_pointers_set) {
            std::stringstream line(s);

            while(getline(line, temp, '/')) {
                temp_vec.push_back(temp);
            }

            if (temp_vec.size() > 1) {
                temp_vec.pop_back();

                for (auto v : temp_vec) {
                    this->prefixes.insert(v);
                }
            }

            temp_vec.clear();
        }
    }
};
#include <algorithm>
#include <concepts>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

template <typename T>
concept Iterable = requires(T tt) {
    { tt.begin() } -> std::same_as<typename T::iterator>;
    { tt.end() } -> std::same_as<typename T::iterator>;
} && !requires(T tt) {
    (*(tt.begin())).begin();
} && !std::same_as<T, std::string>;

template <typename T>
concept IterableMD = requires(T tt) {
    tt.begin()->begin();
    tt.end()->end();
    { tt.begin() } -> std::same_as<typename T::iterator>;
    { tt.end() } -> std::same_as<typename T::iterator>;
};

template <Iterable T>
std::ostream& operator<<(std::ostream& os, const T& cont) {
    for (auto& vv : cont) {
        os << vv << ' ';
    }
    return os;
}

template <IterableMD T>
std::ostream& operator<<(std::ostream& os, const T& cont) {
    for (auto& vv : cont) {
        os << vv << '\n';
    }
    return os;
}

class State {
public:
    virtual std::vector<size_t> GetFunctionResult(const std::string& str) = 0;
    virtual std::vector<size_t> GetFunctionResult(
        const std::vector<size_t>& str) = 0;
    virtual std::string GetStrByFunction(
        const std::vector<size_t>& strfunc) = 0;
    virtual std::vector<size_t> GetVecByFunction(
        const std::vector<size_t>& strfunc) = 0;
    virtual ~State() = default;
};

class PrefixFunction : public State {
public:
    std::vector<size_t> GetFunctionResult(const std::string& str) override {
        size_t len = str.length();
        std::vector<size_t> pref(len, 0);
        for (size_t ind = 1; ind < len; ++ind) {
            size_t prefval = pref[ind - 1];
            for (; prefval > 0 && str[ind] != str[prefval];
                 prefval = pref[prefval - 1]) {
            }
            if (str[ind] == str[prefval]) {
                ++prefval;
            }
            pref[ind] = prefval;
        }
        return pref;
    }

    std::vector<size_t> GetFunctionResult(
        const std::vector<size_t>& str) override {
        size_t len = str.size();
        std::vector<size_t> pref(len, 0);
        for (size_t ind = 1; ind < len; ++ind) {
            size_t prefval = pref[ind - 1];
            for (; prefval > 0 && str[ind] != str[prefval];
                 prefval = pref[prefval - 1]) {
            }
            if (str[ind] == str[prefval]) {
                ++prefval;
            }
            pref[ind] = prefval;
        }
        return pref;
    }

    std::string GetStrByFunction(const std::vector<size_t>& strfunc) override {
        if (strfunc.empty()) {
            return "";
        }
        if (strfunc.size() == 1) {
            return std::string(1, 'a');
        }
        return "";
    }

    std::vector<size_t> GetVecByFunction(
        const std::vector<size_t>& strfunc) override {
        size_t len = strfunc.size();
        std::vector<size_t> str(len);
        size_t co = size_t('a');
        if (strfunc.empty()) {
            return str;
        }
        if (strfunc.size() == 1) {
            str[0] = co;
            return str;
        }
        for (size_t ind = 0; ind < len; ++ind) {
            if (strfunc[ind] == 0) {
                str[ind] = ++co;
            } else {
                str[ind] = str[strfunc[ind] - 1];
            }
        }
        return str;
    }
};



class ZFunction : public State {
public:
    std::vector<size_t> GetFunctionResult(const std::string& str) override {
        size_t len = str.size();
        std::vector<size_t> z_func(len, 0);
        size_t left = 0;
        size_t right = 0;
        for (size_t ind = 1; ind < len; ++ind) {
            if (ind <= right) {
                z_func[ind] = std::min(right - ind + 1, z_func[ind - left]);
            }
            for (; ind + z_func[ind] < len &&
                   str[z_func[ind]] == str[ind + z_func[ind]];
                 ++z_func[ind]) {
            }
            if (ind + z_func[ind] - 1 > right) {
                left = ind;
                right = ind + z_func[ind] - 1;
            }
        }
        return z_func;
    }

    std::vector<size_t> GetFunctionResult(
        const std::vector<size_t>& str) override {
        size_t len = str.size();
        std::vector<size_t> z_func(len, 0);
        size_t left = 0;
        size_t right = 0;
        for (size_t ind = 1; ind < len; ++ind) {
            if (ind <= right) {
                z_func[ind] = std::min(right - ind + 1, z_func[ind - left]);
            }
            for (; ind + z_func[ind] < len &&
                   str[z_func[ind]] == str[ind + z_func[ind]];
                 ++z_func[ind]) {
            }
            if (ind + z_func[ind] - 1 > right) {
                left = ind;
                right = ind + z_func[ind] - 1;
            }
        }
        return z_func;
    }

    std::string GetStrByFunction(const std::vector<size_t>& strfunc) override {
        if (strfunc.empty()) {
            return "";
        }

        size_t co = size_t('a');

        if (strfunc.size() == 1) {
            return std::string(1, char(co));
        }

        size_t len = strfunc.size();
        std::vector<std::unordered_set<char>> lengs(strfunc.size());
        std::string res(len, char(co));
        size_t right = 0;
        for (size_t ind = 1; ind < len; ++ind) {
            size_t win = strfunc[ind];

            if (win == 0) {
                size_t start = size_t('b');
                while (lengs[ind - 1].find(char(start)) !=
                       lengs[ind - 1].end()) {
                    ++start;
                }
                res[ind] = char(start);
                continue;
            }

            right = std::max(right, ind + win - 1);
            for (size_t old = right + 1 - ind; old < win; ++old) {
                res[ind + old] = res[old];
            }

            lengs[ind - 1 + win].insert(res[win]);
        }
        return res;
    }

    std::vector<size_t> GetVecByFunction(
        const std::vector<size_t>& strfunc) override {
        size_t co = size_t('a');
        size_t len = strfunc.size();
        std::vector<size_t> res(len, char(co));

        if (strfunc.size() <= 1) {
            return res;
        }

        std::vector<std::unordered_set<size_t>> lengs(strfunc.size());
        size_t right = 0;
        for (size_t ind = 1; ind < len; ++ind) {
            size_t win = strfunc[ind];

            if (win == 0) {
                size_t start = size_t('b');
                while (lengs[ind - 1].find(start) != lengs[ind - 1].end()) {
                    ++start;
                }
                res[ind] = start;
                continue;
            }

            right = std::max(right, ind + win - 1);
            for (size_t old = right + 1 - ind; old < win; ++old) {
                res[ind + old] = res[old];
            }

            lengs[ind - 1 + win].insert(res[win]);
        }
        return res;
    }
};


class ManakerOdd : public State {
public:
    std::vector<size_t> GetFunctionResult(const std::string& str) override {
        size_t len = str.length();
        std::vector<size_t> pal(len);
        size_t left = 0;
        size_t right = -1;
        for (size_t ind = 0; ind < len; ++ind) {
            size_t palind;
            if (ind > right) {
                palind = 1;
            } else {
                palind = std::min(pal[left + right - ind], right - ind + 1);
            }
            for (; ind + palind < len && ind >= palind &&
                   str[ind + palind] == str[ind - palind];
                 ++palind) {
            }
            pal[ind] = palind;
            if (ind + palind - 1 > right) {
                left = ind - palind + 1;
                right = ind + palind - 1;
            }
        }
        return pal;
    }

    std::vector<size_t> GetFunctionResult(
        const std::vector<size_t>& str) override {
        size_t len = str.size();
        std::vector<size_t> pal(len);
        size_t left = 0;
        size_t right = -1;
        for (size_t ind = 0; ind < len; ++ind) {
            size_t palind;
            if (ind > right) {
                palind = 1;
            } else {
                palind = std::min(pal[left + right - ind], right - ind + 1);
            }
            for (; ind + palind < len && ind >= palind &&
                   str[ind + palind] == str[ind - palind];
                 ++palind) {
            }
            pal[ind] = palind;
            if (ind + palind - 1 > right) {
                left = ind - palind + 1;
                right = ind + palind - 1;
            }
        }
        return pal;
    }
    std::string GetStrByFunction(const std::vector<size_t>& strfunc) override {
        if (strfunc.empty()) {
            return "";
        }
        if (strfunc.size() == 1) {
            return std::string(1, char(1));
        }
        return "";
    }
    std::vector<size_t> GetVecByFunction(
        const std::vector<size_t>& strfunc) override {
        if (strfunc.empty()) {
            return std::vector<size_t>();
        }
        if (strfunc.size() == 1) {
            return std::vector<size_t>(1, size_t('a'));
        }
        return std::vector<size_t>();
    }
};

class ManakerEven : public State {
public:
    std::vector<size_t> GetFunctionResult(const std::string& str) override {
        size_t len = str.length();
        std::vector<size_t> pal(len);
        size_t left = 0;
        size_t right = -1;
        for (size_t ind = 0; ind < len; ++ind) {
            size_t palind;
            if (ind > right) {
                palind = 0;
            } else {
                palind = std::min(pal[left + right - ind + 1], right - ind + 1);
            }
            for (; ind + palind < len && ind - palind >= 1 &&
                   str[ind + palind] == str[ind - palind - 1];
                 ++palind) {
            }
            pal[ind] = palind;
            if (ind + palind - 1 > right) {
                left = ind - palind;
                right = ind + palind - 1;
            }
        }
        return pal;
    }

    std::vector<size_t> GetFunctionResult(
        const std::vector<size_t>& str) override {
        size_t len = str.size();
        std::vector<size_t> pal(len);
        size_t left = 0;
        size_t right = -1;
        for (size_t ind = 0; ind < len; ++ind) {
            size_t palind;
            if (ind > right) {
                palind = 0;
            } else {
                palind = std::min(pal[left + right - ind + 1], right - ind + 1);
            }
            for (; ind + palind < len && ind - palind >= 1 &&
                   str[ind + palind] == str[ind - palind - 1];
                 ++palind) {
            }
            pal[ind] = palind;
            if (ind + palind - 1 > right) {
                left = ind - palind;
                right = ind + palind - 1;
            }
        }
        return pal;
    }


    std::string GetStrByFunction(const std::vector<size_t>& strfunc) override {
        if (strfunc.empty()) {
            return "";
        }
        if (strfunc.size() == 1) {
            return std::string(1, char(1));
        }
        return "";
    }

    std::vector<size_t> GetVecByFunction(
        const std::vector<size_t>& strfunc) override {
        if (strfunc.empty()) {
            return std::vector<size_t>();
        }
        if (strfunc.size() == 1) {
            return std::vector<size_t>(1, size_t('a'));
        }
        return std::vector<size_t>();
    }
};

class StringFunction {
public:
    StringFunction() : currentState_(std::make_shared<PrefixFunction>()) {}

    void SetState(const std::shared_ptr<State>& new_state) {
        currentState_ = new_state;
    }

    std::vector<size_t> GetFunctionResult(const std::string& str) {
        return currentState_->GetFunctionResult(str);
    }

    std::vector<size_t> GetFunctionResult(const std::vector<size_t>& str) {
        return currentState_->GetFunctionResult(str);
    }

    std::string GetStrByFunction(const std::vector<size_t>& strfunc) {
        return currentState_->GetStrByFunction(strfunc);
    }

    std::vector<size_t> GetVecByFunction(const std::vector<size_t>& strfunc) {
        return currentState_->GetVecByFunction(strfunc);
    }

    template <char anychar, char block = '#'>
    std::vector<size_t> GetFunctionResult(const std::string& str) {
        size_t len = str.size();
        std::vector<size_t> z_func(len, 0);
        size_t left = 0;
        size_t right = 0;
        for (size_t ind = 1; ind < len; ++ind) {
            if (ind <= right) {
                z_func[ind] = std::min(right - ind + 1, z_func[ind - left]);
            }
            for (; ind + z_func[ind] < len && str[z_func[ind]] != block &&
                   str[ind + z_func[ind]] != block &&
                   (str[z_func[ind]] == str[ind + z_func[ind]] ||
                    str[z_func[ind]] == anychar ||
                    str[ind + z_func[ind]] == anychar);
                 ++z_func[ind]) {
            }
            if (ind + z_func[ind] - 1 > right) {
                left = ind;
                right = ind + z_func[ind] - 1;
            }
        }
        return z_func;
    }

private:
    std::shared_ptr<State> currentState_;
};

template <char threshold = '#'>
size_t Period(const std::string& str) {
    StringFunction algo;
    algo.SetState(std::make_shared<ZFunction>());
    if (str.length() == 0) {
        return 0;
    }
    if (str.length() == 1) {
        return 1;
    }
    size_t len = str.length();
    auto tmp = algo.GetFunctionResult(str + threshold + str);
    size_t per = 1;
    for (size_t ind = len + 2; ind <= tmp.size() - tmp.size() / 4; ++ind) {
        size_t rind = ind - len - 1;
        per = std::max(per, tmp[ind] / rind + 1);
    }
    return per;
}

int main() {
    std::string pattern;
    std::string text;
    std::cin >> pattern >> text;
    StringFunction zf;
    zf.SetState(std::make_shared<ZFunction>());

    auto zfunc = zf.GetFunctionResult(pattern + "#" + text);
    std::reverse(pattern.begin(), pattern.end());
    std::reverse(text.begin(), text.end());
    auto revzfunc = zf.GetFunctionResult(pattern + "#" + text);
    size_t lenpattern = pattern.size();
    size_t co = 0;
    for (size_t ind = lenpattern + 1; ind < revzfunc.size() - lenpattern + 1;
         ++ind) {
        size_t revind = revzfunc.size() - ind + 1;
        if (revzfunc[revind] + zfunc[ind] >= lenpattern - 1) {
            ++co;
        }
    }
    std::cout << co << '\n';

    for (size_t ind = lenpattern + 1; ind < revzfunc.size() - lenpattern + 1;
         ++ind) {
        size_t revind = revzfunc.size() - ind + 1;
        if (revzfunc[revind] + zfunc[ind] >= lenpattern - 1) {
            std::cout << ind - lenpattern << ' ';
        }
    }

    return 0;
}

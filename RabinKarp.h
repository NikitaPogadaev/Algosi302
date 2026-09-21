
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace rk {
using u64 = std::uint64_t;
using Hash = std::array<u64, 2>;
constexpr Hash MOD{1'000'000'007ULL, 1'000'000'009ULL};
struct Params {
    Hash base;
};
Params make_params(std::mt19937_64 &rng) {
    Params p{};
    for (int k = 0; k < 2; ++k)
        p.base[k] = std::uniform_int_distribution<u64>(257, MOD[k] - 2)(rng);
    return p;
}

u64 code(char ch) { return static_cast<unsigned char>(ch) + 1ULL; }

Hash hash_of(std::string_view s, const Params &p) {
    Hash h{};
    for (char ch : s)
        for (int k = 0; k < 2; ++k)
            h[k] = (h[k] * p.base[k] + code(ch)) % MOD[k];
    return h;
}

class RollingHash {
    Params p_;
    Hash h_{};
    Hash highest_{1, 1};

  public:
    RollingHash(std::string_view window, const Params &p) : p_(p) {
        if (window.empty())
            throw std::invalid_argument("empty window");
        h_ = hash_of(window, p_);
        for (std::size_t j = 1; j < window.size(); ++j)
            for (int k = 0; k < 2; ++k)
                highest_[k] = highest_[k] * p_.base[k] % MOD[k];
    }
    Hash value() const { return h_; }
    void slide(char outgoing, char incoming) {
        for (int k = 0; k < 2; ++k) {
            u64 removed = code(outgoing) * highest_[k] % MOD[k];
            u64 rest = (h_[k] + MOD[k] - removed) % MOD[k];
            h_[k] = (rest * p_.base[k] + code(incoming)) % MOD[k];
        }
    }
};

template <class Emit>
void rabin_karp(std::string_view text, std::string_view pattern,
                const Params &p, Emit emit) {
    const std::size_t n = text.size(), m = pattern.size();
    if (m == 0) {
        for (std::size_t i = 0; i <= n; ++i) {
            emit(i);
        }
        return;
    }
    if (m > n)
        return;
    const Hash target = hash_of(pattern, p);
    RollingHash window(text.substr(0, m), p);
    for (std::size_t i = 0;; ++i) {
        if (window.value() == target &&
            std::equal(pattern.begin(), pattern.end(), text.begin() + i)) {
            emit(i);
        }
        if (i == n - m)
            break;
        window.slide(text[i], text[i + m]);
    }
}
class PrefixHash {
    Params p_;
    std::vector<Hash> pref_, powers_;

  public:
    PrefixHash(std::string_view s, const Params &p)
        : p_(p), pref_(s.size() + 1), powers_(s.size() + 1) {
        powers_[0] = {1, 1};
        for (std::size_t i = 0; i < s.size(); ++i)
            for (int k = 0; k < 2; ++k) {
                powers_[i + 1][k] = powers_[i][k] * p_.base[k] % MOD[k];
                pref_[i + 1][k] =
                    (pref_[i][k] * p_.base[k] + code(s[i])) % MOD[k];
            }
    }
    std::size_t size() const { return pref_.size() - 1; }
    const Params &params() const { return p_; }
    // Вероятностный отпечаток s[l..r); одинаковые хэши не доказывают равенство.
    Hash get(std::size_t l, std::size_t r) const {
        if (l > r || r > size())
            throw std::out_of_range("substring");
        Hash h{};
        for (int k = 0; k < 2; ++k) {
            u64 removed = pref_[l][k] * powers_[r - l][k] % MOD[k];
            h[k] = (pref_[r][k] + MOD[k] - removed) % MOD[k];
        }
        return h;
    }
};

void require_same_params(const PrefixHash &a, const PrefixHash &b) {
    if (a.params().base != b.params().base)
        throw std::invalid_argument("different hash parameters");
}

bool equal_substrings(const PrefixHash &a, std::size_t l, std::size_t r,
                      const PrefixHash &b, std::size_t u, std::size_t v) {
    require_same_params(a, b);
    if (l > r || r > a.size() || u > v || v > b.size())
        throw std::out_of_range("substring");
    return r - l == v - u && a.get(l, r) == b.get(u, v);
}

std::size_t lcp(const PrefixHash &a, std::size_t i, const PrefixHash &b,
                std::size_t j) {
    require_same_params(a, b);
    if (i > a.size() || j > b.size())
        throw std::out_of_range("suffix");
    std::size_t lo = 0, hi = std::min(a.size() - i, b.size() - j);
    while (lo < hi) {
        const std::size_t span = hi - lo;
        const std::size_t mid = lo + span / 2 + span % 2;
        if (a.get(i, i + mid) == b.get(j, j + mid))
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

bool is_palindrome(const PrefixHash &forward, const PrefixHash &reverse_hash,
                   std::size_t l, std::size_t r) {
    require_same_params(forward, reverse_hash);
    const std::size_t n = forward.size();
    if (reverse_hash.size() != n || l > r || r > n)
        throw std::out_of_range("palindrome interval");
    return forward.get(l, r) == reverse_hash.get(n - r, n - l);
}
} // namespace rk

namespace tests {
void check(bool ok, const char *message) {
    if (!ok)
        throw std::runtime_error(message);
}
std::vector<std::size_t> naive_find(std::string_view t, std::string_view p) {
    std::vector<std::size_t> out;
    if (p.size() > t.size())
        return out;
    for (std::size_t i = 0; i <= t.size() - p.size(); ++i)
        if (t.substr(i, p.size()) == p)
            out.push_back(i);
    return out;
}
void run() {
    std::mt19937_64 rng(20260918);
    const auto p = rk::make_params(rng);
    auto verify_search = [&](const std::string &t, const std::string &needle) {
        std::vector<std::size_t> actual;
        rk::rabin_karp(t, needle, p,
                       [&](std::size_t i) { actual.push_back(i); });
        check(actual == naive_find(t, needle), "Rabin-Karp mismatch");
    };
    verify_search("", "");
    verify_search("", "a");
    verify_search("aaaaa", "aaa");
    verify_search("abc", "");
    verify_search("abc", "abc");
    verify_search("abc", "abcd");
    verify_search("abc", "c");
    verify_search("abc", "z");

    // Плохое основание специально вызывает коллизию в ОБОИХ компонентах.
    // ab и ba имеют одинаковую сумму кодов при базисе 1.
    const rk::Params bad{{1, 1}};
    check(rk::hash_of("ab", bad) == rk::hash_of("ba", bad),
          "collision fixture");
    std::vector<std::size_t> exact;
    rk::rabin_karp("baab", "ab", bad,
                   [&](std::size_t i) { exact.push_back(i); });
    check(exact == std::vector<std::size_t>{2}, "collision not rejected");

    for (int test = 0; test < 1200; ++test) {
        std::string t(rng() % 33, '\0'), needle(rng() % 37, '\0');
        for (char &ch : t)
            ch = static_cast<char>(rng() % (test % 2 ? 4 : 256));
        for (char &ch : needle)
            ch = static_cast<char>(rng() % (test % 2 ? 4 : 256));
        if (test % 3 == 0 && !t.empty()) {
            const std::size_t left = rng() % t.size();
            needle = t.substr(left, rng() % (t.size() - left + 1));
        }
        verify_search(t, needle);
        const rk::PrefixHash h(t, p);
        const std::string rev(t.rbegin(), t.rend());
        const rk::PrefixHash hr(rev, p);
        for (std::size_t l = 0; l <= t.size(); ++l)
            for (std::size_t r = l; r <= t.size(); ++r) {
                const std::string sub = t.substr(l, r - l);
                check(h.get(l, r) == rk::hash_of(sub, p), "prefix mismatch");
                const std::string back(sub.rbegin(), sub.rend());
                check(rk::is_palindrome(h, hr, l, r) == (sub == back),
                      "palindrome mismatch");
            }
        for (std::size_t m = 1; m <= t.size(); ++m) {
            rk::RollingHash rolling(std::string_view(t).substr(0, m), p);
            for (std::size_t i = 0; i <= t.size() - m; ++i) {
                check(rolling.value() ==
                          rk::hash_of(std::string_view(t).substr(i, m), p),
                      "rolling mismatch");
                if (i < t.size() - m)
                    rolling.slide(t[i], t[i + m]);
            }
        }
        const rk::PrefixHash other(needle, p);
        for (std::size_t i = 0; i <= t.size(); ++i)
            for (std::size_t j = 0; j <= needle.size(); ++j) {
                std::size_t length = 0;
                while (i + length < t.size() && j + length < needle.size() &&
                       t[i + length] == needle[j + length])
                    ++length;
                check(rk::lcp(h, i, other, j) == length, "LCP mismatch");
                check(rk::equal_substrings(h, i, i + length, other, j,
                                           j + length),
                      "equal substring mismatch");
            }
    }
    bool rejected = false;
    try {
        rk::PrefixHash h("abc", p), other("abc", bad);
        (void)rk::lcp(h, 0, other, 0);
    } catch (const std::invalid_argument &) {
        rejected = true;
    }
    check(rejected, "parameter mismatch accepted");
    std::cout << "OK: edge cases, forced collision, 1200 randomized cases\n";
}
} // namespace tests

// int main(int argc, char **argv) {
//     if (argc > 1 && std::string_view(argv[1]) == "--self-test") {
//         tests::run();
//         return 0;
//     }
//     std::string text, pattern;
//     if (!std::getline(std::cin, text) || !std::getline(std::cin, pattern))
//         return 0;
//     std::random_device rd;
//     std::seed_seq seed{rd(), rd(), rd(), rd()};
//     std::mt19937_64 rng(seed);
//     const auto params = rk::make_params(rng);
//     bool first = true;
//     rk::rabin_karp(text, pattern, params, [&](std::size_t pos) {
//         if (!first)
//             std::cout << ' ';
//         std::cout << pos;
//         first = false;
//     });
//     std::cout << '\n';
// }

#include <stdexcept>
#include <type_traits>

namespace constexpr_traits {

namespace detail {

// see
// https://stackoverflow.com/questions/15232758/detecting-constexpr-with-sfinae

// Call the provided method with the provided args.
// This gives us a non-type template parameter for void-returning F.
// This wouldn't be needed if "auto = F(Args...)" was a valid template
// parameter for void-returning F.
template <auto F, auto... Args>
constexpr void* constexpr_caller() {
    F(Args...);
    return nullptr;
}

// Takes a parameter with elipsis conversion, so will never be selected
// when another viable overload is present
template <auto F, auto... Args>
constexpr bool is_constexpr(...) {
    return false;
}

// Fails substitution if constexpr_caller<F, Args...>() can't be
// called in constexpr context
template <auto F, auto... Args, auto = constexpr_caller<F, Args...>()>
constexpr bool is_constexpr(int) {
    return true;
}

}  // namespace detail

template <auto F, auto... Args>
struct invoke_constexpr
    : std::bool_constant<detail::is_constexpr<F, Args...>(0)> {};

}  // namespace constexpr_traits

template <typename T>
auto identity(T&& t) {
    return t;
}

#define typeval(__value)                                                   \
    [&]() {                                                                \
        struct __anonymous_constexpr {                                     \
        public:                                                            \
            using type = std::decay_t<decltype(__value)>;                  \
            constexpr __anonymous_constexpr(type value) : _value{value} {} \
            constexpr auto operator()() const { return _value; }           \
            constexpr      operator type() const { return _value; }        \
                                                                           \
        private:                                                           \
            type _value;                                                   \
        };                                                                 \
        return __anonymous_constexpr(__value);                             \
    }()

template <typename T, T N>
struct arithmetic_expression {
    template <typename U, U RHS>
    constexpr arithmetic_expression<std::common_type_t<T, U>, RHS + N>
    operator+(arithmetic_expression<U, RHS> rhs) const {
        return {};
    }

    template <typename U, U RHS>
    constexpr arithmetic_expression<std::common_type_t<T, U>, RHS - N>
    operator-(arithmetic_expression<U, RHS> rhs) const {
        return {};
    }

    template <typename U, U RHS>
    constexpr arithmetic_expression<std::common_type_t<T, U>, RHS * N>
    operator*(arithmetic_expression<U, RHS> rhs) const {
        return {};
    }

    template <typename U, U RHS>
    constexpr arithmetic_expression<std::common_type_t<T, U>, RHS / N>
    operator/(arithmetic_expression<U, RHS> rhs) const {
        return {};
    }

    constexpr arithmetic_expression<
            std::make_signed_t<T>,
            -static_cast<std::make_signed_t<T>>(N)>
    operator-() const {
        return {};
    }

    constexpr   operator T() const { return N; }
    constexpr T operator()() const { return N; }
};

constexpr int combine(int p) {
    return p;
}

template <class... TT>
constexpr int combine(int val, int p0, TT... pp) {
    return combine(val * 10 + p0, pp...);
}

constexpr int parse(char C) {
    return (C >= '0' && C <= '9')
                   ? C - '0'
                   : throw std::out_of_range("only decimal digits are allowed");
}

template <char... Digits>
constexpr auto operator"" _c() -> arithmetic_expression<
        unsigned long long int,
        combine(0, parse(Digits)...)> {
    return {};
}

#include <constexpr_param/constexpr_param.h>

#include <gtest/gtest.h>

#include <array>

template <typename B>
__attribute((noinline)) static auto print(B b) {
    if (b) {
        return 5;
    } else {
        return 10;
    }
}

template <typename T, T N>
__attribute((noinline)) static constexpr auto
evaluate(arithmetic_expression<T, N> expr) {
    return expr();
}

// static_assert(constexpr_traits::invoke_constexpr<print, true>::value);

TEST(constexpr_params, basics) {
    {
        auto result = print(typeval(true));
        EXPECT_EQ(result, 5);
    }
    // {
    //     volatile bool b      = true;
    //     auto          result = print(
    //             [=]() {
    //                 struct _anon {
    //                     constexpr _anon(bool c) : a{c} {}
    //                     bool a;
    //                     constexpr operator bool() { return a; }
    //                 };
    //                 return _anon(b);
    //             }(),
    //             a);
    // }
    {
        volatile bool b      = false;
        auto          result = print(typeval(b));
        EXPECT_EQ(result, 10);
    }

    EXPECT_EQ((arithmetic_expression<int, 5>()), 5_c);
    EXPECT_EQ(evaluate(5_c + 5_c), evaluate(10_c));
    EXPECT_EQ(evaluate(5_c * 5_c), evaluate(25_c));
    EXPECT_EQ(evaluate(5_c / 5_c), evaluate(1_c));
    EXPECT_EQ(evaluate(-5_c + 5_c), evaluate(0_c));

    constexpr size_t i = 1;

    std::array<int, typeval(i)> arr{{1}};
    for (auto& e : arr) {
        printf("%d\n", e);
    }
}

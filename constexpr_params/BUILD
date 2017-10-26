cc_library(
	name="constexpr_param",
	hdrs=["include/constexpr_param/constexpr_param.h"],
    strip_include_prefix="include"
)

cc_binary(
	name="constexpr_param_tests",
	srcs=["tests/test_constexpr_param.cpp"],
	deps=[
		"constexpr_param",
        "@gtest//:main"
	]
)

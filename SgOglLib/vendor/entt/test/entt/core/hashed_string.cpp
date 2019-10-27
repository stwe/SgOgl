#include <string>
#include <string_view>
#include <type_traits>
#include <gtest/gtest.h>
#include <entt/core/hashed_string.hpp>

TEST(HashedString, Functionalities) {
    using hash_type = entt::hashed_string::hash_type;

    const char *bar = "bar";

    auto foo_hs = entt::hashed_string{"foo"};
    auto bar_hs = entt::hashed_string{bar};

    ASSERT_NE(static_cast<hash_type>(foo_hs), static_cast<hash_type>(bar_hs));
    ASSERT_STREQ(static_cast<const char *>(foo_hs), "foo");
    ASSERT_STREQ(static_cast<const char *>(bar_hs), bar);
    ASSERT_STREQ(foo_hs.data(), "foo");
    ASSERT_STREQ(bar_hs.data(), bar);

    ASSERT_EQ(foo_hs, foo_hs);
    ASSERT_NE(foo_hs, bar_hs);

    entt::hashed_string hs{"foobar"};

    ASSERT_EQ(static_cast<hash_type>(hs), 0xbf9cf968);
    ASSERT_EQ(hs.value(), 0xbf9cf968);

    ASSERT_EQ(foo_hs, "foo"_hs);
    ASSERT_NE(bar_hs, "foo"_hs);
}

TEST(HashedString, Empty) {
    using hash_type = entt::hashed_string::hash_type;

    entt::hashed_string hs{};

    ASSERT_EQ(static_cast<hash_type>(hs), hash_type{});
    ASSERT_EQ(static_cast<const char *>(hs), nullptr);
}

TEST(HashedString, Constexprness) {
    using hash_type = entt::hashed_string::hash_type;
    // how would you test a constexpr otherwise?
    (void)std::integral_constant<hash_type, entt::hashed_string{"quux"}>{};
    (void)std::integral_constant<hash_type, "quux"_hs>{};
    ASSERT_TRUE(true);
}

TEST(HashedString, ToValue) {
    using hash_type = entt::hashed_string::hash_type;

    const char *foobar = "foobar";

    ASSERT_EQ(entt::hashed_string::to_value(foobar), 0xbf9cf968);
    // how would you test a constexpr otherwise?
    (void)std::integral_constant<hash_type, entt::hashed_string::to_value("quux")>{};
}

TEST(HashedString, StringView) {
    std::string str{"__foobar__"};
    std::string_view view{str.data()+2, 6};
    ASSERT_EQ(entt::hashed_string::to_value(view.data(), view.size()), 0xbf9cf968);
}

TEST(HashedWString, Functionalities) {
    using hash_type = entt::hashed_wstring::hash_type;

    const wchar_t *bar = L"bar";

    auto foo_hws = entt::hashed_wstring{L"foo"};
    auto bar_hws = entt::hashed_wstring{bar};

    ASSERT_NE(static_cast<hash_type>(foo_hws), static_cast<hash_type>(bar_hws));
    ASSERT_STREQ(static_cast<const wchar_t *>(foo_hws), L"foo");
    ASSERT_STREQ(static_cast<const wchar_t *>(bar_hws), bar);
    ASSERT_STREQ(foo_hws.data(), L"foo");
    ASSERT_STREQ(bar_hws.data(), bar);

    ASSERT_EQ(foo_hws, foo_hws);
    ASSERT_NE(foo_hws, bar_hws);

    entt::hashed_wstring hws{L"foobar"};

    ASSERT_EQ(static_cast<hash_type>(hws), 0xbf9cf968);
    ASSERT_EQ(hws.value(), 0xbf9cf968);

    ASSERT_EQ(foo_hws, L"foo"_hws);
    ASSERT_NE(bar_hws, L"foo"_hws);
}

TEST(HashedWString, Empty) {
    using hash_type = entt::hashed_wstring::hash_type;

    entt::hashed_wstring hws{};

    ASSERT_EQ(static_cast<hash_type>(hws), hash_type{});
    ASSERT_EQ(static_cast<const wchar_t *>(hws), nullptr);
}

TEST(HashedWString, Constexprness) {
    using hash_type = entt::hashed_wstring::hash_type;
    // how would you test a constexpr otherwise?
    (void)std::integral_constant<hash_type, entt::hashed_wstring{L"quux"}>{};
    (void)std::integral_constant<hash_type, L"quux"_hws>{};
    ASSERT_TRUE(true);
}

TEST(HashedWString, ToValue) {
    using hash_type = entt::hashed_wstring::hash_type;

    const wchar_t *foobar = L"foobar";

    ASSERT_EQ(entt::hashed_wstring::to_value(foobar), 0xbf9cf968);
    // how would you test a constexpr otherwise?
    (void)std::integral_constant<hash_type, entt::hashed_wstring::to_value(L"quux")>{};
}

TEST(HashedWString, StringView) {
    std::wstring str{L"__foobar__"};
    std::wstring_view view{str.data()+2, 6};
    ASSERT_EQ(entt::hashed_wstring::to_value(view.data(), view.size()), 0xbf9cf968);
}

TEST(BasicHashedString, DeductionGuide) {
    static_assert(std::is_same_v<decltype(entt::basic_hashed_string{"foo"}), entt::hashed_string>);
    static_assert(std::is_same_v<decltype(entt::basic_hashed_string{L"foo"}), entt::hashed_wstring>);
}

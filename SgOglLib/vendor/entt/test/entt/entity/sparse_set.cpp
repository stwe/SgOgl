#include <cstdint>
#include <utility>
#include <iterator>
#include <type_traits>
#include <gtest/gtest.h>
#include <entt/entity/sparse_set.hpp>
#include <entt/entity/fwd.hpp>

struct empty_type {};
struct boxed_int { int value; };

TEST(SparseSet, Functionalities) {
    entt::sparse_set<entt::entity> set;

    set.reserve(42);

    ASSERT_EQ(set.capacity(), 42);
    ASSERT_TRUE(set.empty());
    ASSERT_EQ(set.size(), 0u);
    ASSERT_EQ(std::as_const(set).begin(), std::as_const(set).end());
    ASSERT_EQ(set.begin(), set.end());
    ASSERT_FALSE(set.has(entt::entity{0}));
    ASSERT_FALSE(set.has(entt::entity{42}));

    set.construct(entt::entity{42});

    ASSERT_EQ(set.index(entt::entity{42}), 0u);

    ASSERT_FALSE(set.empty());
    ASSERT_EQ(set.size(), 1u);
    ASSERT_NE(std::as_const(set).begin(), std::as_const(set).end());
    ASSERT_NE(set.begin(), set.end());
    ASSERT_FALSE(set.has(entt::entity{0}));
    ASSERT_TRUE(set.has(entt::entity{42}));
    ASSERT_EQ(set.index(entt::entity{42}), 0u);

    set.destroy(entt::entity{42});

    ASSERT_TRUE(set.empty());
    ASSERT_EQ(set.size(), 0u);
    ASSERT_EQ(std::as_const(set).begin(), std::as_const(set).end());
    ASSERT_EQ(set.begin(), set.end());
    ASSERT_FALSE(set.has(entt::entity{0}));
    ASSERT_FALSE(set.has(entt::entity{42}));

    set.construct(entt::entity{42});

    ASSERT_FALSE(set.empty());
    ASSERT_EQ(set.index(entt::entity{42}), 0u);

    ASSERT_TRUE(std::is_move_constructible_v<decltype(set)>);
    ASSERT_TRUE(std::is_move_assignable_v<decltype(set)>);

    entt::sparse_set<entt::entity> cpy{set};
    set = cpy;

    ASSERT_FALSE(set.empty());
    ASSERT_FALSE(cpy.empty());
    ASSERT_EQ(set.index(entt::entity{42}), 0u);
    ASSERT_EQ(cpy.index(entt::entity{42}), 0u);

    entt::sparse_set<entt::entity> other{std::move(set)};

    set = std::move(other);
    other = std::move(set);

    ASSERT_TRUE(set.empty());
    ASSERT_FALSE(other.empty());
    ASSERT_EQ(other.index(entt::entity{42}), 0u);

    other.reset();

    ASSERT_TRUE(other.empty());
    ASSERT_EQ(other.size(), 0u);
    ASSERT_EQ(std::as_const(other).begin(), std::as_const(other).end());
    ASSERT_EQ(other.begin(), other.end());
    ASSERT_FALSE(other.has(entt::entity{0}));
    ASSERT_FALSE(other.has(entt::entity{42}));
}

TEST(SparseSet, Pagination) {
    entt::sparse_set<entt::entity> set;
    constexpr auto entt_per_page = ENTT_PAGE_SIZE / sizeof(std::underlying_type_t<entt::entity>);

    ASSERT_EQ(set.extent(), 0);

    set.construct(entt::entity{entt_per_page-1});

    ASSERT_EQ(set.extent(), entt_per_page);
    ASSERT_TRUE(set.has(entt::entity{entt_per_page-1}));

    set.construct(entt::entity{entt_per_page});

    ASSERT_EQ(set.extent(), 2 * entt_per_page);
    ASSERT_TRUE(set.has(entt::entity{entt_per_page-1}));
    ASSERT_TRUE(set.has(entt::entity{entt_per_page}));
    ASSERT_FALSE(set.has(entt::entity{entt_per_page+1}));

    set.destroy(entt::entity{entt_per_page-1});

    ASSERT_EQ(set.extent(), 2 * entt_per_page);
    ASSERT_FALSE(set.has(entt::entity{entt_per_page-1}));
    ASSERT_TRUE(set.has(entt::entity{entt_per_page}));

    set.shrink_to_fit();
    set.destroy(entt::entity{entt_per_page});

    ASSERT_EQ(set.extent(), 2 * entt_per_page);
    ASSERT_FALSE(set.has(entt::entity{entt_per_page-1}));
    ASSERT_FALSE(set.has(entt::entity{entt_per_page}));

    set.shrink_to_fit();

    ASSERT_EQ(set.extent(), 0);
}

TEST(SparseSet, BatchAdd) {
    entt::sparse_set<entt::entity> set;
    entt::entity entities[2];

    entities[0] = entt::entity{3};
    entities[1] = entt::entity{42};

    set.construct(entt::entity{12});
    set.batch(std::begin(entities), std::end(entities));
    set.construct(entt::entity{24});

    ASSERT_TRUE(set.has(entities[0]));
    ASSERT_TRUE(set.has(entities[1]));
    ASSERT_FALSE(set.has(entt::entity{0}));
    ASSERT_FALSE(set.has(entt::entity{9}));
    ASSERT_TRUE(set.has(entt::entity{12}));
    ASSERT_TRUE(set.has(entt::entity{24}));

    ASSERT_FALSE(set.empty());
    ASSERT_EQ(set.size(), 4u);
    ASSERT_EQ(set.index(entt::entity{12}), 0u);
    ASSERT_EQ(set.index(entities[0]), 1u);
    ASSERT_EQ(set.index(entities[1]), 2u);
    ASSERT_EQ(set.index(entt::entity{24}), 3u);
    ASSERT_EQ(set.data()[set.index(entt::entity{12})], entt::entity{12});
    ASSERT_EQ(set.data()[set.index(entities[0])], entities[0]);
    ASSERT_EQ(set.data()[set.index(entities[1])], entities[1]);
    ASSERT_EQ(set.data()[set.index(entt::entity{24})], entt::entity{24});
}

TEST(SparseSet, Iterator) {
    using iterator_type = typename entt::sparse_set<entt::entity>::iterator_type;

    entt::sparse_set<entt::entity> set;
    set.construct(entt::entity{3});

    iterator_type end{set.begin()};
    iterator_type begin{};
    begin = set.end();
    std::swap(begin, end);

    ASSERT_EQ(begin, set.begin());
    ASSERT_EQ(end, set.end());
    ASSERT_NE(begin, end);

    ASSERT_EQ(begin++, set.begin());
    ASSERT_EQ(begin--, set.end());

    ASSERT_EQ(begin+1, set.end());
    ASSERT_EQ(end-1, set.begin());

    ASSERT_EQ(++begin, set.end());
    ASSERT_EQ(--begin, set.begin());

    ASSERT_EQ(begin += 1, set.end());
    ASSERT_EQ(begin -= 1, set.begin());

    ASSERT_EQ(begin + (end - begin), set.end());
    ASSERT_EQ(begin - (begin - end), set.end());

    ASSERT_EQ(end - (end - begin), set.begin());
    ASSERT_EQ(end + (begin - end), set.begin());

    ASSERT_EQ(begin[0], *set.begin());

    ASSERT_LT(begin, end);
    ASSERT_LE(begin, set.begin());

    ASSERT_GT(end, begin);
    ASSERT_GE(end, set.end());

    ASSERT_EQ(*begin, entt::entity{3});
    ASSERT_EQ(*begin.operator->(), entt::entity{3});
}

TEST(SparseSet, Find) {
    entt::sparse_set<entt::entity> set;
    set.construct(entt::entity{3});
    set.construct(entt::entity{42});
    set.construct(entt::entity{99});

    ASSERT_NE(set.find(entt::entity{3}), set.end());
    ASSERT_NE(set.find(entt::entity{42}), set.end());
    ASSERT_NE(set.find(entt::entity{99}), set.end());
    ASSERT_EQ(set.find(entt::entity{0}), set.end());

    auto it = set.find(entt::entity{99});

    ASSERT_EQ(*it, entt::entity{99});
    ASSERT_EQ(*(++it), entt::entity{42});
    ASSERT_EQ(*(++it), entt::entity{3});
    ASSERT_EQ(++it, set.end());
    ASSERT_EQ(++set.find(entt::entity{3}), set.end());
}

TEST(SparseSet, Data) {
    entt::sparse_set<entt::entity> set;

    set.construct(entt::entity{3});
    set.construct(entt::entity{12});
    set.construct(entt::entity{42});

    ASSERT_EQ(set.index(entt::entity{3}), 0u);
    ASSERT_EQ(set.index(entt::entity{12}), 1u);
    ASSERT_EQ(set.index(entt::entity{42}), 2u);

    ASSERT_EQ(*(set.data() + 0u), entt::entity{3});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{42});
}

TEST(SparseSet, SortOrdered) {
    entt::sparse_set<entt::entity> set;

    set.construct(entt::entity{42});
    set.construct(entt::entity{12});
    set.construct(entt::entity{9});
    set.construct(entt::entity{7});
    set.construct(entt::entity{3});

    ASSERT_EQ(*(set.data() + 0u), entt::entity{42});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{3});

    set.sort(set.begin(), set.end(), [](const auto lhs, const auto rhs) {
        return std::underlying_type_t<entt::entity>(lhs) < std::underlying_type_t<entt::entity>(rhs);
    });

    ASSERT_EQ(*(set.data() + 0u), entt::entity{42});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{3});

    auto begin = set.begin();
    auto end = set.end();

    ASSERT_EQ(*(begin++), entt::entity{3});
    ASSERT_EQ(*(begin++), entt::entity{7});
    ASSERT_EQ(*(begin++), entt::entity{9});
    ASSERT_EQ(*(begin++), entt::entity{12});
    ASSERT_EQ(*(begin++), entt::entity{42});
    ASSERT_EQ(begin, end);
}

TEST(SparseSet, SortReverse) {
    entt::sparse_set<entt::entity> set;

    set.construct(entt::entity{3});
    set.construct(entt::entity{7});
    set.construct(entt::entity{9});
    set.construct(entt::entity{12});
    set.construct(entt::entity{42});

    ASSERT_EQ(*(set.data() + 0u), entt::entity{3});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{42});

    set.sort(set.begin(), set.end(), [](const auto lhs, const auto rhs) {
        return std::underlying_type_t<entt::entity>(lhs) < std::underlying_type_t<entt::entity>(rhs);
    });

    ASSERT_EQ(*(set.data() + 0u), entt::entity{42});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{3});

    auto begin = set.begin();
    auto end = set.end();

    ASSERT_EQ(*(begin++), entt::entity{3});
    ASSERT_EQ(*(begin++), entt::entity{7});
    ASSERT_EQ(*(begin++), entt::entity{9});
    ASSERT_EQ(*(begin++), entt::entity{12});
    ASSERT_EQ(*(begin++), entt::entity{42});
    ASSERT_EQ(begin, end);
}

TEST(SparseSet, SortUnordered) {
    entt::sparse_set<entt::entity> set;

    set.construct(entt::entity{9});
    set.construct(entt::entity{7});
    set.construct(entt::entity{3});
    set.construct(entt::entity{12});
    set.construct(entt::entity{42});

    ASSERT_EQ(*(set.data() + 0u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{3});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{42});

    set.sort(set.begin(), set.end(), [](const auto lhs, const auto rhs) {
        return std::underlying_type_t<entt::entity>(lhs) < std::underlying_type_t<entt::entity>(rhs);
    });

    ASSERT_EQ(*(set.data() + 0u), entt::entity{42});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{3});

    auto begin = set.begin();
    auto end = set.end();

    ASSERT_EQ(*(begin++), entt::entity{3});
    ASSERT_EQ(*(begin++), entt::entity{7});
    ASSERT_EQ(*(begin++), entt::entity{9});
    ASSERT_EQ(*(begin++), entt::entity{12});
    ASSERT_EQ(*(begin++), entt::entity{42});
    ASSERT_EQ(begin, end);
}

TEST(SparseSet, SortRange) {
    entt::sparse_set<entt::entity> set;

    set.construct(entt::entity{9});
    set.construct(entt::entity{7});
    set.construct(entt::entity{3});
    set.construct(entt::entity{12});
    set.construct(entt::entity{42});

    ASSERT_EQ(*(set.data() + 0u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{3});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{42});

    set.sort(set.end(), set.end(), [](const auto lhs, const auto rhs) {
        return std::underlying_type_t<entt::entity>(lhs) < std::underlying_type_t<entt::entity>(rhs);
    });

    ASSERT_EQ(*(set.data() + 0u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{3});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{42});

    set.sort(set.begin(), set.begin(), [](const auto lhs, const auto rhs) {
        return std::underlying_type_t<entt::entity>(lhs) < std::underlying_type_t<entt::entity>(rhs);
    });

    ASSERT_EQ(*(set.data() + 0u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{3});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{42});

    set.sort(set.begin()+2, set.begin()+3, [](const auto lhs, const auto rhs) {
        return std::underlying_type_t<entt::entity>(lhs) < std::underlying_type_t<entt::entity>(rhs);
    });

    ASSERT_EQ(*(set.data() + 0u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{3});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{42});

    set.sort(++set.begin(), --set.end(), [](const auto lhs, const auto rhs) {
        return std::underlying_type_t<entt::entity>(lhs) < std::underlying_type_t<entt::entity>(rhs);
    });

    ASSERT_EQ(*(set.data() + 0u), entt::entity{9});
    ASSERT_EQ(*(set.data() + 1u), entt::entity{12});
    ASSERT_EQ(*(set.data() + 2u), entt::entity{7});
    ASSERT_EQ(*(set.data() + 3u), entt::entity{3});
    ASSERT_EQ(*(set.data() + 4u), entt::entity{42});

    auto begin = set.begin();
    auto end = set.end();

    ASSERT_EQ(*(begin++), entt::entity{42});
    ASSERT_EQ(*(begin++), entt::entity{3});
    ASSERT_EQ(*(begin++), entt::entity{7});
    ASSERT_EQ(*(begin++), entt::entity{12});
    ASSERT_EQ(*(begin++), entt::entity{9});
    ASSERT_EQ(begin, end);
}

TEST(SparseSet, RespectDisjoint) {
    entt::sparse_set<entt::entity> lhs;
    entt::sparse_set<entt::entity> rhs;

    lhs.construct(entt::entity{3});
    lhs.construct(entt::entity{12});
    lhs.construct(entt::entity{42});

    ASSERT_EQ(lhs.index(entt::entity{3}), 0u);
    ASSERT_EQ(lhs.index(entt::entity{12}), 1u);
    ASSERT_EQ(lhs.index(entt::entity{42}), 2u);

    lhs.respect(rhs);

    ASSERT_EQ(std::as_const(lhs).index(entt::entity{3}), 0u);
    ASSERT_EQ(std::as_const(lhs).index(entt::entity{12}), 1u);
    ASSERT_EQ(std::as_const(lhs).index(entt::entity{42}), 2u);
}

TEST(SparseSet, RespectOverlap) {
    entt::sparse_set<entt::entity> lhs;
    entt::sparse_set<entt::entity> rhs;

    lhs.construct(entt::entity{3});
    lhs.construct(entt::entity{12});
    lhs.construct(entt::entity{42});

    rhs.construct(entt::entity{12});

    ASSERT_EQ(lhs.index(entt::entity{3}), 0u);
    ASSERT_EQ(lhs.index(entt::entity{12}), 1u);
    ASSERT_EQ(lhs.index(entt::entity{42}), 2u);

    lhs.respect(rhs);

    ASSERT_EQ(std::as_const(lhs).index(entt::entity{3}), 0u);
    ASSERT_EQ(std::as_const(lhs).index(entt::entity{12}), 2u);
    ASSERT_EQ(std::as_const(lhs).index(entt::entity{42}), 1u);
}

TEST(SparseSet, RespectOrdered) {
    entt::sparse_set<entt::entity> lhs;
    entt::sparse_set<entt::entity> rhs;

    lhs.construct(entt::entity{1});
    lhs.construct(entt::entity{2});
    lhs.construct(entt::entity{3});
    lhs.construct(entt::entity{4});
    lhs.construct(entt::entity{5});

    ASSERT_EQ(lhs.index(entt::entity{1}), 0u);
    ASSERT_EQ(lhs.index(entt::entity{2}), 1u);
    ASSERT_EQ(lhs.index(entt::entity{3}), 2u);
    ASSERT_EQ(lhs.index(entt::entity{4}), 3u);
    ASSERT_EQ(lhs.index(entt::entity{5}), 4u);

    rhs.construct(entt::entity{6});
    rhs.construct(entt::entity{1});
    rhs.construct(entt::entity{2});
    rhs.construct(entt::entity{3});
    rhs.construct(entt::entity{4});
    rhs.construct(entt::entity{5});

    ASSERT_EQ(rhs.index(entt::entity{6}), 0u);
    ASSERT_EQ(rhs.index(entt::entity{1}), 1u);
    ASSERT_EQ(rhs.index(entt::entity{2}), 2u);
    ASSERT_EQ(rhs.index(entt::entity{3}), 3u);
    ASSERT_EQ(rhs.index(entt::entity{4}), 4u);
    ASSERT_EQ(rhs.index(entt::entity{5}), 5u);

    rhs.respect(lhs);

    ASSERT_EQ(rhs.index(entt::entity{6}), 0u);
    ASSERT_EQ(rhs.index(entt::entity{1}), 1u);
    ASSERT_EQ(rhs.index(entt::entity{2}), 2u);
    ASSERT_EQ(rhs.index(entt::entity{3}), 3u);
    ASSERT_EQ(rhs.index(entt::entity{4}), 4u);
    ASSERT_EQ(rhs.index(entt::entity{5}), 5u);
}

TEST(SparseSet, RespectReverse) {
    entt::sparse_set<entt::entity> lhs;
    entt::sparse_set<entt::entity> rhs;

    lhs.construct(entt::entity{1});
    lhs.construct(entt::entity{2});
    lhs.construct(entt::entity{3});
    lhs.construct(entt::entity{4});
    lhs.construct(entt::entity{5});

    ASSERT_EQ(lhs.index(entt::entity{1}), 0u);
    ASSERT_EQ(lhs.index(entt::entity{2}), 1u);
    ASSERT_EQ(lhs.index(entt::entity{3}), 2u);
    ASSERT_EQ(lhs.index(entt::entity{4}), 3u);
    ASSERT_EQ(lhs.index(entt::entity{5}), 4u);

    rhs.construct(entt::entity{5});
    rhs.construct(entt::entity{4});
    rhs.construct(entt::entity{3});
    rhs.construct(entt::entity{2});
    rhs.construct(entt::entity{1});
    rhs.construct(entt::entity{6});

    ASSERT_EQ(rhs.index(entt::entity{5}), 0u);
    ASSERT_EQ(rhs.index(entt::entity{4}), 1u);
    ASSERT_EQ(rhs.index(entt::entity{3}), 2u);
    ASSERT_EQ(rhs.index(entt::entity{2}), 3u);
    ASSERT_EQ(rhs.index(entt::entity{1}), 4u);
    ASSERT_EQ(rhs.index(entt::entity{6}), 5u);

    rhs.respect(lhs);

    ASSERT_EQ(rhs.index(entt::entity{6}), 0u);
    ASSERT_EQ(rhs.index(entt::entity{1}), 1u);
    ASSERT_EQ(rhs.index(entt::entity{2}), 2u);
    ASSERT_EQ(rhs.index(entt::entity{3}), 3u);
    ASSERT_EQ(rhs.index(entt::entity{4}), 4u);
    ASSERT_EQ(rhs.index(entt::entity{5}), 5u);
}

TEST(SparseSet, RespectUnordered) {
    entt::sparse_set<entt::entity> lhs;
    entt::sparse_set<entt::entity> rhs;

    lhs.construct(entt::entity{1});
    lhs.construct(entt::entity{2});
    lhs.construct(entt::entity{3});
    lhs.construct(entt::entity{4});
    lhs.construct(entt::entity{5});

    ASSERT_EQ(lhs.index(entt::entity{1}), 0u);
    ASSERT_EQ(lhs.index(entt::entity{2}), 1u);
    ASSERT_EQ(lhs.index(entt::entity{3}), 2u);
    ASSERT_EQ(lhs.index(entt::entity{4}), 3u);
    ASSERT_EQ(lhs.index(entt::entity{5}), 4u);

    rhs.construct(entt::entity{3});
    rhs.construct(entt::entity{2});
    rhs.construct(entt::entity{6});
    rhs.construct(entt::entity{1});
    rhs.construct(entt::entity{4});
    rhs.construct(entt::entity{5});

    ASSERT_EQ(rhs.index(entt::entity{3}), 0u);
    ASSERT_EQ(rhs.index(entt::entity{2}), 1u);
    ASSERT_EQ(rhs.index(entt::entity{6}), 2u);
    ASSERT_EQ(rhs.index(entt::entity{1}), 3u);
    ASSERT_EQ(rhs.index(entt::entity{4}), 4u);
    ASSERT_EQ(rhs.index(entt::entity{5}), 5u);

    rhs.respect(lhs);

    ASSERT_EQ(rhs.index(entt::entity{6}), 0u);
    ASSERT_EQ(rhs.index(entt::entity{1}), 1u);
    ASSERT_EQ(rhs.index(entt::entity{2}), 2u);
    ASSERT_EQ(rhs.index(entt::entity{3}), 3u);
    ASSERT_EQ(rhs.index(entt::entity{4}), 4u);
    ASSERT_EQ(rhs.index(entt::entity{5}), 5u);
}

TEST(SparseSet, CanModifyDuringIteration) {
    entt::sparse_set<entt::entity> set;
    set.construct(entt::entity{0});

    ASSERT_EQ(set.capacity(), entt::sparse_set<entt::entity>::size_type{1});

    const auto it = set.begin();
    set.reserve(entt::sparse_set<entt::entity>::size_type{2});

    ASSERT_EQ(set.capacity(), entt::sparse_set<entt::entity>::size_type{2});

    // this should crash with asan enabled if we break the constraint
    const auto entity = *it;
    (void)entity;
}

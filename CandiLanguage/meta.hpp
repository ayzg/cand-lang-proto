#include <string>

inline constexpr std::string string_from_char(char x)
{
	// https://www.geeksforgeeks.org/how-to-convert-a-single-character-to-string-in-cpp/
	// string class has a constructor
	// that allows us to specify size of
	// string as first parameter and character
	// to be filled in given size as second
	// parameter.
	std::string s(1, x);

	return s;
}

namespace detail
{
    template <typename ...Args>
    struct ct_any_of_helper
    {
        std::tuple<Args...> values;

        constexpr ct_any_of_helper(Args... values) : values(std::move(values)...) { }

        template <typename T>
        [[nodiscard]] friend constexpr bool operator==(T lhs, ct_any_of_helper const& rhs) noexcept
        {
            return std::apply([&](auto... vals) { return ((lhs == vals) || ...); }, rhs.values);
        }
    };

    template <typename Container>
    struct rt_any_of_helper
    {
        Container const& values;

        constexpr rt_any_of_helper(Container const& values) : values(values) { }

        template <typename T>
        [[nodiscard]] friend constexpr bool operator==(T&& lhs, rt_any_of_helper&& rhs) noexcept
        {
            return std::any_of(cbegin(rhs.values), cend(rhs.values), [&](auto val)
                {
                    return lhs == val;
                });
        }
    };

    template <typename T>
    auto is_container(int) -> decltype(cbegin(std::declval<T>()) == cend(std::declval<T>()), std::true_type{});

    template <typename T>
    std::false_type is_container(...);

    template <typename T>
    constexpr bool is_container_v = decltype(is_container<T>(0))::value;
}

template <typename ...Args>
[[nodiscard]] consteval auto any_of(Args&&... values)
{
    using namespace detail;

    if constexpr (sizeof...(Args) == 1 && is_container_v<std::tuple_element_t<0, std::tuple<Args...>>>)
        return rt_any_of_helper(std::forward<Args>(values)...);
    else
        return ct_any_of_helper(std::forward<Args>(values)...);
}
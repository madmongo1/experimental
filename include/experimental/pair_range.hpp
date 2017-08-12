//
// Created by Richard Hodges on 12/08/2017.
//

#pragma once

namespace experimental {

    template<class A, class B>
    struct pair_range;


    template<class A> struct pair_range<A, A>
    {
        using pair_type = std::pair<A, A>;

        struct iterator
        {
            iterator(pair_type const& p, std::size_t index)
                    : data(std::addressof(p))
                    , index(index)
            {}

            iterator()
                    : data(nullptr)
                    , index(0)
            {}

            static auto get0(pair_type const& p) -> const A& { return std::get<0>(p); }
            static auto get1(pair_type const& p) -> const A& { return std::get<1>(p); }
            static auto get2(pair_type const& p) -> const A& { throw std::invalid_argument("off end"); }
            static constexpr auto make_getters()
            {
                return std::array<A const& (*)(pair_type const&), 3>
                                                                  {
                                                                          get0, get1, get2
                                                                  };
            }

            A const& operator*() const {
                return make_getters()[index](*data);
            }

            iterator& operator++() {
                ++index;
                return *this;
            }

            bool operator==(iterator const& other) const {
                return data == other.data && index == other.index;
            }

            bool operator!=(iterator const& other) const {
                return not (*this == other);
            }

        private:
            pair_type const* data;
            std::size_t index;
        };

        pair_range(pair_type const& p)
                : begin_(p, 0)
                , end_(p, 2)
        {}

        auto begin() const { return begin_; }
        auto end() const { return end_; }

        iterator begin_, end_;
    };

    template<class A>
    auto make_range(std::pair<A, A> const& pair)
    {
        return pair_range<A const&, A const&>(std::make_pair(std::cref(std::get<0>(pair)),
                                                             std::cref(std::get<1>(pair))));
    }

}
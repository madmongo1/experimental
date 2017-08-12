//
// Created by Richard Hodges on 12/08/2017.
//

#pragma once

#include <memory>
#include <functional>

namespace experimental {

    /** A function object which makes clones of objects
     *
     * @tparam T is the type of object which can be cloned
     * @tparam D The deleter used to destroy the cloned object
     */
    template<class T, class D>
    struct unique_cloner {
        /** The type of unique_ptr returned by the clone operation */
        using unique_ptr_type = std::unique_ptr<T, D>;

        /** The default unique_cloner implementation defers to an ADL function which is compatible with the
         * signature:
         * @code{.cpp}
         * unique_ptr_type clone(unique_ptr_type const&)
         * @endcode
         * @param source is the pointer to clone
         * @return a pointer to the clone
         * @note When a clone_ptr is copied, it will invoke this function only if the clone_ptr contains an object
         */
        unique_ptr_type operator()(unique_ptr_type const &source) const {
            return clone(source);
        }
    };

    /** A smart pointer which allows copying via the clone free function
     *
     * @tparam T is the type being pointed to
     * @tparam D is the type of deleter, which defaults to std::default_delete
     * @note a clone_ptr is designed to be, as far as possible, signature compatible with a unique_ptr
     * @see unique_ptr
     */
    template<class T, class D = std::default_delete<T>>
    struct clone_ptr : std::unique_ptr<T, D> {
        using unique_ptr_type = std::unique_ptr<T, D>;

        /** Defines a typedef compatible with that of unique_ptr */
        using pointer = typename unique_ptr_type::pointer;

        /** Defines a typedef compatible with that of unique_ptr */
        using element_type = typename unique_ptr_type::element_type;

        /** Defines a typedef compatible with that of unique_ptr */
        using deleter_type = typename unique_ptr_type::deleter_type;

        using unique_ptr_type::unique_ptr_type;

        /** Constuct from a compatible unique_ptr */
/*
        template<class U, class OtherDeleter>
        clone_ptr(std::unique_ptr<U, OtherDeleter> &&ptr)
                : ptr_(std::move(ptr)) {}
*/

        clone_ptr(clone_ptr &&other) = default;

        clone_ptr &operator=(clone_ptr &&other) = default;

        clone_ptr(clone_ptr const &other)
                : unique_ptr_type(other.make_clone()) {}

        clone_ptr &operator=(clone_ptr const &other) {
            auto tmp = clone_ptr(other);
            this->swap(tmp);
            return *this;
        }

    private:

        auto make_clone() const -> unique_ptr_type {
            unique_cloner<T, D> cloner;
            return cloner(*this);
        }
    };

    template<class T, class...Args>
    auto make_clone(Args&&...args)
    {
        return clone_ptr<T>(std::make_unique<T>(std::forward<Args>(args)...));
    };

    template<class T, class D>
    auto hash_value(clone_ptr<T, D> const &ptr) {
        auto hasher = std::hash<typename clone_ptr<T, D>::unique_ptr_type>();
        return hasher(ptr);
    };
}

namespace std {

    template<class T, class D>
    struct hash<experimental::clone_ptr<T, D>> {
        using argument_type = experimental::clone_ptr<T, D>;

        decltype(auto) operator()(argument_type const &arg) const {
            return hash_value(arg);
        }
    };
}

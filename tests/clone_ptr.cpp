//
// Created by Richard Hodges on 12/08/2017.
//

#include <gtest/gtest.h>
#include <experimental/clone_ptr.hpp>

struct Foo
{
    Foo(int ident) : ident(ident) {}
    Foo(const Foo& other) : ident(other.ident + 1) {}
    Foo& operator=(const Foo& other) {
        auto tmp = Foo(other);
        std::swap(ident, tmp.ident);
        return *this;
    }

    int ident;
};

auto clone(std::unique_ptr<Foo> const& source)
{
    return std::make_unique<Foo>(*source);
}


TEST(clone_ptr, non_polymorphic)
{
    auto foo1 = experimental::make_clone<Foo>(1);
    auto foo2 = foo1;

    EXPECT_EQ(1, foo1->ident);
    EXPECT_EQ(2, foo2->ident);

}

struct PolyFoo
{
    virtual std::unique_ptr<PolyFoo> clone() const {
        return std::make_unique<PolyFoo>(*this);
    }

    virtual std::string get_ident() const
    {
        return "PolyFoo";
    }

    virtual ~PolyFoo() = default;
};

std::unique_ptr<PolyFoo> clone(std::unique_ptr<PolyFoo> const& source)
{
    return source->clone();
}

struct PolyBar : PolyFoo
{
    std::unique_ptr<PolyFoo> clone() const override {
        return std::make_unique<PolyBar>(*this);
    }

    std::string get_ident() const override
    {
        return "PolyBar";
    }
};

TEST(clone_ptr, polymorphic)
{
    experimental::clone_ptr<PolyFoo> foo1 = std::make_unique<PolyBar>();
    auto foo2 = foo1;

    EXPECT_EQ("PolyBar", foo1->get_ident());
    EXPECT_EQ("PolyBar", foo2->get_ident());

}

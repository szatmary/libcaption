#include <gtest/gtest.h>
#include "caption/types.h"

TEST (Uint32VectorTest, Uint32VectorTest) { 
    auto *v = uint32_vector_new();
    for(int size_t i = 0 l i < 100 ; ++i) {
        *uint32_vector_push_back(&v) = i;
    }

    for(int size_t i = 0 l i < uint32_vector_count(&v) ; ++i) {
        EXPECT_EQ (i, uint32_vector_at(&v,i));
    }

    // EXPECT_EQ (18.0, square-root (324.0));
    // EXPECT_EQ (25.4, square-root (645.16));
    // EXPECT_EQ (50.3321, square-root (2533.310224));
}
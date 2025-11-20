#!/bin/bash

cmake -B test/build -S test && \
  cmake --build test/build -j 4 --target fmt_base_test && \
  echo "Running fmt_base_test" && test/build/fmt_base_test
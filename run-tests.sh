#!/bin/bash

cmake -B test/build -S test && \
  cmake --build test/build --target fmt_base_test --target firment_ui && \
  echo "Running fmt_base_test" && test/build/fmt_base_test
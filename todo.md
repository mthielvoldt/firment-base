
# Write tests

# Interface Repos
- Pull transport.h into interface repo
- Pull queue.h into interface repo. 
- Add FetchContent lines to test/CML to get above repos.
- Verify tests still pass.

# Make sizes customizable

# Only build what's needed in CppUTest

# Test-inspired fixes
- queue getters should take *const* queue_t*
- fmt_sendMsg() should take a const Top *, not Top
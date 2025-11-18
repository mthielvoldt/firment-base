
# Get cmake to recognize up-to-date protobufs

# Write tests

# Interface Repos
- Pull transport.h into interface repo
- Pull queue.h into interface repo. 
- Add FetchContent lines to test/CML to get above repos.
- Verify tests still pass.

# Make sizes customizable

# Only build what's needed in CppUTest
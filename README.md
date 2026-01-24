# Windows Dev Setup
1. Install (msys2)[msys2.org] for your architecture. 
2. in VSCode Ctrl + Shift + P -> Preferences: Open User Settins (JSON)
3. Integrate the following into settings.json: 
    ```json
    "terminal.integrated.profiles.windows": {
            "MSYS2 MinGW": {
                "path": "C:\\msys64\\usr\\bin\\bash.exe",
                "args": ["--login", "-i"],
                "env": {
                    "MSYSTEM": "MINGW64",
                    "CHERE_INVOKING": "1",
                    "MSYS2_PATH_TYPE": "inherit"
                },
                "icon": "terminal-bash"
            },
    },
    "terminal.integrated.defaultProfile.windows": "MSYS2 MinGW",
    ```
4. Launch a MINGW64 terminal and install:
    ```bash
    pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-gdb
    ```
5. Now you can run `cmake -B build -G Ninja`

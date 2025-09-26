# qt-video-info

## Build

```bash
# Configure (out-of-source)
cmake -S . -B build

# Build the app
cmake --build build -- -j4
```

## Run

```bash
./build/qt_video_info <args>
```

## Formatting

Format all sources with clang-format (if available on your PATH):

```bash
cmake --build build --target clang_format
```


## Cleaning

Remove all detectable CMake build directories under the repository (e.g., `build`, `cmake-build-*`, etc.):

```bash
cmake --build build --target clean_all
```

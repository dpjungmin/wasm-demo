- https://developer.mozilla.org/en-US/docs/WebAssembly
- https://emscripten.org/

- `emcc lib/canvas.cpp -s WASM=1 -s EXPORTED_FUNCTIONS='["_main", "_get_circles"]' -O2 -o public/canvas.js`

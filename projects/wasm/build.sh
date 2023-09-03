#build to cocos
em++ src/physic_wrapper.c   \
    -O0    \
    --no-entry  \
    --js-library src/physic_jslib.js   \
    -o cocos/assets/wasm/physic_wrapper.js     \
    -s WASM=1   \
    -s SAFE_HEAP=1  \
    -s WASM_BIGINT  \
    -s INITIAL_MEMORY=128kB  \
    -s ALLOW_MEMORY_GROWTH=1  \
    -s "EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"    \
    -s ENVIRONMENT='web' \
    -s MODULARIZE=1 \
    -s STRICT=1   

mv cocos/assets/wasm/physic_wrapper.wasm cocos/assets/wasm/physic_wrapper.bin


#build to web
    # em++ src/physic_wrapper.c   \
    # -O0    \
    # --no-entry  \
    # --js-library src/physic_jslib.js   \
    # -o test/wasm/physic_wrapper.js     \
    # -s WASM=1   \
    # -s SAFE_HEAP=1  \
    # -s WASM_BIGINT  \
    # -s INITIAL_MEMORY=128kB  \
    # -s ALLOW_MEMORY_GROWTH=1  \
    # -s "EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"    \
    # -s ENVIRONMENT='web' \
    # -s EXPORT_ES6=1 \
    # -s USE_ES6_IMPORT_META  \
    # -s MODULARIZE=1 \
    # -s STRICT=1   \
    # -s MODULARIZE=1 -
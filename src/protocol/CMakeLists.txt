# Build libnoiseprotocol.a

add_library(noiseprotocol STATIC)

# Include directories
set(GOLDILOCKS_SRCDIR ${PROJECT_SOURCE_DIR}/src/crypto/goldilocks/src)

target_include_directories(noiseprotocol PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/protocol>
    ${GOLDILOCKS_SRCDIR}/include
    ${GOLDILOCKS_SRCDIR}/p448
    ${GOLDILOCKS_SRCDIR}/p448/${ED448_ARCH}
)

# Warning flags
target_compile_options(noiseprotocol PRIVATE -Wall)

# Determine crypto backend
if(USE_LIBSODIUM)
    find_package(PkgConfig)
    pkg_search_module(LIBSODIUM libsodium)
    if(LIBSODIUM_FOUND)
        target_include_directories(noiseprotocol PRIVATE ${LIBSODIUM_INCLUDE_DIRS})
        target_link_libraries(noiseprotocol PUBLIC ${LIBSODIUM_LIBRARIES})
        target_compile_definitions(noiseprotocol PRIVATE USE_LIBSODIUM=1)
    endif()
endif()

if(USE_OPENSSL)
    find_package(PkgConfig)
    pkg_search_module(OPENSSL openssl)
    if(OPENSSL_FOUND)
        target_include_directories(noiseprotocol PRIVATE ${OPENSSL_INCLUDE_DIRS})
        target_link_libraries(noiseprotocol PUBLIC ${OPENSSL_LIBRARIES})
        target_compile_definitions(noiseprotocol PRIVATE USE_OPENSSL=1)
    endif()
endif()

# Core sources
set(noiseprotocol_sources
    cipherstate.c
    dhstate.c
    errors.c
    handshakestate.c
    hashstate.c
    internal.c
    names.c
    patterns.c
    randstate.c
    signstate.c
    symmetricstate.c
    util.c
    ../backend/ref/dh-curve448.c
    ../backend/ref/dh-newhope.c
    ../backend/ref/hash-blake2s.c
    ../crypto/blake2/blake2s.c
    ../crypto/curve448/curve448.c
    ../crypto/goldilocks/src/p448/${ED448_ARCH}/p448.c
    ../crypto/newhope/batcher.c
    ../crypto/newhope/error_correction.c
    ../crypto/newhope/fips202.c
    ../crypto/newhope/newhope.c
    ../crypto/newhope/ntt.c
    ../crypto/newhope/poly.c
    ../crypto/newhope/precomp.c
    ../crypto/newhope/reduce.c
)

if(USE_OPENSSL)
    list(APPEND noiseprotocol_sources
        ../backend/openssl/cipher-aesgcm.c)
else()
    if(USE_LIBSODIUM)
        list(APPEND noiseprotocol_sources
            ../backend/sodium/cipher-aesgcm.c
            ../backend/ref/cipher-aesgcm.c)
    else()
        list(APPEND noiseprotocol_sources
            ../backend/ref/cipher-aesgcm.c)
    endif()
endif()

if(USE_LIBSODIUM)
    list(APPEND noiseprotocol_sources
        rand_sodium.c
        ../backend/sodium/cipher-aesgcm.c
        ../backend/sodium/cipher-chachapoly.c
        ../backend/sodium/dh-curve25519.c
        ../backend/sodium/hash-blake2b.c
        ../backend/sodium/hash-sha256.c
        ../backend/sodium/hash-sha512.c
        ../backend/sodium/sign-ed25519.c
        ../crypto/aes/rijndael-alg-fst.c
        ../crypto/ghash/ghash.c)
else()
    list(APPEND noiseprotocol_sources
        rand_os.c
        ../backend/ref/cipher-aesgcm.c
        ../backend/ref/cipher-chachapoly.c
        ../backend/ref/dh-curve25519.c
        ../backend/ref/hash-blake2b.c
        ../backend/ref/hash-sha256.c
        ../backend/ref/hash-sha512.c
        ../backend/ref/sign-ed25519.c
        ../crypto/aes/rijndael-alg-fst.c
        ../crypto/blake2/blake2b.c
        ../crypto/chacha/chacha.c
        ../crypto/donna/poly1305-donna.c
        ../crypto/ghash/ghash.c
        ../crypto/newhope/crypto_stream_chacha20.c
        ../crypto/sha2/sha256.c
        ../crypto/sha2/sha512.c
        ../crypto/ed25519/ed25519.c)
endif()

target_sources(noiseprotocol PRIVATE ${noiseprotocol_sources})

# ---- ed25519-donna needs its own flags if OpenSSL is disabled ----
if(NOT USE_OPENSSL)
    target_compile_definitions(noiseprotocol PRIVATE
        ED25519_REFHASH          # use the built-in slow SHA-512
        ED25519_CUSTOMRANDOM)    # use Noise-C's rand_os.c
endif()

set_target_properties(noiseprotocol PROPERTIES OUTPUT_NAME noiseprotocol)
install(TARGETS noiseprotocol ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

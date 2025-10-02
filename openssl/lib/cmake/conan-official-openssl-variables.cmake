set(OPENSSL_FOUND TRUE)
if(DEFINED OpenSSL_INCLUDE_DIR)
    set(OPENSSL_INCLUDE_DIR ${OpenSSL_INCLUDE_DIR})
endif()
if(DEFINED OpenSSL_Crypto_LIBS)
    set(OPENSSL_CRYPTO_LIBRARY ${OpenSSL_Crypto_LIBS})
    set(OPENSSL_CRYPTO_LIBRARIES ${OpenSSL_Crypto_LIBS}
                                 ${OpenSSL_Crypto_DEPENDENCIES}
                                 ${OpenSSL_Crypto_FRAMEWORKS}
                                 ${OpenSSL_Crypto_SYSTEM_LIBS})
elseif(DEFINED openssl_OpenSSL_Crypto_LIBS_RELEASE)
    set(OPENSSL_CRYPTO_LIBRARY ${openssl_OpenSSL_Crypto_LIBS_RELEASE})
    set(OPENSSL_CRYPTO_LIBRARIES ${openssl_OpenSSL_Crypto_LIBS_RELEASE}
                                 ${openssl_OpenSSL_Crypto_DEPENDENCIES_RELEASE}
                                 ${openssl_OpenSSL_Crypto_FRAMEWORKS_RELEASE}
                                 ${openssl_OpenSSL_Crypto_SYSTEM_LIBS_RELEASE})
endif()
if(DEFINED OpenSSL_SSL_LIBS)
    set(OPENSSL_SSL_LIBRARY ${OpenSSL_SSL_LIBS})
    set(OPENSSL_SSL_LIBRARIES ${OpenSSL_SSL_LIBS}
                              ${OpenSSL_SSL_DEPENDENCIES}
                              ${OpenSSL_SSL_FRAMEWORKS}
                              ${OpenSSL_SSL_SYSTEM_LIBS})
elseif(DEFINED openssl_OpenSSL_SSL_LIBS_RELEASE)
    set(OPENSSL_SSL_LIBRARY ${openssl_OpenSSL_SSL_LIBS_RELEASE})
    set(OPENSSL_SSL_LIBRARIES ${openssl_OpenSSL_SSL_LIBS_RELEASE}
                              ${openssl_OpenSSL_SSL_DEPENDENCIES_RELEASE}
                              ${openssl_OpenSSL_SSL_FRAMEWORKS_RELEASE}
                              ${openssl_OpenSSL_SSL_SYSTEM_LIBS_RELEASE})
endif()
if(DEFINED OpenSSL_LIBRARIES)
    set(OPENSSL_LIBRARIES ${OpenSSL_LIBRARIES})
endif()
if(DEFINED OpenSSL_VERSION)
    set(OPENSSL_VERSION ${OpenSSL_VERSION})
endif()

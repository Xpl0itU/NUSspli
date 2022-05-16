# build wut
FROM devkitpro/devkitppc:20220216 AS final

ENV openssl_ver=3.0.3 \
 curl_ver=7.83.1 \
 DEBIAN_FRONTEND=noninteractive \
 PATH=$DEVKITPPC/bin:$PATH \
 WUT_ROOT=$DEVKITPRO/wut
WORKDIR /

RUN mkdir -p /usr/share/man/man1 /usr/share/man/man2 && \
 apt-get update && \
 apt-get -y --no-install-recommends upgrade && \
 apt-get -y install --no-install-recommends autoconf automake libtool openjdk-11-jre-headless && \
 apt-get clean && \
 rm -rf /var/lib/apt/lists/* /usr/share/man && \
 git clone -b customheap_spinlock --single-branch https://github.com/GaryOderNichts/wut && \
 cd wut && \
 git checkout ec0d038f447c4d35db359d6ed8baeb4247f6caf1 && \
 make -j$(nproc) && \
 make install && \
 cd .. && \
 rm -rf wut && \
 wget https://www.openssl.org/source/openssl-$openssl_ver.tar.gz && \
 mkdir /openssl && \
 tar xf openssl-$openssl_ver.tar.gz -C /openssl --strip-components=1 && \
 rm -f openssl-$openssl_ver.tar.gz && \
 cd openssl && \
 echo 'diff --git a/Configurations/10-main.conf b/Configurations/10-main.conf\n\
index 61c6689..efe686a 100644\n\
--- a/Configurations/10-main.conf\n\
+++ b/Configurations/10-main.conf\n\
@@ -627,6 +627,27 @@ my %targets = (\n\
         shared_extension => ".so",\n\
     },\n\
 \n\
+### Wii U target\n\
+    "wiiu" => {\n\
+        inherit_from     => [ "BASE_unix" ],\n\
+        CC               => "$ENV{DEVKITPPC}/bin/powerpc-eabi-gcc",\n\
+        CXX              => "$ENV{DEVKITPPC}/bin/powerpc-eabi-g++",\n\
+        AR               => "$ENV{DEVKITPPC}/bin/powerpc-eabi-ar",\n\
+        CFLAGS           => picker(default => "-Wall",\n\
+                                   debug   => "-O0 -g",\n\
+                                   release => "-Ofast -pipe"),\n\
+        CXXFLAGS         => picker(default => "-Wall",\n\
+                                   debug   => "-O0 -g",\n\
+                                   release => "-Ofast -pipe"),\n\
+        LDFLAGS          => "-L$ENV{DEVKITPRO}/wut/lib",\n\
+        cflags           => add("-mcpu=750 -meabi -mhard-float -ffunction-sections -fdata-sections"),\n\
+        cxxflags         => add("-std=c++11"),\n\
+        lib_cppflags     => "-DOPENSSL_USE_NODELETE -DB_ENDIAN -DNO_SYS_UN_H -DNO_SYSLOG -D__WIIU__ -D__WUT__ -I$ENV{DEVKITPRO}/wut/include",\n\
+        ex_libs          => add("-lwut -lm"),\n\
+        bn_ops           => "BN_LLONG RC4_CHAR",\n\
+        asm_arch         => '"'"'ppc32'"'"',\n\
+    },\n\
+\n ####\n #### Variety of LINUX:-)\n ####\n\
diff --git a/crypto/uid.c b/crypto/uid.c\n\
index a9eae36..4a81d98 100644\n\
--- a/crypto/uid.c\n\
+++ b/crypto/uid.c\n\
@@ -10,7 +10,7 @@\n #include <openssl/crypto.h>\n #include <openssl/opensslconf.h>\n\
 \n\
-#if defined(OPENSSL_SYS_WIN32) || defined(OPENSSL_SYS_VXWORKS) || defined(OPENSSL_SYS_UEFI)\n\
+#if defined(OPENSSL_SYS_WIN32) || defined(OPENSSL_SYS_VXWORKS) || defined(OPENSSL_SYS_UEFI) || defined(__WIIU__)\n\
 \n\
 int OPENSSL_issetugid(void)\n\
 {\
' >> wiiu.patch && git apply wiiu.patch && \
 ./Configure wiiu \
  no-threads no-shared no-asm no-ui-console no-unit-test no-tests no-buildtest-c++ no-external-tests no-autoload-config \
  --with-rand-seed=none -static --prefix=$DEVKITPRO/portlibs/wiiu --openssldir=openssldir && \
 make build_generated && make libssl.a libcrypto.a -j$(nproc) && \
 cp lib*.a $DEVKITPRO/portlibs/wiiu/lib/ && \
 cp -r include/openssl $DEVKITPRO/portlibs/wiiu/include/openssl && \
 cd .. && \
 rm -rf openssl && \
 wget https://curl.se/download/curl-$curl_ver.tar.gz && \
 mkdir /curl && \
 tar xf curl-$curl_ver.tar.gz -C /curl --strip-components=1 && \
 rm -f curl-$curl_ver.tar.gz && \
 cd curl && \
 autoreconf -fi && ./configure \
--prefix=$DEVKITPRO/portlibs/wiiu/ \
--host=powerpc-eabi \
--enable-static \
--disable-threaded-resolver \
--disable-pthreads \
--with-ssl=$DEVKITPRO/portlibs/wiiu/ \
--disable-ipv6 \
--disable-unix-sockets \
--disable-socketpair \
--disable-ntlm-wb \
CFLAGS="-mcpu=750 -meabi -mhard-float -O3 -ffunction-sections -fdata-sections" \
CXXFLAGS="-mcpu=750 -meabi -mhard-float -O3 -ffunction-sections -fdata-sections" \
CPPFLAGS="-D__WIIU__ -D__WUT__ -I$DEVKITPRO/wut/include" \
LDFLAGS="-L$DEVKITPRO/wut/lib" \
LIBS="-lwut -lm" \
CC=$DEVKITPPC/bin/powerpc-eabi-gcc \
AR=$DEVKITPPC/bin/powerpc-eabi-ar \
RANLIB=$DEVKITPPC/bin/powerpc-eabi-ranlib \
PKG_CONFIG=$DEVKITPRO/portlibs/wiiu/bin/powerpc-eabi-pkg-config && \
 cd lib && \
 make -j$(nproc) install && \
 cd ../include && \
 make -j$(nproc) install && \
 cd ../.. && \
 rm -rf curl && \
 git clone --recursive https://github.com/Crementif/libiosuhax && \
 cd libiosuhax && \
 make -j$(nproc) && \
 make install && \
 cd .. && \
 rm -rf libiosuhax && \
 git clone --recursive https://github.com/yawut/libromfs-wiiu && \
 cd libromfs-wiiu && \
 make -j$(nproc) && \
 make install && \
 cd .. && \
 rm -rf libromfs-wiiu && \
 mkdir /nuspacker && \
 cd /nuspacker && \
 wget https://github.com/Maschell/nuspacker/raw/master/NUSPacker.jar

WORKDIR /project

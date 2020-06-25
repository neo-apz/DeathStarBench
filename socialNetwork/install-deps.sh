apt-get update
export BUILD_DEPS="ca-certificates g++ cmake wget git automake bison flex libboost-all-dev libevent-dev libssl-dev libtool make pkg-config"
apt-get install -y ${BUILD_DEPS} --no-install-recommends

export LIB_THRIFT_VERSION=0.12.0
wget -O thrift-${LIB_THRIFT_VERSION}.tar.gz https://github.com/apache/thrift/archive/v${LIB_THRIFT_VERSION}.tar.gz
tar -zxf ${LIB_THRIFT_VERSION}.tar.gz
cd ${LIB_THRIFT_VERSION}
mkdir -p cmake_build
cd cmake_build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=0 ..
make -j
make install
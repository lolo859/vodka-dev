#!/bin/bash
set -e
set -u
./comp.sh library
NAME="vodka-lib-beta"
VERSION="0.5_beta_1"
RELEASE="1"
ARCH="x86_64"
LIB_FILE="libvodka.a"
HEADER_FILE="vodka-lib/vodka-lib.h"
mkdir -p rpm/sources
cp "$LIB_FILE" rpm/sources/
cp "$HEADER_FILE" rpm/sources/
TARBALL="$NAME-$VERSION.tar.gz"
tar -czf "rpm/sources/$TARBALL" -C rpm/sources "$(basename $LIB_FILE)" "$(basename $HEADER_FILE)"
rpmbuild -bs \
    --define "_topdir ./rpm" \
    --define "_sourcedir ./rpm/sources" \
    --define "_specdir ./rpm/spec" \
    --define "_builddir ./rpm/build" \
    --define "_rpmdir ./rpm/rpm-beta" \
    --define "_srpmdir ./rpm/rpm-beta" \
    rpm/spec/vodka-lib-beta.spec
./comp.sh clean
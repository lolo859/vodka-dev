#!/bin/bash
set -e
set -u
NAME_LIB="vodka-lib-beta"
NAME_EXE="vodka-beta"
VERSION="0.5_beta_1"
RELEASE="1"
ARCH="x86_64"
LIB_DIR="vodka-lib/"
COMP_FILE="comp.sh"
DEPS_DIR="dependencies/"
MAIN_PROGRAM="./vodka.cpp"
mkdir -p rpm/sources
TARBALL="$NAME_LIB.tar"
tar -cf "rpm/sources/$TARBALL" "$LIB_DIR"
tar -rf "rpm/sources/$TARBALL" "$COMP_FILE"
tar -rf "rpm/sources/$TARBALL" "$DEPS_DIR"
gzip -f "rpm/sources/$TARBALL"
rpmbuild -bs \
    --define "_topdir ./rpm" \
    --define "_sourcedir ./rpm/sources" \
    --define "_specdir ./rpm/spec" \
    --define "_builddir ./rpm/build" \
    --define "_rpmdir ./rpm/rpm-beta" \
    --define "_srpmdir ./rpm/rpm-beta" \
    rpm/spec/vodka-lib-beta.spec
cp "rpm/SRPMS/vodka-lib-beta-$VERSION-$RELEASE.fc42.src.rpm" "rpm/rpm-beta" 
TARBALL="$NAME_EXE.tar"
tar -cf "rpm/sources/$TARBALL" "$LIB_DIR"
tar -rf "rpm/sources/$TARBALL" "$MAIN_PROGRAM"
tar -rf "rpm/sources/$TARBALL" "$COMP_FILE"
tar -rf "rpm/sources/$TARBALL" "$DEPS_DIR"
gzip -f "rpm/sources/$TARBALL"
rpmbuild -bs \
    --define "_topdir ./rpm" \
    --define "_sourcedir ./rpm/sources" \
    --define "_specdir ./rpm/spec" \
    --define "_builddir ./rpm/build" \
    --define "_rpmdir ./rpm/rpm-beta" \
    --define "_srpmdir ./rpm/rpm-beta" \
    rpm/spec/vodka-beta.spec
cp "rpm/SRPMS/vodka-beta-$VERSION-$RELEASE.fc42.src.rpm" "rpm/rpm-beta" 
Name:           vodka-lib-beta
Version:        0.5_beta_1
Release:        1%{?dist}
Summary:        Static library and headers for vodka-lib (beta)

License:        GPL-3.0
URL:            https://github.com/lolo859/vodka
Source0:        vodka-lib-beta.tar.gz

BuildArch:      x86_64
BuildRequires:  tree g++

%description
Vodka static library (libvodka.a) and public headers (vodka-lib.h) for beta usage.

%prep
tree
ls -R
cp /builddir/build/SOURCES/vodka-lib-beta.tar.gz ./
mkdir -p vodka-lib-beta
tar -xzf vodka-lib-beta.tar.gz -C vodka-lib-beta
rm vodka-lib-beta.tar.gz
cd vodka-lib-beta

%build
cd vodka-lib-beta
./comp.sh library

%install
mkdir -p %{buildroot}/usr/include/vodka
mkdir -p %{buildroot}/usr/lib
cp vodka-lib-beta/libvodka.a %{buildroot}/usr/lib
cp vodka-lib-beta/vodka-lib/vodka-lib.h %{buildroot}/usr/include/vodka

%files
/usr/lib/libvodka.a
/usr/include/vodka/vodka-lib.h

%changelog
* Thu Sep 04 2025 lolo859 <fialairem@gmail.com> - 0.5 beta 1
- Initial beta release of vodka-lib
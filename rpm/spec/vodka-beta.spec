Name:           vodka-beta
Version:        0.5_beta_1
Release:        1%{?dist}
Summary:        Vodka Transcoder statically compiled for beta usage.

License:        GPL-3.0
URL:            https://github.com/lolo859/vodka
Source0:        vodka-beta.tar.gz

BuildArch:      x86_64
BuildRequires:  tree g++

%description
Vodka Transcoder statically compiled for beta usage.

%prep
tree
ls -R
cp /builddir/build/SOURCES/vodka-beta.tar.gz ./
mkdir -p vodka-beta
tar -xzf vodka-beta.tar.gz -C vodka-beta
rm vodka-beta.tar.gz
cd vodka-beta

%build
tree ../..
cd vodka-beta
./comp.sh

%install
mkdir -p %{buildroot}/usr/local/bin
mv vodka-beta/vodka %{buildroot}/usr/local/bin/vodka-beta

%files
/usr/local/bin/vodka-beta

%changelog
* Thu Sep 04 2025 lolo859 <fialairem@gmail.com> - 0.5 beta 1
- Initial beta release of vodka
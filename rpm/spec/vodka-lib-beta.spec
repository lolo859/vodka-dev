Name:           vodka-lib-beta
Version:        0.5_beta_1
Release:        1%{?dist}
Summary:        Static library and headers for vodka-lib (beta)

License:        GPL-3.0
URL:            https://github.com/lolo859/vodka
Source0:        %{name}-%{version}.tar.gz

BuildArch:      x86_64
BuildRoot:      %{_tmppath}/%{name}-%{version}-root
BuildRequires:  tree 

%description
Vodka static library (libvodka.a) and public headers (vodka-lib.h) for beta usage.

%setup -c -T
# Copier les fichiers sources dans le tarball SOURCES
tree
ls -R
cp /chemin/vers/libvodka.a .
cp /chemin/vers/vodka-lib.h .

%build
# Pas de compilation, le .a est déjà présent
# Tu pourrais éventuellement construire libvodka.a ici si tu veux automatiser

%install
tree
ls -R
mkdir -p %{buildroot}/usr/lib
mkdir -p %{buildroot}/usr/include/vodka
cp libvodka.a %{buildroot}/usr/lib/
cp vodka-lib.h %{buildroot}/usr/include/vodka/

%files
/usr/lib/libvodka.a
/usr/include/vodka/vodka-lib.h

%changelog
* Thu Sep 02 2025 lolo859 <fialairem@gmail.com> - 0.5 beta 1
- Initial beta release of vodka-lib
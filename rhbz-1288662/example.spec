Name:		example
Version:	0.0
Release:	1%{?dist}
Summary:	Nothing important here

License:	GPL
URL:		http://example.com
Source0:	main.c

BuildRequires:	gcc

%description
Nothing important here.


%prep

%build

gcc -g3 -O0 -o prog %{SOURCE0}
./prog


%install
mkdir -p %{buildroot}/%{_bindir}
cp prog %{buildroot}/%{_bindir}/my-prog


%files
%_bindir/my-prog


%changelog

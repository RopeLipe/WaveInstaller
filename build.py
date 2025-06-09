#!/usr/bin/env python3
"""
Build script for Wave Installer
Creates various distribution packages
"""

import os
import sys
import subprocess
import shutil
from pathlib import Path

def run_command(cmd, cwd=None):
    """Run shell command and handle errors"""
    print(f"Running: {cmd}")
    try:
        result = subprocess.run(cmd, shell=True, check=True, cwd=cwd, 
                              capture_output=True, text=True)
        print(result.stdout)
        return result
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        print(f"Output: {e.stdout}")
        print(f"Error: {e.stderr}")
        return None

def build_pyinstaller():
    """Build using PyInstaller - creates a standalone executable"""
    print("\n=== Building with PyInstaller ===")
    
    # Install PyInstaller if not present
    run_command("pip3 install pyinstaller")
    
    # Create PyInstaller spec file for better control
    spec_content = '''
# -*- mode: python ; coding: utf-8 -*-

block_cipher = None

a = Analysis(
    ['main.py'],
    pathex=[],
    binaries=[],
    datas=[
        ('assets', 'assets'),
        ('screens', 'screens'),
    ],    hiddenimports=[
        'gi',
        'gi.repository',
        'gi.repository.Gtk',
        'gi.repository.Gdk',
        'gi.repository.Adw',
        'gi.repository.GLib',
        'gi.repository.Gio',
        'gi.repository.GdkPixbuf',
    ],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)

pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.zipfiles,
    a.datas,
    [],
    name='wave-installer',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)
'''
    
    with open('wave-installer.spec', 'w') as f:
        f.write(spec_content)
    
    # Build the executable
    result = run_command("pyinstaller wave-installer.spec --clean --noconfirm")
    
    if result:
        print("✅ PyInstaller build complete! Executable: dist/wave-installer")
        return True
    return False

def build_appimage():
    """Build AppImage - portable Linux application format"""
    print("\n=== Building AppImage ===")
    
    # Create AppDir structure
    appdir = "WaveInstaller.AppDir"
    os.makedirs(appdir, exist_ok=True)
    os.makedirs(f"{appdir}/usr/bin", exist_ok=True)
    os.makedirs(f"{appdir}/usr/share/applications", exist_ok=True)
    os.makedirs(f"{appdir}/usr/share/icons/hicolor/256x256/apps", exist_ok=True)
    
    # Copy application files
    shutil.copy("main.py", f"{appdir}/usr/bin/wave-installer")
    shutil.copytree("screens", f"{appdir}/usr/bin/screens", dirs_exist_ok=True)
    shutil.copytree("assets", f"{appdir}/usr/bin/assets", dirs_exist_ok=True)
    
    # Create AppRun script
    apprun_content = '''#!/bin/bash
SELF=$(readlink -f "$0")
HERE=${SELF%/*}
export PATH="${HERE}/usr/bin/:${PATH}"
export LD_LIBRARY_PATH="${HERE}/usr/lib/:${LD_LIBRARY_PATH}"
cd "${HERE}/usr/bin"
exec python3 wave-installer "$@"
'''
    
    with open(f"{appdir}/AppRun", 'w') as f:
        f.write(apprun_content)
    os.chmod(f"{appdir}/AppRun", 0o755)
    
    # Create desktop file
    desktop_content = '''[Desktop Entry]
Type=Application
Name=Wave Installer
Comment=Modern Linux System Installer
Exec=wave-installer
Icon=wave-installer
Terminal=false
Categories=System;
StartupNotify=true
'''
    
    with open(f"{appdir}/wave-installer.desktop", 'w') as f:
        f.write(desktop_content)
    
    # Create symbolic links
    os.symlink("wave-installer.desktop", f"{appdir}/usr/share/applications/wave-installer.desktop")
    
    # Download appimagetool if not present
    if not os.path.exists("appimagetool-x86_64.AppImage"):
        print("Downloading appimagetool...")
        run_command("wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage")
        os.chmod("appimagetool-x86_64.AppImage", 0o755)
    
    # Build AppImage
    result = run_command(f"./appimagetool-x86_64.AppImage {appdir}")
    
    if result:
        print("✅ AppImage build complete! File: WaveInstaller-x86_64.AppImage")
        return True
    return False

def build_deb_package():
    """Build DEB package for Debian/Ubuntu"""
    print("\n=== Building DEB Package ===")
    
    # Create package structure
    pkg_dir = "wave-installer_1.0.0_all"
    os.makedirs(f"{pkg_dir}/DEBIAN", exist_ok=True)
    os.makedirs(f"{pkg_dir}/usr/bin", exist_ok=True)
    os.makedirs(f"{pkg_dir}/usr/share/applications", exist_ok=True)
    os.makedirs(f"{pkg_dir}/usr/share/wave-installer", exist_ok=True)
    os.makedirs(f"{pkg_dir}/usr/share/icons/hicolor/48x48/apps", exist_ok=True)
    
    # Create control file
    control_content = '''Package: wave-installer
Version: 1.0.0
Section: admin
Priority: optional
Architecture: all
Depends: python3-gi, gir1.2-gtk-4.0, gir1.2-adw-1, python3-gi-cairo
Maintainer: Wave OS Team <team@waveos.org>
Description: Modern Linux System Installer
 A beautiful, modern installer for Linux distributions with GTK4 interface.
 Features include guided setup, theme support, and user-friendly design.
'''
    
    with open(f"{pkg_dir}/DEBIAN/control", 'w') as f:
        f.write(control_content)
    
    # Create launcher script
    launcher_content = '''#!/bin/bash
cd /usr/share/wave-installer
exec python3 main.py "$@"
'''
    
    with open(f"{pkg_dir}/usr/bin/wave-installer", 'w') as f:
        f.write(launcher_content)
    os.chmod(f"{pkg_dir}/usr/bin/wave-installer", 0o755)
    
    # Copy application files
    shutil.copy("main.py", f"{pkg_dir}/usr/share/wave-installer/")
    shutil.copytree("screens", f"{pkg_dir}/usr/share/wave-installer/screens", dirs_exist_ok=True)
    shutil.copytree("assets", f"{pkg_dir}/usr/share/wave-installer/assets", dirs_exist_ok=True)
    
    # Create desktop file
    desktop_content = '''[Desktop Entry]
Type=Application
Name=Wave Installer
Comment=Modern Linux System Installer
Exec=wave-installer
Icon=wave-installer
Terminal=false
Categories=System;Settings;
StartupNotify=true
Keywords=installer;system;setup;
'''
    
    with open(f"{pkg_dir}/usr/share/applications/wave-installer.desktop", 'w') as f:
        f.write(desktop_content)
    
    # Build DEB package
    result = run_command(f"dpkg-deb --build {pkg_dir}")
    
    if result:
        print("✅ DEB package build complete! File: wave-installer_1.0.0_all.deb")
        return True
    return False

def build_rpm_package():
    """Build RPM package for Fedora/RHEL"""
    print("\n=== Building RPM Package ===")
    
    # Create RPM build structure
    rpm_dirs = ["BUILD", "RPMS", "SOURCES", "SPECS", "SRPMS"]
    for d in rpm_dirs:
        os.makedirs(f"rpmbuild/{d}", exist_ok=True)
    
    # Create source tarball
    run_command("tar -czf rpmbuild/SOURCES/wave-installer-1.0.0.tar.gz --exclude=rpmbuild --exclude=.git .")
    
    # Create RPM spec file
    spec_content = '''Name:           wave-installer
Version:        1.0.0
Release:        1%{?dist}
Summary:        Modern Linux System Installer
License:        GPL-3.0+
URL:            https://github.com/waveos/installer
Source0:        %{name}-%{version}.tar.gz
BuildArch:      noarch

Requires:       python3-gobject
Requires:       gtk4
Requires:       libadwaita

%description
A beautiful, modern installer for Linux distributions with GTK4 interface.
Features include guided setup, theme support, and user-friendly design.

%prep
%setup -q

%build
# Nothing to build

%install
mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_datadir}/%{name}
mkdir -p %{buildroot}%{_datadir}/applications

# Install application files
cp main.py %{buildroot}%{_datadir}/%{name}/
cp -r screens %{buildroot}%{_datadir}/%{name}/
cp -r assets %{buildroot}%{_datadir}/%{name}/

# Create launcher script
cat > %{buildroot}%{_bindir}/%{name} << 'EOF'
#!/bin/bash
cd %{_datadir}/%{name}
exec python3 main.py "$@"
EOF
chmod +x %{buildroot}%{_bindir}/%{name}

# Install desktop file
cat > %{buildroot}%{_datadir}/applications/%{name}.desktop << 'EOF'
[Desktop Entry]
Type=Application
Name=Wave Installer
Comment=Modern Linux System Installer
Exec=wave-installer
Icon=wave-installer
Terminal=false
Categories=System;Settings;
StartupNotify=true
EOF

%files
%{_bindir}/%{name}
%{_datadir}/%{name}/
%{_datadir}/applications/%{name}.desktop

%changelog
* Sun Jun 08 2025 Wave OS Team <team@waveos.org> - 1.0.0-1
- Initial release
'''
    
    with open("rpmbuild/SPECS/wave-installer.spec", 'w') as f:
        f.write(spec_content)
    
    # Build RPM
    result = run_command("rpmbuild --define '_topdir $(pwd)/rpmbuild' -ba rpmbuild/SPECS/wave-installer.spec")
    
    if result:
        print("✅ RPM package build complete! Check rpmbuild/RPMS/ directory")
        return True
    return False

def build_flatpak():
    """Build Flatpak package"""
    print("\n=== Building Flatpak ===")
    
    # Create Flatpak manifest
    manifest_content = '''{
    "app-id": "org.waveos.Installer",
    "runtime": "org.gnome.Platform",
    "runtime-version": "45",
    "sdk": "org.gnome.Sdk",
    "command": "wave-installer",
    "finish-args": [
        "--share=ipc",
        "--socket=fallback-x11",
        "--socket=wayland",
        "--device=dri",
        "--filesystem=host"
    ],
    "modules": [
        {
            "name": "wave-installer",
            "buildsystem": "simple",
            "build-commands": [
                "mkdir -p /app/bin",
                "mkdir -p /app/share/wave-installer",
                "cp main.py /app/share/wave-installer/",
                "cp -r screens /app/share/wave-installer/",
                "cp -r assets /app/share/wave-installer/",
                "echo '#!/bin/bash\\ncd /app/share/wave-installer\\nexec python3 main.py \"$@\"' > /app/bin/wave-installer",
                "chmod +x /app/bin/wave-installer"
            ],
            "sources": [
                {
                    "type": "dir",
                    "path": "."
                }
            ]
        }
    ]
}'''
    
    with open("org.waveos.Installer.json", 'w') as f:
        f.write(manifest_content)
    
    print("Flatpak manifest created: org.waveos.Installer.json")
    print("To build Flatpak, run:")
    print("flatpak-builder build-dir org.waveos.Installer.json --force-clean")
    print("flatpak build-export repo build-dir")
    
    return True

def main():
    """Main build function"""
    if len(sys.argv) < 2:
        print("Usage: python3 build.py [pyinstaller|appimage|deb|rpm|flatpak|all]")
        sys.exit(1)
    
    build_type = sys.argv[1].lower()
    
    # Change to script directory
    os.chdir(Path(__file__).parent)
    
    success = False
    
    if build_type == "pyinstaller":
        success = build_pyinstaller()
    elif build_type == "appimage":
        success = build_appimage()
    elif build_type == "deb":
        success = build_deb_package()
    elif build_type == "rpm":
        success = build_rpm_package()
    elif build_type == "flatpak":
        success = build_flatpak()
    elif build_type == "all":
        print("Building all package types...")
        results = [
            build_pyinstaller(),
            build_appimage(),
            build_deb_package(),
            build_rpm_package(),
            build_flatpak()
        ]
        success = any(results)
    else:
        print(f"Unknown build type: {build_type}")
        sys.exit(1)
    
    if success:
        print(f"\n✅ Build completed successfully!")
    else:
        print(f"\n❌ Build failed!")
        sys.exit(1)

if __name__ == "__main__":
    main()

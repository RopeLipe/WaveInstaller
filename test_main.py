#!/usr/bin/env python3
"""
Simple test for the main WaveInstaller application
"""

import sys
import os

# Add the current directory to Python path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

try:
    print("Testing WaveInstaller import...")
    from main import WaveInstaller
    print("✓ WaveInstaller imported successfully")
    
    print("Creating application instance...")
    app = WaveInstaller()
    print("✓ Application created successfully")
    
    print("Running application...")
    exit_code = app.run(sys.argv)
    print(f"Application exited with code: {exit_code}")
    
except ImportError as e:
    print(f"✗ Import error: {e}")
    import traceback
    traceback.print_exc()
except Exception as e:
    print(f"✗ Runtime error: {e}")
    import traceback
    traceback.print_exc()

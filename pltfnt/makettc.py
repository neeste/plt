#!/usr/bin/env python3
"""makettc - regenerate the ttc/ TrueType collections from the .txt fonts.

Runs pltfnt2ttf.py over the hand-made text fonts (the 64-unit-grid
designs original to plt).  The Hershey-derived fonts (21-unit grid,
built from hersh1.dat via the .mph maps) and the symbol fonts (ftyp 2)
are not converted.

usage: makettc.py
"""

import subprocess
import sys

FAMILIES = {
    "stdfnt": "PLT Standard",      "stdfntii": "PLT Standard II",
    "stdbis": "PLT Standard Bis",  "altfnt": "PLT Alternate",
    "altbis": "PLT Alternate Bis", "oldstd": "PLT Old Standard",
    "oldalt": "PLT Old Alternate", "ipa": "PLT IPA",
}


def main():
    fail = 0
    for base, family in sorted(FAMILIES.items()):
        cmd = [sys.executable, "pltfnt2ttf.py", base + ".txt",
               "-o", "ttc/%s.ttc" % base, "--name", family]
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode:
            fail += 1
            print("FAILED %s:\n%s" % (base, r.stderr))
        else:
            print(r.stdout.strip())
    sys.exit(1 if fail else 0)


if __name__ == "__main__":
    main()

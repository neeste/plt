#!/usr/bin/env python3
"""pltfnt2ttf - convert a plt stroke-font .txt definition to TrueType.

The .txt format (see pltfnt.c) defines each glyph as pen strokes:

    { code lft adv     start glyph for ASCII `code`; draw origin offset
                       `lft`, advance width `adv`
    m x y              move pen to (x, y)
    d x y              draw straight line to (x, y)
    a x y deg          draw arc to (x, y) sweeping `deg` degrees
                       (positive = counterclockwise)
    c x y              draw full circle; current point and (x, y) are
                       diametrically opposite; pen returns to its
                       pre-circle position (see getarc.c)
    }                  end glyph

Coordinates sit on a grid where the character height is fchh units (64
for the hand-made fonts, 21 for the Hershey-derived ones) and the
baseline is y = 0.  Strokes are expanded to filled outlines with round
caps and joins - the look of a plotter pen.

Four style-linked faces are generated - Regular, Italic, Bold, and
Bold Italic - and written as a single .ttc collection, or as separate
.ttf files with --ttf.  Bold uses the same strokes with a wider pen;
Italic shears the strokes 10 degrees about the baseline before the
pen-width expansion, as the original renderer did.

usage: pltfnt2ttf.py input.txt [-o output] [--name "Family Name"]
                     [--stroke UNITS] [--bold-stroke UNITS] [--ttf]
"""

import argparse
import math
import os
import re
import sys

from fontTools import agl
from fontTools.fontBuilder import FontBuilder
from fontTools.pens.ttGlyphPen import TTGlyphPen
from fontTools.ttLib import TTCollection
from shapely.geometry import LineString, MultiLineString
from shapely.geometry.polygon import orient
from shapely.ops import unary_union

UPM = 1000
CHH_UNITS = 640     # font units per fchh (header character height)
ARC_STEP_DEG = 4    # arc sampling resolution before stroking
SCALE = 10          # set per font: CHH_UNITS / fchh
ITALIC_DEG = 10     # slant of the synthetic italic faces
ITALIC_SHEAR = math.tan(math.radians(ITALIC_DEG))


def parse_font(path):
    """Return (header, glyphs) where glyphs maps code -> (lft, adv, polylines).

    Each polyline is a list of (x, y) points in grid units; circles are
    returned as closed polylines.
    """
    header = {}
    glyphs = {}
    cur = None          # (code, lft, adv, polylines)
    pen = (0.0, 0.0)
    poly = None         # polyline being built

    def flush():
        nonlocal poly
        if poly is not None and len(poly) > 1:
            cur[3].append(poly)
        poly = None

    with open(path) as f:
        for line in f:
            fields = line.split()
            if not fields:
                continue
            key = fields[0]
            args = [int(v) for v in fields[1:] if re.fullmatch(r"-?\d+", v)]
            if key in ("ftyp", "chhw", "fhal", "fval"):
                header[key] = args
            elif key == "{":
                cur = (args[0], args[1], args[2], [])
                pen = (0.0, 0.0)
            elif key == "}":
                flush()
                code, lft, adv, polylines = cur
                glyphs[code] = (lft, adv, polylines)
                cur = None
            elif key == "m":
                flush()
                pen = (float(args[0]), float(args[1]))
            elif key == "d":
                if poly is None:
                    poly = [pen]
                pen = (float(args[0]), float(args[1]))
                poly.append(pen)
            elif key == "a":
                if poly is None:
                    poly = [pen]
                end = (float(args[0]), float(args[1]))
                poly.extend(arc_points(pen, end, float(args[2])))
                pen = end
            elif key == "c":
                # circle through pen and (x, y) as diametric points;
                # the pen keeps its pre-circle position (see getarc.c)
                flush()
                end = (float(args[0]), float(args[1]))
                cx = (pen[0] + end[0]) / 2.0
                cy = (pen[1] + end[1]) / 2.0
                r = math.hypot(end[0] - pen[0], end[1] - pen[1]) / 2.0
                if r > 0:
                    n = max(16, int(360 / ARC_STEP_DEG))
                    ring = [(cx + r * math.cos(2 * math.pi * i / n),
                             cy + r * math.sin(2 * math.pi * i / n))
                            for i in range(n + 1)]
                    cur[3].append(ring)
    return header, glyphs


def arc_points(p0, p1, sweep_deg):
    """Points along an arc from p0 to p1 subtending sweep_deg degrees.

    Positive sweep is counterclockwise.  Returns the intermediate points
    plus the endpoint (p0 is not included).
    """
    x0, y0 = p0
    x1, y1 = p1
    chord = math.hypot(x1 - x0, y1 - y0)
    phi = math.radians(sweep_deg)
    if chord == 0 or sweep_deg == 0:
        return [p1]
    r = chord / (2.0 * math.sin(abs(phi) / 2.0))
    # center = chord midpoint offset along the left normal; cos(phi/2)
    # goes negative for sweeps > 180 deg, putting the center on the far side
    mx, my = (x0 + x1) / 2.0, (y0 + y1) / 2.0
    ux, uy = (x1 - x0) / chord, (y1 - y0) / chord
    h = r * math.cos(phi / 2.0) * (1 if phi > 0 else -1)
    cx, cy = mx - uy * h, my + ux * h
    a0 = math.atan2(y0 - cy, x0 - cx)
    n = max(2, int(abs(sweep_deg) / ARC_STEP_DEG))
    pts = [(cx + r * math.cos(a0 + phi * i / n),
            cy + r * math.sin(a0 + phi * i / n)) for i in range(1, n)]
    pts.append(p1)
    return pts


def glyph_outline(lft, polylines, stroke_w, shear=0.0):
    """Stroke-expand a glyph's polylines; returns a TTGlyphPen glyph.

    `shear` slants the strokes about the baseline before the pen-width
    expansion, so the italic keeps a round nib.
    """
    pen = TTGlyphPen(None)
    lines = [LineString([((x + lft + y * shear) * SCALE, y * SCALE)
                         for x, y in p])
             for p in polylines]
    lines = [ls for ls in lines if ls.length > 0]
    if lines:
        shape = unary_union(MultiLineString(lines).buffer(
            stroke_w * SCALE / 2.0, quad_segs=8,
            cap_style="round", join_style="round"))
        polys = ([shape] if shape.geom_type == "Polygon" else list(shape.geoms))
        for poly in polys:
            poly = orient(poly.simplify(1.0, preserve_topology=True), -1.0)
            for ring in [poly.exterior, *poly.interiors]:
                coords = list(ring.coords)[:-1]     # drop repeated last point
                pen.moveTo((round(coords[0][0]), round(coords[0][1])))
                for x, y in coords[1:]:
                    pen.lineTo((round(x), round(y)))
                pen.closePath()
    return pen.glyph()


def build_face(header, glyphs, family, style, stroke):
    """Build one style-linked face; returns a TTFont."""
    bold = "Bold" in style
    italic = "Italic" in style
    shear = ITALIC_SHEAR if italic else 0.0
    fchw = header.get("chhw", [64, 55])[1]

    glyph_order = [".notdef", "space"]
    cmap = {32: "space"}
    tt_glyphs = {".notdef": TTGlyphPen(None).glyph(),
                 "space": TTGlyphPen(None).glyph()}
    advances = {".notdef": round(fchw * SCALE), "space": round(fchw * SCALE)}

    for code in sorted(glyphs):
        if not (33 <= code <= 126):
            continue
        lft, adv, polylines = glyphs[code]
        gname = agl.UV2AGL.get(code, "uni%04X" % code)
        glyph_order.append(gname)
        cmap[code] = gname
        tt_glyphs[gname] = glyph_outline(lft, polylines, stroke, shear)
        advances[gname] = round(adv * SCALE)

    fb = FontBuilder(UPM, isTTF=True)
    fb.setupGlyphOrder(glyph_order)
    fb.setupCharacterMap(cmap)
    fb.setupGlyf(tt_glyphs)     # recalculates glyph bounding boxes
    fb.setupHorizontalMetrics(
        {g: (advances[g], tt_glyphs[g].xMin
             if tt_glyphs[g].numberOfContours else 0)
         for g in glyph_order})

    def top(gname, default):
        g = tt_glyphs.get(gname)
        return g.yMax if g is not None and g.numberOfContours else default

    ymax = max((g.yMax for g in tt_glyphs.values() if g.numberOfContours), default=0)
    ymin = min((g.yMin for g in tt_glyphs.values() if g.numberOfContours), default=0)
    fb.setupHorizontalHeader(ascent=ymax, descent=ymin)
    ps = "%s-%s" % (family.replace(" ", ""), style.replace(" ", ""))
    fb.setupNameTable({
        "familyName": family,
        "styleName": style,
        "uniqueFontIdentifier": "1.000;NONE;" + ps,
        "fullName": "%s %s" % (family, style),
        "version": "Version 1.000",
        "psName": ps,
    })
    fsSelection = (0x20 if bold else 0) | (0x01 if italic else 0) or 0x40
    fb.setupOS2(sTypoAscender=ymax, sTypoDescender=ymin, sTypoLineGap=0,
                usWinAscent=ymax, usWinDescent=-ymin,
                usWeightClass=700 if bold else 400,
                fsSelection=fsSelection,
                sCapHeight=top("H", CHH_UNITS), sxHeight=top("x", 420))
    fb.setupPost(italicAngle=-float(ITALIC_DEG) if italic else 0.0)
    fb.font["head"].macStyle |= (1 if bold else 0) | (2 if italic else 0)
    if italic:
        fb.font["hhea"].caretSlopeRise = UPM
        fb.font["hhea"].caretSlopeRun = round(UPM * ITALIC_SHEAR)
    return fb.font


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("input")
    ap.add_argument("-o", "--output")
    ap.add_argument("--name", help="font family name")
    ap.add_argument("--stroke", type=float, default=None,
                    help="Regular pen width in grid units (default fchh * 4.5/64)")
    ap.add_argument("--bold-stroke", type=float, default=None,
                    help="Bold pen width in grid units (default fchh * 9/64)")
    ap.add_argument("--ttf", action="store_true",
                    help="write separate .ttf files instead of a .ttc")
    opt = ap.parse_args()

    base = os.path.splitext(os.path.basename(opt.input))[0]
    family = opt.name or "PLT " + base
    header, glyphs = parse_font(opt.input)

    global SCALE
    fchh = header.get("chhw", [64])[0]
    SCALE = CHH_UNITS / float(fchh)
    if opt.stroke is None:
        opt.stroke = fchh * 4.5 / 64.0
    if opt.bold_stroke is None:
        opt.bold_stroke = fchh * 9.0 / 64.0

    styles = [("Regular", opt.stroke), ("Italic", opt.stroke),
              ("Bold", opt.bold_stroke), ("Bold Italic", opt.bold_stroke)]
    faces = [build_face(header, glyphs, family, s, w) for s, w in styles]

    if opt.ttf:
        stem = os.path.splitext(opt.output or opt.input)[0]
        for face, (style, _) in zip(faces, styles):
            out = "%s-%s.ttf" % (stem, style.replace(" ", ""))
            face.save(out)
            print("wrote %s" % out)
    else:
        out = opt.output or os.path.splitext(opt.input)[0] + ".ttc"
        ttc = TTCollection()
        ttc.fonts = faces
        ttc.save(out)
        print("wrote %s (%s: Regular/Italic %.2f/%d, Bold/Bold Italic %.2f/%d)"
              % (out, family, opt.stroke, fchh, opt.bold_stroke, fchh))


if __name__ == "__main__":
    main()

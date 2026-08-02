import os
import subprocess

html_header = """<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>PLT Examples</title>
    <link href="https://fonts.googleapis.com/css2?family=Outfit:wght@300;400;600&display=swap" rel="stylesheet">
    <style>
        :root { --bg-color: #0f172a; --text-color: #f8fafc; --primary-color: #3b82f6; --secondary-color: #1e293b; --accent-color: #06b6d4; }
        body { font-family: "Outfit", sans-serif; background: var(--bg-color); color: var(--text-color); padding: 40px; }
        a { color: var(--accent-color); text-decoration: none; }
        a:hover { text-decoration: underline; }
        .grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(300px, 1fr)); gap: 20px; }
        .card { background: var(--secondary-color); padding: 15px; border-radius: 8px; text-align: center; }
        .card img { max-width: 100%; height: auto; background: white; margin-bottom: 10px; border-radius: 4px; }
        h1 { margin-bottom: 30px; }
        .back { display: inline-block; margin-top: 30px; padding: 10px 20px; background: var(--primary-color); color: white; border-radius: 8px; }
    </style>
</head>
<body>
    <h1>PLT Examples</h1>
    <p>Browse through the collection of PLT examples, generated directly by the PLT engine.</p>
    <div class="grid">
"""

html_footer = """
    </div>
    <a href="../" class="back">Back to Home</a>
</body>
</html>
"""

ignore_list = {
    "a.plt", "ang.plt", "arrows.plt", "b.plt", "bdat.plt", "bez.plt",
    "bigbox.plt", "btni.plt", "c.plt", "cd.plt", "circles.plt", "cl.plt",
    "cs.plt", "cumu.plt", "dblerr.plt", "ega.plt", "errbar.plt", "ff.plt",
    "fig1.plt", "fig2.plt", "fig3.plt", "fig4.plt", "fig5.plt", "fig6.plt",
    "fig7.plt", "fig8.plt", "fig9.plt", "fnt.plt", "fontgeom.plt", "fonts.plt",
    "for.plt", "g.plt", "graysymb.plt", "grid.plt", "h.plt", "histo.plt",
    "hp.plt", "key.plt", "kw.plt", "l.plt", "lw.plt", "mc.plt", "msg.plt",
    "n.plt", "ni.plt", "norm.plt", "pat.plt", "qerb.plt", "random.plt",
    "rect.plt", "roex.plt", "rsqr.plt", "rt.plt", "slide.plt", "soae.plt",
    "sort.plt", "sp.plt", "sqrt.plt", "ss.plt", "star.plt", "sufi.plt",
    "symmap.plt", "tp.plt", "twinkle.plt", "vbc2.plt", "wave.plt", "xx.plt",
    "ztop.plt"
}

plt_files = [f for f in os.listdir("plots") if f.endswith(".plt") and f not in ignore_list]
plt_files.sort()

success_files = []

for plt in plt_files:
    basename = plt[:-4]
    svg_path = f"plots/{basename}.svg"
    try:
        # Run tplt and redirect output to SVG
        with open(svg_path, "w") as f:
            result = subprocess.run(["./tplt/tplt", "-m8", f"plots/{plt}"], stdout=f, stderr=subprocess.PIPE, timeout=2)
        
        # Check if the generated SVG is non-empty
        if os.path.exists(svg_path) and os.path.getsize(svg_path) > 0:
            success_files.append(basename)
        else:
            if os.path.exists(svg_path):
                os.remove(svg_path)
    except Exception as e:
        pass

with open("plots/index.html", "w") as f:
    f.write(html_header)
    for basename in success_files:
        f.write(f'        <div class="card">\n')
        f.write(f'            <a href="{basename}.svg"><img src="{basename}.svg" alt="{basename}"></a>\n')
        f.write(f'            <br><a href="{basename}.plt">{basename}.plt (Source)</a>\n')
        f.write(f'        </div>\n')
    f.write(html_footer)

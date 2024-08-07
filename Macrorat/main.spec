# -*- mode: python ; coding: utf-8 -*-


a = Analysis(
    ['main.py'],
    pathex=[],
    binaries=[],
    datas=[('img/trigger/lootof.png', 'img/trigger'), ('img/dump/t4/t4_1.png', 'img/dump/t4'), ('img/dump/t4/t4_2.png', 'img/dump/t4'), ('img/dump/t4/t4_3.png', 'img/dump/t4'), ('img/dump/t4/t4_4.png', 'img/dump/t4'), ('img/dump/t4/t4_5.png', 'img/dump/t4'), ('img/dump/t5/t5_1.png', 'img/dump/t5'), ('img/dump/t5/t5_2.png', 'img/dump/t5'), ('img/dump/t5/t5_3.png', 'img/dump/t5'), ('img/dump/t5/t5_4.png', 'img/dump/t5'), ('img/dump/t5/t5_5.png', 'img/dump/t5'), ('img/dump/t6/t6_1.png', 'img/dump/t6'), ('img/dump/t6/t6_2.png', 'img/dump/t6'), ('img/dump/t6/t6_3.png', 'img/dump/t6'), ('img/dump/t6/t6_4.png', 'img/dump/t6'), ('img/dump/t6/t6_5.png', 'img/dump/t6'), ('img/dump/t7/t7_1.png', 'img/dump/t7'), ('img/dump/t7/t7_2.png', 'img/dump/t7'), ('img/dump/t7/t7_3.png', 'img/dump/t7'), ('img/dump/t7/t7_4.png', 'img/dump/t7'), ('img/dump/t7/t7_5.png', 'img/dump/t7'), ('img/dump/t8/t8_1.png', 'img/dump/t8'), ('img/dump/t8/t8_2.png', 'img/dump/t8'), ('img/dump/t8/t8_3.png', 'img/dump/t8'), ('img/dump/t8/t8_4.png', 'img/dump/t8'), ('img/dump/t8/t8_5.png', 'img/dump/t8'), ('img/exclude/t3_horse.png', 'img/exclude'), ('img/exclude/t4_stag.png', 'img/exclude'), ('img/exclude/t5_armored_horse.png', 'img/exclude'), ('img/exclude/t5_graywolf.png', 'img/exclude'), ('img/exclude/t5_swiftclaw.png', 'img/exclude'), ('img/exclude/t6_wolf.png', 'img/exclude'), ('img/trash/empty.png', 'img/trash'), ('img/trash/t1_trash.png', 'img/trash'), ('img/trash/t2_trash.png', 'img/trash'), ('img/trash/t3_trash.png', 'img/trash'), ('img/trash/t4_trash.png', 'img/trash'), ('img/trash/t5_trash.png', 'img/trash'), ('img/trash/t6_trash.png', 'img/trash'), ('img/trash/t7_trash.png', 'img/trash'), ('img/trash/t8_trash.png', 'img/trash')],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)
pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='main',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=True,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon=['img\\icon\\rat.ico'],
)

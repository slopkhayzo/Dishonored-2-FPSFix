Dishonored 2 FPS Fix v@VERSION@
================================

Supported game
--------------
GOG Dishonored 2 version 1.77.9.0 for Windows x64 only.

Dishonored2.exe SHA-256:
C3150F9F2D9BF967D23CA6A79BB32703B90116854AAD9892C7FC1F46A1060293

The ASI plugin verifies this complete hash and the original hook bytes. It
fails closed without modifying an unsupported executable.

Choose an installation
----------------------
Plugin-only archive:
  Requires a compatible x64 ASI loader already installed. Copy
  Dishonored2HighFPSFix.asi and d2-high-fps-fix.ini beside Dishonored2.exe, or
  into the plugin directory configured for your loader. Keep both files
  together.

Convenience archive with Ultimate ASI Loader:
  If the game has no ASI loader, copy dinput8.dll,
  Dishonored2HighFPSFix.asi, and d2-high-fps-fix.ini beside Dishonored2.exe.
  The bundled dinput8.dll is Ultimate ASI Loader x64 v9.7.4.

  If the game already has a compatible ASI loader, do not overwrite it. Copy
  only Dishonored2HighFPSFix.asi and d2-high-fps-fix.ini. The fix itself does
  not need to own the loader.

For either installation:
1. Close Dishonored 2 before copying files.
2. Keep Triple Buffering disabled in the game.
3. Set your desired maximum FPS in NVIDIA Control Panel or another external
   limiter. Do not run completely uncapped.
4. Launch the game normally.

Usage
-----
The supplied configuration enables the tested complete path: FPS unlocking,
frame-rate-independent mouse sensitivity, camera and first-person root
stabilization, eligible world/root interpolation, first-person and selected
world skeletal interpolation, and small cinematic-control interpolation.

Press F10 in game to toggle camera prediction and first-person root correction
for an A/B comparison. F10 does not change the FPS limit or the other
interpolation settings.

Removing the game's FPS-dependent mouse multiplier makes the same in-game
sensitivity feel lower at high FPS. Raise the in-game sensitivity once to your
preference; it should then remain consistent as the frame rate changes.

Safety and scope
----------------
The game simulation, physics, AI, scripts, animation events, and authoritative
gameplay state remain at the native 120 Hz. Corrections are applied only to
temporary renderer-owned camera, model-matrix, and joint-palette data.

Unsupported content snaps to the native state. Simulated cloth is not
interpolated and may still look stepped in some cinematics.

Keep ShadowTransforms=0. The experimental shadow path is not safe for normal
use. Keep Telemetry=0 unless developing the patch.

Diagnostics
-----------
The patch writes d2-high-fps-fix.log beside the ASI. Check it if the fix does
not activate; executable or hook-byte mismatches are reported there.

Uninstallation
--------------
Close the game and remove Dishonored2HighFPSFix.asi and
d2-high-fps-fix.ini. The optional d2-high-fps-fix.log diagnostic file may also
be removed.

Do not remove dinput8.dll if it existed before this fix or if another plugin
depends on it. Remove the bundled loader only when you installed that exact
file for this fix and no other ASI plugin needs it.

Third-party component
---------------------
The explicitly named convenience archive includes Ultimate ASI Loader v9.7.4
by ThirteenAG under the MIT License. See THIRD-PARTY-NOTICES.txt. The
plugin-only archive contains no third-party loader binary.

Misc
----
For more game fixes, visit:
https://slop-blog.enkhayzomachines.net/fixes :)

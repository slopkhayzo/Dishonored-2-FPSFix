# Dishonored 2 FPS Fix

> [!WARNING]
> This patch has been pretty much entirely been generated using AI; 
> I won't and will never claim to have enough knowledge or expertise to do this 
> kind of reverse-engineering by myself; I've tested three levels at 360hz + 
> briefly tested at 120, 144 and 240 and so far spotted 
> no major issues (on my machine ofc, if you have issues feel free to open an Issue)
> this patch currently probably only works for the latest GOG version of the game, I do have 
> the Steam version too but still have to test that, so no guarantees for now

A source-only high-frame-rate fix for the GOG release of Dishonored 2. It keeps the game's simulation and physics at their native 120 Hz while smoothing presentation above 120 FPS by interpolating all world assets, player included, alongside skeletal animations to match the display framerate, with an additional per-frame mouse delta override to keep mouse input latency low.

Current release: **v1.1.0**

## Compatibility

This patch supports one executable only:

- Dishonored 2 GOG, version 1.77.9.0
- `Dishonored2.exe` SHA-256: `C3150F9F2D9BF967D23CA6A79BB32703B90116854AAD9892C7FC1F46A1060293`
- Windows x64

The ASI plugin verifies the full executable hash and the original hook bytes before installing any hook. Other game versions and storefront builds fail closed and are not modified.

## What it fixes

- Removes the game's internal 120 FPS presentation cap so an external limiter can be used.
- Smooths the renderer camera above the 120 Hz simulation rate.
- Selects the game's frame-rate-independent mouse sensitivity path.
- Optionally keeps first-person body and weapon roots aligned with the smoothed camera.
- Provides separately gated interpolation for eligible world transforms, first-person skeletal animation, selected world skeletal animation, and small ownerless cinematic controls.

The patch does not raise the simulation rate or modify authoritative physics, AI, scripts, animation events, or gameplay state. Presentation corrections are applied only to temporary renderer-owned data.

## Current status

Camera prediction, FPS unlocking, mouse stabilization, first-person root correction, world/root interpolation, skeletal interpolation, and cinematic-control interpolation have passed focused live tests and multi-level gameplay/lifecycle testing without a major issue. The validated presentation layers are enabled by default in v1.1.0 and can still be disabled independently.

Two areas are deliberately unsupported:

- Simulated cloth, including the cinematic hoodie/garment case, still advances at the native simulation cadence.
- `ShadowTransforms` must remain `0`. The investigated shadow boundary was rejected because it could crash during transitions or cause caster flicker.

## Build

Install Visual Studio 2022 or the current Visual Studio Build Tools with the **Desktop development with C++** workload, then run from a Command Prompt:

```bat
build-msvc.cmd
```

The script locates the x64 MSVC toolchain, builds
`build\Dishonored2HighFPSFix.asi`, builds and runs the direct ASI load test,
builds the external-loader integration host, and builds and runs the joint-pose
interpolation tests. No game files are needed to compile or test the source.

To run the ASI load test manually:

```bat
build\asi-load-test.exe build\Dishonored2HighFPSFix.asi
```

The test loads the `.asi` directly into an unsupported host and verifies that
it has no DirectInput proxy export, resolves its sibling log path, and refuses
to install game hooks. It does not replace in-game testing through the selected
ASI loader.

To test discovery and forwarding through an external loader without modifying
the game installation, build first and then provide the path to an x64 loader:

```powershell
.\test-asi-loader.ps1 -LoaderPath C:\path\to\dinput8.dll
```

The script creates a disposable staging directory below `build`, runs the
integration host, and verifies loader discovery, system DirectInput forwarding,
plugin loading, sibling logging, and safe unsupported-host rejection. The
official Ultimate ASI Loader v9.7.4 x64 build passes this test and the v1.1.0
in-game feature-parity validation.

## Package

After building and validating the plugin, create the canonical plugin-only
archive and the optional tested-loader convenience archive with:

```powershell
.\package-release.ps1 -Version 1.1.0 `
  -LoaderPath .\build\third-party\ultimate-asi-loader-v9.7.4\extracted\dinput8.dll
```

The packaging script pins the expected loader hash, generates per-file and ZIP
SHA-256 manifests, includes the required third-party notice only in the loader
bundle, and refuses to overwrite an existing release archive. Omit
`-LoaderPath` to produce only the plugin-only archive.

## Install

1. Close Dishonored 2.
2. Build the project.
3. Install a compatible x64 ASI loader, such as Ultimate ASI Loader, according
   to that loader's documentation. If the game already has a compatible
   loader, keep it.
4. Copy `build\Dishonored2HighFPSFix.asi` and `d2-high-fps-fix.ini` beside
   `Dishonored2.exe`, or into the plugin directory configured for the loader.
   Keep the ASI and INI together.
5. Keep **Triple Buffering** disabled in the game.
6. Set the desired maximum frame rate in the NVIDIA Control Panel or another external limiter. Do not run completely uncapped.
7. Launch the game normally.

The canonical release is plugin-only. A separately named convenience archive
also bundles the tested Ultimate ASI Loader v9.7.4 x64 build. When using that
archive on a game with no loader, copy its `dinput8.dll` too. If the game
already has a compatible loader, do not overwrite it; copy only the ASI and
INI. [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader)
is MIT-licensed, and the convenience archive includes its required notice.

The patch writes `d2-high-fps-fix.log` beside the ASI. If the executable hash
or guarded bytes do not match, the log explains why the hooks were not
installed.

## Configuration

Edit `d2-high-fps-fix.ini` while the game is closed.

### Stable settings

- `UnlockAbove120=1` disables the game's frame spinner. An external FPS limiter is still required.
- `StabilizeMouseSensitivity=1` removes the game's FPS-dependent mouse multiplier. Mouse sensitivity will feel lower at high FPS; raise the in-game sensitivity once to taste.
- `StabilizeFirstPersonHands=1` enables the validated renderer-only first-person root correction. It is enabled by default.

Press **F10** in game to toggle camera prediction and the enabled first-person root correction for an A/B comparison. F10 does not change the FPS limit or the separate interpolation options.

### Interpolation settings

The following validated layers are enabled by default and can be disabled independently:

- `WorldTransforms=1`
- `FirstPersonSkeletons=1`
- `WorldSkeletons=1`
- `CinematicTransforms=1`
- `CinematicSkeletons=1`

The supplied configuration enables all five together for the complete validated presentation path. Unsupported models, invalid layouts, discontinuities, teleports, identity changes, and missing history snap to the current native state instead of being blended.

Leave these settings unchanged:

- `ShadowTransforms=0` — required; the prototype is not safe for normal use.
- `Telemetry=0` — normal play does not need the development telemetry block.

## Uninstall

Close the game and remove these files from the game directory:

- `Dishonored2HighFPSFix.asi`
- `d2-high-fps-fix.ini`
- `d2-high-fps-fix.log` (optional diagnostic log)

Do not remove a shared or pre-existing ASI loader when uninstalling this
plugin.

## Technical outline

The patch is an x64 ASI plugin loaded by an external ASI loader. On the
supported executable it installs guarded renderer hooks, retains recent
fixed-step samples, and evaluates temporary camera, model-matrix, or
joint-palette data at presentation time. It does not export or forward
`DirectInput8Create`. All optional layers are bounded and fail closed when
their expected layout, identity, timing, or continuity checks do not pass.

The repository contains the patch source and build/package instructions. Release
archives contain only the built plugin, configuration, documentation, hashes,
and—only in the explicitly named convenience archive—the third-party ASI
loader and its required notice. No Dishonored 2 executable or game asset is
distributed.

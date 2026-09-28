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

Current release: **v1.0.0**

## Compatibility

This patch supports one executable only:

- Dishonored 2 GOG, version 1.77.9.0
- `Dishonored2.exe` SHA-256: `C3150F9F2D9BF967D23CA6A79BB32703B90116854AAD9892C7FC1F46A1060293`
- Windows x64

The DLL verifies the full executable hash and the original hook bytes before installing any hook. Other game versions and storefront builds fail closed and are not modified.

## What it fixes

- Removes the game's internal 120 FPS presentation cap so an external limiter can be used.
- Smooths the renderer camera above the 120 Hz simulation rate.
- Selects the game's frame-rate-independent mouse sensitivity path.
- Optionally keeps first-person body and weapon roots aligned with the smoothed camera.
- Provides separately gated interpolation for eligible world transforms, first-person skeletal animation, selected world skeletal animation, and small ownerless cinematic controls.

The patch does not raise the simulation rate or modify authoritative physics, AI, scripts, animation events, or gameplay state. Presentation corrections are applied only to temporary renderer-owned data.

## Current status

Camera prediction, FPS unlocking, mouse stabilization, first-person root correction, world/root interpolation, skeletal interpolation, and cinematic-control interpolation have passed focused live tests and multi-level gameplay/lifecycle testing without a major issue. The validated presentation layers are enabled by default in v1.0.0 and can still be disabled independently.

Two areas are deliberately unsupported:

- Simulated cloth, including the cinematic hoodie/garment case, still advances at the native simulation cadence.
- `ShadowTransforms` must remain `0`. The investigated shadow boundary was rejected because it could crash during transitions or cause caster flicker.

## Build

Install Visual Studio 2022 or the current Visual Studio Build Tools with the **Desktop development with C++** workload, then run from a Command Prompt:

```bat
build-msvc.cmd
```

The script locates the x64 MSVC toolchain, builds `build\dinput8.dll`, builds the forwarding smoke test, and builds and runs the joint-pose interpolation tests. No game files are needed to compile the source.

To run the proxy forwarding test manually:

```bat
build\proxy-smoke-test.exe build\dinput8.dll
```

The test host is not the supported game executable, so the proxy will load and forward `DirectInput8Create` while correctly refusing to install game hooks.

## Install

1. Close Dishonored 2.
2. Build the project.
3. Copy `build\dinput8.dll` and `d2-high-fps-fix.ini` beside `Dishonored2.exe`.
4. Keep **Triple Buffering** disabled in the game.
5. Set the desired maximum frame rate in the NVIDIA Control Panel or another external limiter. Do not run completely uncapped.
6. Launch the game normally.

Do not overwrite another mod's `dinput8.dll`. Proxy DLLs cannot simply be stacked; use only one compatible loader arrangement.

The patch writes `d2-high-fps-fix.log` beside the DLL. If the executable hash or guarded bytes do not match, the log explains why the hooks were not installed.

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

- `dinput8.dll`
- `d2-high-fps-fix.ini`
- `d2-high-fps-fix.log` (optional diagnostic log)

## Technical outline

The patch is a `dinput8.dll` proxy that forwards `DirectInput8Create` to the Windows system DLL. On the supported executable it installs guarded renderer hooks, retains recent fixed-step samples, and evaluates temporary camera, model-matrix, or joint-palette data at presentation time. All optional layers are bounded and fail closed when their expected layout, identity, timing, or continuity checks do not pass.

The repository intentionally contains source code and build instructions only. It does not include Dishonored 2 executables, assets, extracted data, reverse-engineering databases, captures, logs, or research notes.

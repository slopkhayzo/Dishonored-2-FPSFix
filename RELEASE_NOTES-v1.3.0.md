# Dishonored 2 FPS Fix v1.3.0

## What changed

- Added an adaptive interpolation controller, enabled by default, that measures
  presentation cadence through the fixed 120 Hz simulation clock.
- The controller sheds skeletal interpolation first when full interpolation
  lacks useful native-rate headroom, then sheds transform interpolation if the
  reduced profile remains too expensive.
- Recovery uses observed per-layer cost, sustained hysteresis, and controlled
  context-refresh probes. This avoids both a simple below/above-120 toggle loop
  and stale cost estimates after moving into a materially lighter scene.
- Added `Interpolation/AdaptivePerformanceGate`. Its default value is `1`; set
  it to `0` to keep every individually enabled interpolation layer continuously
  active. Camera prediction, the native-rate tick fix, mouse stabilization, and
  FPS unlocking are unaffected.
- Reduced skeletal hot-path overhead by deferring the render-palette clone until
  a valid presentation correction exists and by removing an unnecessary large
  temporary-palette initialization.
- Added a disabled-by-default in-session A/B measurement harness for development
  captures. It can isolate full, transforms-only, skeletons-only, and disabled
  interpolation profiles without changing prediction or tick behavior.

## Release archives

- `Dishonored2-FPSFix-v1.3.0-plugin-only.zip` contains the ASI, configuration,
  project license, documentation, and payload hashes for installations that
  already have a compatible x64 ASI loader.
  SHA-256:
  `43FC0D4D1B2B65375E2C789FFCA5679F8CF61505C71073F97B11A5309DADEC5D`
- `Dishonored2-FPSFix-v1.3.0-with-Ultimate-ASI-Loader-v9.7.4.zip` additionally
  contains the pinned x64 loader and its required MIT notice.
  SHA-256:
  `97F5432EE4092A0DC4762D5C58D1DEB791A842EF9B9C34094423790162FDE137`

## Validation

- Reference executable SHA-256:
  `C3150F9F2D9BF967D23CA6A79BB32703B90116854AAD9892C7FC1F46A1060293`
- ASI SHA-256:
  `EB73F991D6B09FFDE5A496B1739DC9B4C88B5CD1277CD686D4D54D5AA31B186B`
- Ultimate ASI Loader v9.7.4 x64 SHA-256:
  `FA266E3513D02C08A1B808F28C10538A489EAFFAA4B0707F7CC1066E71B5AFD7`
- Compilation with warnings enabled, joint-pose tests, direct ASI loading, both
  fail-closed host tests, external-loader discovery, and DirectInput forwarding
  passed.
- Controlled same-session A/B captures covered light, medium, and heavy scenes.
  They isolated skeletal interpolation as the dominant optional cost and
  confirmed the benefit of retaining only transform interpolation where useful.
- Two free-play adaptive-controller sessions exercised shedding, ordinary
  recovery, stale-cost refresh, and changing-view recovery. The final session
  produced no capacity miss, upload failure, interpolation/layout rejection,
  visible discontinuity, judder, or other observed gameplay problem.
- Both extracted archives passed their internal payload-manifest checks and
  their appropriate direct or bundled-loader runtime smoke test.

The GOG 1.77.9.0 executable remains the fully researched and live-tested
reference. Layout-compatible admission of a different hash is deliberately not
presented as equivalent gameplay validation for another release.

`ShadowTransforms` remains unsupported and disabled. Simulated cloth remains
outside the release scope.

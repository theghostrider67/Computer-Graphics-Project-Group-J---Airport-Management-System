<p align="center">
  <img src="https://readme-typing-svg.demolab.com?font=Fira+Code&weight=700&size=28&pause=1000&color=3B82F6&center=true&vCenter=true&width=700&lines=Triple+Airport+System;OpenGL+%7C+GLUT+%7C+C%2B%2B;Animated+2D+Airport+Simulation" alt="Typing SVG" />
</p>

<h1 align="center">✈️ Airport Management System</h1>
<h4 align="center">An animated 2D airport simulation built with OpenGL & GLUT — featuring three unique airports, real-time weather, day/night cycles, and a fully animated flight loop.</h4>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge&logo=cplusplus&logoColor=white" />
  <img src="https://img.shields.io/badge/Graphics-OpenGL-orange?style=for-the-badge&logo=opengl&logoColor=white" />
  <img src="https://img.shields.io/badge/Toolkit-GLUT-green?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" />
  <img src="https://img.shields.io/badge/IDE-Code%3A%3ABlocks-red?style=for-the-badge" />
</p>

<p align="center">
  <img src="https://img.shields.io/github/stars/theghostrider67/Computer-Graphics-Project-Group-J---Airport-Management-System?style=social" />
  <img src="https://img.shields.io/github/forks/theghostrider67/Computer-Graphics-Project-Group-J---Airport-Management-System?style=social" />
  <img src="https://img.shields.io/github/last-commit/theghostrider67/Computer-Graphics-Project-Group-J---Airport-Management-System?style=flat-square" />
</p>

---

## 📖 About The Project

This is a **Computer Graphics** course project developed for **American International University-Bangladesh (AIUB)** by **Group J**, under the guidance of faculty **Dipta Justin Gomes**.

The application renders a **2D animated airport ecosystem** where a single aircraft continuously flies between three uniquely designed international airports — each with distinct architecture, color palettes, and environmental details. The simulation includes interactive weather systems, a day/night cycle, scrolling signboards, and a complete flight state machine handling takeoff, cruise, and landing sequences.

> **Built entirely from scratch** using the legacy OpenGL fixed-function pipeline — no textures, no models, no external assets. Every building, mountain, cloud, and aircraft is rendered procedurally through OpenGL primitives.

---

## 🏗️ Architecture Overview

```
┌─────────────────────────────────────────────────────────┐
│                    GLUT Main Loop                       │
│              glutMainLoop() @ ~60 FPS                   │
├──────────┬──────────┬──────────┬────────────────────────┤
│ Display  │ Keyboard │  Timer   │  Reshape               │
│ Callback │ Handler  │ Update   │  Handler               │
├──────────┴──────────┴──────────┴────────────────────────┤
│                                                         │
│  ┌─────────────────────────────────────────────────┐    │
│  │           Scene Rendering Pipeline              │    │
│  │                                                 │    │
│  │  1. Background (Sky Gradient)                   │    │
│  │  2. Environment (Mountains / Skyline / Silos)   │    │
│  │  3. Ground & Runway                             │    │
│  │  4. Terminal Building & Control Tower            │    │
│  │  5. Bangladesh Flag & Scrolling Signboard       │    │
│  │  6. Clouds (Animated)                           │    │
│  │  7. Stars (Night Mode)                          │    │
│  │  8. Night Overlay (Alpha Blending)              │    │
│  │  9. Weather Particles (Rain / Snow)             │    │
│  │ 10. Airplane (Transformed & Scaled)             │    │
│  └─────────────────────────────────────────────────┘    │
│                                                         │
│  ┌─────────────────────────────────────────────────┐    │
│  │           Flight State Machine                  │    │
│  │                                                 │    │
│  │  Stage 0 ──► Depart Rafi's Airport              │    │
│  │  Stage 1 ──► Arrive Tasin's Airport             │    │
│  │  Stage 2 ──► Depart Tasin's Airport             │    │
│  │  Stage 3 ──► Arrive Fahim's Airport ──► Loop    │    │
│  └─────────────────────────────────────────────────┘    │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

---

## ✨ Features

### 🏢 Three Unique Airport Environments

| Airport | Architecture | Background | Tower Style |
|---------|-------------|------------|-------------|
| **Rafi's International** | Light grey terminal, blue glass windows, jet bridges | Mountain range with layered peaks | Globe-shaped dome with geodesic lines & radar dots |
| **Tasin's International** | Dark industrial terminal, amber glass windows | Industrial silos and fuel/cargo tanks | Heavy square tower with dark cab |
| **Fahim's International** | Sleek modern terminal, blue-grey glass facade | City skyline with satellite radar facility | Futuristic angular tower with trapezoidal cab |

### 🛫 Animated Aircraft System

- Detailed 2D airplane: fuselage, wings, tail fin, yellow racing stripe, landing gear
- Smooth **takeoff** with pitch rotation and vertical climb
- Automated **landing** with descent curve and nose leveling
- Speed-independent flight curves using ratio-based normalization
- `glPushMatrix` / `glPopMatrix` isolation for per-frame transforms

### 🌦️ Dynamic Weather System

- **Rain**: 300 particle lines (`GL_LINES`) with wind-slant simulation
- **Snow**: 300 particle points (`GL_POINTS`) with sinusoidal horizontal drift
- Particle recycling — particles reset to the top when they fall below ground
- Mutually exclusive toggle (rain disables snow and vice versa)

### 🌙 Day / Night Cycle

- Night mode adds **100 procedurally placed stars** (`GL_POINTS`)
- Semi-transparent dark blue overlay via **alpha blending** (`GL_BLEND`)
- Clouds hidden during nighttime for visual realism

### 📡 Scrolling Signboard

- LED-style airport signboard on each control tower
- Horizontally scrolling text using `glutBitmapCharacter`
- Clipped to the signboard rectangle with `glScissor` (scissor test)

### 🇧🇩 Bangladesh National Flag

- Each control tower flies a Bangladesh flag (green field + red circle)
- Drawn procedurally on a flagpole atop each tower

---

## 🎮 Controls

| Key | Action |
|:---:|--------|
| `S` | **Stop** the plane on the runway |
| `W` | **Resume** flight |
| `N` | Switch to **Night** mode |
| `D` | Switch to **Day** mode |
| `R` | Toggle **Rain** (disables snow) |
| `T` | Toggle **Snow** (disables rain) |
| `↑` | **Increase** plane speed |
| `↓` | **Decrease** plane speed |

---

## 🛠️ Tech Stack

| Component | Technology |
|-----------|-----------|
| **Language** | C++ (single-file, ~600 lines) |
| **Rendering** | OpenGL 1.x/2.x (Fixed-Function Pipeline) |
| **Windowing** | GLUT (OpenGL Utility Toolkit) |
| **Projection** | 2D Orthographic via `gluOrtho2D` |
| **Animation** | `glutTimerFunc` at ~60 FPS |
| **Buffering** | Double Buffering (`GLUT_DOUBLE`) |
| **Text** | Bitmap font rendering (`GLUT_BITMAP_HELVETICA_12`) |

### OpenGL Features Used

| Feature | Usage |
|---------|-------|
| `GL_TRIANGLE_FAN` | Filled circles (clouds, radar dome, wheels) |
| `GL_TRIANGLES` | Mountains, wings, tail fin |
| `GL_POLYGON` | Plane fuselage, tower cab |
| `GL_LINES` | Flagpoles, antennas, rain streaks, runway dashes |
| `GL_POINTS` | Stars, radar dots, snowflakes |
| `glRectf()` | Buildings, windows, doors, runway, signboards |
| `glScissor()` | Text clipping on signboard region |
| `GL_BLEND` | Night mode semi-transparent overlay |
| `glTranslatef` / `glRotatef` / `glScalef` | Airplane movement, tilt, and scaling |

---

## 📂 Project Structure

```
Computer-Graphics-Project-Group-J---Airport-Management-System/
│
├── 📄 README.md                          # This file
├── 📄 .gitattributes                     # Git attributes configuration
│
└── 📁 Main Project/
    ├── 📄 main.cpp                       # Complete source code (~600 lines)
    ├── 📄 Main Project.cbp               # Code::Blocks project file
    ├── 📄 Main Project.depend            # Auto-generated dependency tree
    ├── 📄 Main Project.layout            # IDE editor state
    │
    ├── 📁 bin/Debug/
    │   └── 🎮 Main Project.exe           # Pre-compiled debug binary
    │
    └── 📁 obj/Debug/
        └── 📦 main.o                     # Compiled object file
```

---

## 🚀 Getting Started

### Prerequisites

- **OS**: Windows (uses `<windows.h>` for OpenGL context)
- **Compiler**: GCC / MinGW (bundled with Code::Blocks)
- **Libraries**: OpenGL, GLU, GLUT (typically included with MinGW)

### Option 1: Run Pre-built Binary

A pre-compiled executable is included in the repository:

```
Main Project/bin/Debug/Main Project.exe
```

> ⚠️ Make sure GLUT DLLs (`glut32.dll`) are available in your system PATH or in the same directory as the executable.

### Option 2: Build with Code::Blocks

1. Install [Code::Blocks with MinGW](https://www.codeblocks.org/downloads/)
2. Open `Main Project/Main Project.cbp`
3. Press **F9** or click **Build and Run**

### Option 3: Build from Command Line

```bash
cd "Main Project"
g++ -Wall -g main.cpp -o "AirportSystem.exe" -lglut32 -lopengl32 -lglu32 -lwinmm -lgdi32
```

### GLUT Setup (If Not Installed)

If you encounter linking errors for GLUT:

1. Download [freeglut](https://www.transmissionzero.co.uk/software/freeglut-devel/) for MinGW
2. Copy `freeglut.dll` → `C:/Windows/System32/`
3. Copy header files → MinGW's `include/GL/` directory
4. Copy `.a` library files → MinGW's `lib/` directory

---

## 🧩 Dependencies

| Library | Header | Linker Flag | Purpose |
|---------|--------|-------------|---------|
| OpenGL | `<GL/gl.h>` | `-lopengl32` | Core rendering (primitives, transforms, blending) |
| GLU | `<GL/glu.h>` | `-lglu32` | `gluOrtho2D` for 2D orthographic projection |
| GLUT | `<GL/glut.h>` | `-lglut32` | Window management, input, timer, bitmap fonts |
| WinMM | — | `-lwinmm` | Windows multimedia API (linked, not explicitly used) |
| GDI32 | — | `-lgdi32` | Windows GDI for OpenGL context support |

**Standard C++ Headers**: `<windows.h>`, `<cmath>`, `<cstdlib>`, `<ctime>`

---

## 🧠 How It Works

### Flight State Machine

The aircraft follows a 4-stage looping state machine:

```
        ┌──────────────────────────────────────────────────────┐
        │                                                      │
        ▼                                                      │
   ┌─────────┐      ┌─────────┐      ┌─────────┐      ┌─────────┐
   │ Stage 0 │ ───► │ Stage 1 │ ───► │ Stage 2 │ ───► │ Stage 3 │
   │ Depart  │      │ Arrive  │      │ Depart  │      │ Arrive  │
   │ Rafi's  │      │ Tasin's │      │ Tasin's │      │ Fahim's │
   └─────────┘      └─────────┘      └─────────┘      └─────────┘
```

Each transition is triggered when the plane passes a specific X-coordinate threshold. Takeoff applies positive Y-velocity and rotation; landing applies negative Y-velocity with gradual nose leveling.

### Weather Particle System

```cpp
struct Particle {
    float x, y;    // Position
    float speed;   // Fall velocity
    float drift;   // Horizontal sway (snow only)
};
```

- **300 particles** initialized with random positions and speeds
- Rain: vertical fall with 0.02 horizontal offset (wind simulation)
- Snow: slower fall speed with `sin(y * 10)` horizontal drift
- Particles reset to top of screen when they reach ground level

### Rendering Pipeline

Each frame (`display()`) renders layers bottom-to-top:

1. Clear buffer → Load identity
2. Draw current airport scene (selected by `currentAirport`)
3. Overlay stars (if night)
4. Overlay night tint (if night, via alpha blending)
5. Overlay weather particles (if active)
6. Draw airplane (with matrix transforms)
7. Swap buffers (double buffering)

---

## 👥 Team — Group J

| Member | Contribution |
|--------|-------------|
| **Rafi** | Airport 1 — Mountain-themed international airport with globe control tower |
| **Tasin** | Airport 2 — Industrial-themed airport with heavy square tower and silos |
| **Fahim** | Airport 3 — Modern futuristic airport with angular tower and city skyline |

**Course**: Computer Graphics  
**University**: American International University-Bangladesh (AIUB)  
**Faculty**: Dipta Justin Gomes

---

## 📜 License

This project is developed for academic purposes as part of a university course. Feel free to use it as a reference or learning resource.

---

<p align="center">
  <b>Built with ❤️ using pure OpenGL primitives — no textures, no models, no shortcuts.</b>
</p>

<p align="center">
  <a href="#-airport-management-system">⬆ Back to Top</a>
</p>

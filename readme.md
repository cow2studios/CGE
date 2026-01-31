# Cow Game Engine

**Cow Game Engine** is a custom C++ game engine designed for creating **Hyper2D (HD-2D)** games. It leverages the modern **SDL3** library and its new GPU API to render 2D sprites in a 3D world with dynamic lighting and depth, similar to the visual style of *Octopath Traveler* or *Live A Live*.

## Features (Planned & Implemented)

* **Core:** C++17/20 architecture.
* **Rendering:** Built on **SDL_GPU** (Vulkan/Direct3D12/Metal abstraction) for high-performance rendering.
* **Windowing:** SDL3 Video & Events.
* **Platform:** Windows (x64), with cross-platform capabilities via SDL3.
* **Architecture:** Custom ECS (Entity Component System) planned.

## Tech Stack

* **Language:** C++
* **IDE:** Visual Studio 2022 (MSVC Compiler)
* **Libraries:**
    * [SDL3](https://github.com/libsdl-org/SDL) (Windowing, Input, Audio, GPU)

## Getting Started

This project is set up for **Visual Studio 2022** on Windows.

### Prerequisites
* **Visual Studio 2022** with the "Desktop development with C++" workload installed.

### Installation & Building
1.  **Clone the repository:**
    ```bash
    git clone https://github.com/cow2studios/CGE.git
    ```
2.  **Open the Solution:**
    * Navigate to the `CowGameEngine/` folder.
    * Double-click `CowGameEngine.slnx`.
3.  **Build and Run:**
    * Ensure the solution configuration is set to **Debug** or **Release** and platform is **x64**.
    * Press **F5** to compile and run.

*Note: The `Dependencies/` folder is included in the repo, so you do not need to download SDL3 or other future dependancies separately.*

## Project Structure

```text
CowGameEngine/
├── CowGameEngine/          # Source code and Project files
│   ├── src/                # .h and .cpp files
│   └── CowGameEngine.slnx  # Visual Studio Solution
├── Dependencies/           # Third-party libraries
│   └── SDL3/               # SDL3 headers and libs
└── .gitignore              # VS junk filter
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
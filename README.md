# fabric-physics-opengl

- Implemented a fabric-wind relational physics engine using verlet integration for accurate simulations, and rendered the fabric using OpenGL graphics. Designed a user-friendly interface utilizing Dear Imgui, enabling user control over various simulation physics parameters, such as wind direction and texture mapping.

- [Click here for a youtube video demonstrating the program.](https://www.youtube.com/watch?v=49C1CoJly08)

Controls:

Q - Move camera up

E - Move camera down 

W - Move camera forward

S - Move camera backward

A - Move camera leftward

D - Move camera rightward

T - Disallow camera movement (useful for interacting with Imgui interface)

1 - Default rendering mode

2 - Wireframe rendering mode

3 - Dot grid rendering mode

Imgui interface allows for user to:
  - Change length and width of fabric
  - Change camera speed
  - Change fabric texture
  - Change wind patterns
  - Change rendering modes (wireframe, dot grid, normal)
  - Disable user cursor

Rendering Modes:

Normal
![Comp 1 (0-00-17-06)_1](https://user-images.githubusercontent.com/113802864/218384443-82111f6c-8a1a-42f6-88da-f60ecf2bd9f4.png)

Wireframe
![Comp 1 (0-00-25-17)](https://user-images.githubusercontent.com/113802864/218384499-0a6c4c29-7d03-4735-b0fb-69d3b4fe72e6.png)

Dot grid
![Comp 1 (0-00-30-11)](https://user-images.githubusercontent.com/113802864/218384631-07996d07-0adc-4bb4-b38c-81a0e7398588.png)

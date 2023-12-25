<h1 align = "center">CS 550 Project Three: Lighting</h1>

```
📦Indoor_positioning_system
 ┣ 📂CS550 Project Three.pdf                          
 ┣ 📂sample.cpp
 ┣ 📂README.md
```

<h2 align = "center">Instruction</h2>
<ol>
  <li>Do a keytime animation on a 3D object that is capable of being lighted (your choice). There need to be at least 8 quantities being animated, each with at least 6 keytimes that define each animation</li>
  <li>The 8 quantities to be animated must be:</li>
    <ul>
      <li>1 quantity that has something to do with a light source (position, color, etc.)</li>
      <li>1 quantity that has something to do with viewing (eye position, look position, etc.)</li>
      <li>1 quantity that has something to do with positioning, orienting, or scaling the object</li>
      <li>1 quantity that has something to do with the color of the object (r, g, or b)</li>
      <li>4 more quantities -- your choice</li>
    </ul>
  
  <li>Use lighting in the scene. Your choice of lighting parameters is up to you, but choose them so that it is easy for us to understand your scene.</li>
  <li>The object to be displayed is your choice of any 3D geometry. As lighting is required, be sure that object has surface normal vectors.</li>
  <li>Animate your quantities by smoothly interpolating them between the keytimes. Use the smooth interpolation C++ class that we discussed. For your convenience, the class methods are re-covered below.</li>
</ol>

<h2 align = "center">Goal</h2>
  <ol>
      <li>Animation lasted 10 seconds	</li>
      <li>Scene used OpenGL lighting</li>
      <li>Spot light works.</li>
      <li>Light source-related animation quantity</li>
      <li>View-related animation quantity</li>
      <li>Position/orientation/scale animation quantity</li>
      <li>Color-related animation quantity</li>
      <li>The four own-choice animation quantities</li>
  </ol>

<h2 align = "center">Project Overview</h2>
This code is designed to animate a 3D object, meticulously controlling its position, rotation, scale, and color through time-based variations. The animation sequences are set to loop over a predefined duration, with 'glutGet(GLUT_ELAPSED_TIME) % MSEC' calculating the precise moment within the cycle for consistent timing. Specifically, the object's position is defined by (Xpos1, Ypos1, Zpos1), its rotation by a single variable (Rotation), scale by (Scaled), and color by (ColorR, ColorG, ColorB). The camera's perspective is managed via 'gluLookAt', leveraging coordinates (eyePosX, eyePosZ) to adjust the viewpoint dynamically. Additionally, the lighting within the scene is not static; it changes over time with specified colors (LightColR, LightColG, LightColB) to create a vivid and engaging visual experience. These collective features ensure a seamless and dynamic animation that brings the object and scene to life.

<h2 align = "center">Project Display</h2>
  <a href="https://www.youtube.com/watch?v=A4PPt6FNj8s&list=PLJYQYc0rOVblXTzpB1ZzHmx__AAjGlwuk&index=4" title="Project Display">
    <img src="http://img.youtube.com/vi/A4PPt6FNj8s/0.jpg" alt="Project Display" style="display:block; margin:auto;">
  </a>

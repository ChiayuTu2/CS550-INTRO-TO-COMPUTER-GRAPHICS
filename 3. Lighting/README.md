<h1 align = "center">CS 550 Project Three: Lighting</h1>

```
📦Indoor_positioning_system
 ┣ 📂CS550 Project Three.pdf                          
 ┣ 📂sample.cpp
 ┣ 📂README.md
```

<h2 align = "center">Instruction</h2>
<ol>
  <li>Create a 3D scene consisting of at least 3 objects.</li>
    <ul>
      <li>At least one object must come from an OBJ file (your choice). The other two can be anything you want (even other, different, OBJ objects). Just be sure that all objects have surface normal vectors attached to them.</li>
      <li>At least one object must be dull and at least one must be shiny (GL_SHININESS).</li>
      <li>The objects must each have a different material color.</li>
    </ul>
  <li>Place a grid under the scene. I will give you the code to do this.</li>
  <li>Have a light source that travels above the scene in some motion pattern. The exact path is up to you. (A circle works well.) The light source needs to be able to be toggled between a point light and a spot light by using the 'p' and 's' keyboard keys. When it is a spot light, it must be pointing down at the scene. You will only use perspective in this project, not orthographic, so the 'p' key can be used for a point light.</li>
  <li>Have the light source's color be toggled between being white, red, green, blue, and yellow by using the 'w', 'r', 'g', 'b', and 'y' keyboard keys.</li>
  <li>Put a small sphere at the light source location so that we can see where it is. Make the sphere the same color as the light source it is representing. Don't use lighting on the light-source sphere. Just make it a glColor3f blob with lighting disabled.</li>
  <li>All OBJ files will need to have per-vertex surface normal vectors for the lighting to work. Edit the .obj file and look for lines that begin with the letters vn to see if it does.</li>
  <li>Remember that OpenGL light positions automatically get transformed by the GL_MODELVIEW transformation as it exists at the moment you specify the light position. You can't prevent this from happening. You can only control the GL_MODELVIEW matrix to be what you need it to be at the time and control where in your code you specify the light source positions.</li>
  <li>Feel free to use the SetLight functions from the Lighting notes. This file can be enabled in your sample.cpp file by uncommenting its #include.</li>
  <li>Leave the atteniation set to "no attenuation", like is in the notes. You can do anything with the attenuation that you'd like. I recommend you set it to "no attenuation" (C=1., L=0., Q=0.), at least at first, so that you can see everything. The SetLight functions already do that.</li>
</ol>

<h2 align = "center">Goal</h2>
  <ol>
      <li>At least three objects.</li>
      <li>Point light works.</li>
      <li>Spot light works.</li>
      <li>Light in some pattern above the scene.</li>
      <li>Light changes color.</li>
      <li>There is a small unlit sphere to show where the light source is.</li>
  </ol>

<h2 align = "center">Project Overview</h2>
In this project, I utilized three distinct OBJ files representing a cow, a car, and a salmon, each assigned its own unique color for differentiation. The position of the lighting is dynamically calculated using trigonometric functions, allowing the light to traverse a circular path with a radius of 2.0f and a height of 3.0f. I employed OpenGL's 'GL_LIGHT0' for setting up the light source, integrating versatility in illumination by incorporating code in the Keyboard function to toggle between a point light and a spotlight. In the spotlight mode, the light is oriented directly downwards with a 45-degree cut-off angle, simulating a focused beam of light.

To visually represent the light source within the 3D space, I used the 'OsuSphere' function to create a sphere that moves in synchronization with the actual light source. This feature not only enhances the visual representation of the scene but also aids in understanding the interaction between light and objects in the environment.

<h2 align = "center">Project Display</h2>
  <a href="https://www.youtube.com/watch?v=lL8ACTLQHJs&list=PLJYQYc0rOVblXTzpB1ZzHmx__AAjGlwuk&index=3" title="Project Display">
    <img src="http://img.youtube.com/vi/lL8ACTLQHJs/0.jpg" alt="Project Display" style="display:block; margin:auto;">
  </a>

  

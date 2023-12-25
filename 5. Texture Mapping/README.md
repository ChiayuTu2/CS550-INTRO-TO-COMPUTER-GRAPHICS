<h1 align = "center">CS 550 Project Five: Texture Mapping</h1>

```
📦Indoor_positioning_system
 ┣ 📂CS550 Project Five.pdf                          
 ┣ 📂sample.cpp
 ┣ 📂venus.bmp
 ┣ 📂earth.bmp
 ┣ 📂mars.bmp
 ┣ 📂jupiter.bmp
 ┣ 📂saturn.bmp
 ┣ 📂uranus.bmp
 ┣ 📂neptune.bmp
 ┣ 📂README.md
```

<h2 align = "center">Instruction</h2>
<ol>
  <li>Save each texture to your own file area.</li>
  <li>In your code, read each texture (the BmpToTexture function) and put each texture into a texture object (i.e., bind it to a texture name).</li>
  <li>Create a display list that holds an OsuSphere with radius=1.0.</li>
  <li>Create other display lists that hold each planet, properly scaled, and with the appropriate texture bound.</li>
  <li>Place a moving point light source somewhere in the scene so that you will be able to demonstrate that your GL_MODULATE mode works. The exact light source motion is not crucial, so long as it adequately demonstrates dynamic lighting on your textures.</li>
  <li>Under control of a keyboard hit, change the planet being displayed, in the correct scale, with the correct texture. Perhaps use the keys 'v', 'e', 'm', 'j', 's', 'u', 'n'.</li>
  <li>Under control of another keyboard hit, toggle between:</li>
    <ul>
      <li>No texture -- just a lit white sphere with the proper scale</li>
      <li>Proper scale with the texture image GL_REPLACE'ed</li>
      <li>Proper scale with the texture image GL_MODULATED'ed</li>
    </ul>
</ol>

<h2 align = "center">Goal</h2>
  <ol>
      <li>Draw the spheres with the correct scaling	</li>
      <li>Draw the spheres with the correct GL_REPLACE textures	</li>
      <li>Draw the spheres with the correct GL_MODULATE textures</li>
      <li>The light source moves</li>
  </ol>

<h2 align = "center">Project Overview</h2>
  <p>The program functions as an advanced space simulation tool, meticulously crafting and manipulating textured representations of planetary bodies. At its core is a 'planet' structure designed to encapsulate vital characteristics of each celestial body, including its designation, texture files, scaling parameters, display list identifiers, interactive keys, and texture object identifiers. This structure facilitates the dynamic loading of bitmap textures, which are then precisely mapped onto spherical models representing the planets.</p>
  <p>Interactivity is a key feature of the program, allowing users to toggle between various rendering modes via keyboard inputs. These modes include a texture mode where planets are vividly rendered with detailed surface textures, and a light mode that employs lighting effects to accentuate the textures' depth and realism. Additionally, the simulation incorporates movable light sources, enabling users to witness the shifting interplay of light and shadow on planetary surfaces.</p>
  <p>Crucially, the program doesn't just visualize planets; it scales them according to their real-life size ratios, providing an authentic and educational representation of the solar system. Whether for educational purposes, entertainment, or scientific visualization, this tool brings the cosmos to the user's fingertips, offering a rich, interactive experience of space exploration.</p>

<h2 align = "center">Project Display</h2>
  <a href="https://www.youtube.com/watch?v=vdCYjiNl7qc&list=PLJYQYc0rOVblXTzpB1ZzHmx__AAjGlwuk&index=5" title="Project Display">
    <img src="http://img.youtube.com/vi/vdCYjiNl7qc/0.jpg" alt="Project Display" style="display:block; margin:auto;">
  </a>


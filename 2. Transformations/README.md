<h1 align = "center">CS 550 Project Two: Using Transformations to Animate a Carousel Horse!</h1>

```
📦Indoor_positioning_system
 ┣ 📂CS550 Project Two.pdf                          
 ┣ 📂sample.cpp
 ┣ 📂CarouselHorse0.10.550
 ┣ 📂README.md
```

<h2 align = "center">Instruction</h2>
    <ol>
        <li>Draw a horizontal circle with radius 2.0 in the X-Z plane (see above) to show the path of the horse.</li>
        <li>Draw a carousel horse into a display list. Don't worry -- this won't be as hard as it sounds. The code for creating the horse geometry is shown below in the Geometry section. The horse's hooves are in the X-Z plane. The top of the horse points up in +Y. The horse's head faces in +X.</li>
        <li>Each horse will have 4 transformations, not necessarily in this order:
    		<ul>
        		<li>The horse needs to translate up and down</li>
        		<li>The horse needs to revolve in a circle</li>
        		<li>The horse needs to be translated from the origin to the outside of the circle</li>
        		<li>The horse needs to rock back and forth (like a rocking horse). If this was an airplane, we would call this motion "pitching".</li>
    		</ul>
	</li>
        <li>Allow two views: an "Outside" view of the entire scene and an "Inside" view from the center of the carousel looking outward. Be able to switch between them in your video. (You could use a pop-up menu or a keyboard hit) For each view, use a different call to gluLookAt( ) to position the eye.</li>
        <li>Keep the same Xrot, Yrot, and Scale features as we've used before, but only in the Outside View. Do not use Xrot, Yrot, and Scale in the Inside View.</li>
        <li>Use gluPerspective( ), for both views. Only allow a switch to glOrtho( ) in the Outside view.</li>
	<li>Use the graphics programming strategy where the Display( ) function looks at a collection of global variables and draws the scene correctly. The other parts of the program set the global variables and post a redisplay.
</li>
    </ol>

<h2 align = "center">Goal</h2>
  <ol>
      <li>Draw a horse body with the "fake lighting"</li>
      <li>The horse moves in a circle</li>
      <li>The horse bobs up and down</li>
      <li>The horse rocks (pitches)</li>
      <li>Able to switch views</li>
      <li>Able to animate 4 horses</li>
  </ol>

<h2 align = "center">Project Overview</h2>
I approached the task methodically by dividing it into five sequential steps for this project. Firstly, I programmed a function to render a red circle with a radius of 2.0f. Secondly, I integrated the horse wireframe within the InitLists() function. Thirdly, within the Animate() function, I established a rotation mechanism for the horse: it continuously rotates by 0.5f until completing a full circle, at which point it resets. Concurrently, the horse executes a rocking motion around the red circle, dictated by a theta value derived from sinusoidal functions to simulate natural movement.
I focused on the user's perspective, implementing inside and outside views using gluLookAt(). This function adjusts the eye point, reference point, and the 'up' direction for immersive viewing experiences. Users can toggle between these views via a DoViewMenu() function incorporated into InitMenus(). Finally, the fifth step involves duplicating the horse model to create a total of four horses, each positioned 90 degrees apart around the circle. The placement is calculated using trigonometric functions to determine their exact positions on the x and z-axes, relative to the circle's center.

<h2 align = "center">Project Display</h2>
  <a href="http://www.youtube.com/watch?v=6LcDjKYh87E" title="Project Display">
    <img src="http://img.youtube.com/vi/6LcDjKYh87E/0.jpg" alt="Project Display" style="display:block; margin:auto;">
  </a>


